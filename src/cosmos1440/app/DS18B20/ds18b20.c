/*
 * ds18b20.c
 *
 *  Created on: Feb 7, 2026
 *      Author: ChatDeepseekGPTCapilot
 */


#include "bs18b20.h"

/*	  HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_RESET);

	  Configure GPIO pin : DS18B20_Pin
	  GPIO_InitStruct.Pin = DS18B20_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(DS18B20_GPIO_Port, &GPIO_InitStruct); */

void one_wire_force_down()
{
	  GPIO_InitTypeDef ds_pin = {0};
	  ds_pin.Pin = DS18B20_Pin;
	  ds_pin.Pull = GPIO_NOPULL;
	  ds_pin.Speed = GPIO_SPEED_FREQ_HIGH;
	  ds_pin.Mode = GPIO_MODE_OUTPUT_OD;
	  HAL_GPIO_Init(DS18B20_GPIO_Port, &ds_pin);
	  HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_RESET);
}

void one_wire_release()
{
	  GPIO_InitTypeDef ds_pin = {0};
	  ds_pin.Pin = DS18B20_Pin;
	  ds_pin.Pull = GPIO_NOPULL;
	  ds_pin.Speed = GPIO_SPEED_FREQ_HIGH;
	  ds_pin.Mode = GPIO_MODE_INPUT;
	  HAL_GPIO_Init(DS18B20_GPIO_Port, &ds_pin);
}

void one_wire_write_bit(uint8_t bit)
{
	if (bit == 0)
	{
		one_wire_force_down();
		dwt_delay_us(60);
		one_wire_release();
	}
	else
	{
		one_wire_force_down();
		dwt_delay_us(1);
		one_wire_release();
		dwt_delay_us(59);
	}
}

//

void one_wire_write_byte(uint8_t byte)
{
	for (int i = 0; i < 8; i++)
	{
		if (((1 << i) & byte) == 0)
		{
			one_wire_write_bit(0);
		}
		else
		{
			one_wire_write_bit(1);
		}
	}
}



GPIO_PinState one_wire_read_bit(void)
{
    one_wire_force_down();
    //dwt_delay_us(2);

    one_wire_release();
    //dwt_delay_us(10);

    GPIO_PinState bit = HAL_GPIO_ReadPin(DS18B20_GPIO_Port, DS18B20_Pin);

    dwt_delay_us(60 - 12);

    return bit;
}

uint8_t one_wire_read_byte()
{
	uint8_t num = 0;
	for (int i = 0; i < 8; i++)
	{
		if (one_wire_read_bit() == GPIO_PIN_SET)
			num = (1 << i) | num;
	}
	return num;
}

uint8_t one_wire_reset()
{
	one_wire_force_down();
	dwt_delay_us(500);
	one_wire_release();
	dwt_delay_us(50);
	GPIO_PinState bit = HAL_GPIO_ReadPin(DS18B20_GPIO_Port, DS18B20_Pin);
	dwt_delay_us(480-50);

	return bit == GPIO_PIN_RESET;
}

void ds18b20_conv()
{
	one_wire_reset();
	one_wire_write_byte(0xCC);
	one_wire_write_byte(0x44);
}

int ds18b20_read_temp()
{
	volatile uint8_t buf[8] = {0};
	one_wire_reset();
	one_wire_write_byte(0xCC);
	one_wire_write_byte(0xBE);
	for (int i = 0; i < 8; i++)
	{
		buf[i] = one_wire_read_byte();
	}

	return (int16_t)((buf[1] << 8) | buf[0]) / 16.0;
}


void ds18b20_init(DS18B20_BIT_t conf)
{
	dwt_delay_init();
	one_wire_write_byte(0xCC);
	one_wire_write_byte(0x4E);
	one_wire_write_byte(0);
	one_wire_write_byte(0);
	one_wire_write_byte(conf);
;}
