/*
 * ds18b20.c
 *
 *  Created on: Feb 7, 2026
 *      Author: GPT
 */

#include "stm32f1xx.h"
#include "main.h"
#include "delay/dwt_delay.h"

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
