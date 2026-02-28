/*
 * bmp280.c
 *
 *  Created on: Feb 27, 202
 *      Author: ChatDeepseekGPTCapilot
 */
#include "stm32f1xx.h"
#include "BPM280/bmp280.h"
#include "delay/dwt_delay.h"

// this


BME280_INTF_RET_TYPE bmp280_read_reg(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
	bmp280_bus_t *ptr = (bmp280_bus_t *)intf_ptr;
	HAL_StatusTypeDef transmit = HAL_I2C_Master_Transmit(ptr->hi2c1, ptr->addr, &reg_addr, 1, 100);
	if (transmit != HAL_OK)
	{
		return transmit;
//		this
	}
	HAL_StatusTypeDef receive = HAL_I2C_Master_Receive(ptr->hi2c1, ptr->addr, reg_data, len, 100);

	if (receive != HAL_OK)
	{
		return receive;
//		this
	}
	return HAL_OK;
}

BME280_INTF_RET_TYPE bmp280_write_reg (uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
	bmp280_bus_t *ptr = (bmp280_bus_t *)intf_ptr;
	uint8_t buf[2] = {0};

	for (int i = 0; i < len; i++)
	{
		buf[0] = reg_addr + i;
		buf[1] = reg_data[i];
		HAL_StatusTypeDef transmit = HAL_I2C_Master_Transmit(ptr->hi2c1, ptr->addr, buf, 2, 100);

		if (transmit != HAL_OK)
		{
			return transmit;
		}
	}
	return HAL_OK;
}

void bmp280_delay_us(uint32_t period, void *intf_ptr)
{
	dwt_delay_us(period);
}

