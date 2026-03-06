/*
 * lsm5ds3.c
 *
 *  Created on: Feb 27, 2026
 *      Author: ChatDeepseekGPTCapilot
 */

#include "lsm6ds3.h"
#include "delay/i2c-crutch.h"

int32_t lsm6ds3_write (void *handle, uint8_t sub, const uint8_t *data, uint16_t len)
{
	lsm6ds3_data__t *ptr = (lsm6ds3_data__t *)handle;
	uint8_t buf_lsm6ds3[2] = {0};
	for (int i = 0; i < len; i++)
	{
		buf_lsm6ds3[0] = sub + i;
		buf_lsm6ds3[1] = data[i];

		HAL_StatusTypeDef transmit = HAL_I2C_Master_Transmit(ptr->hi2c1, ptr->add, buf_lsm6ds3, 2, 100);

		if (transmit != HAL_OK)
		{
			if (transmit == HAL_BUSY)
			{
				I2C_ClearBusyFlagErratum(ptr->hi2c1, 100);
			}
			return transmit;
		}
	}
	return HAL_OK;
}




int32_t lsm6ds3_read (void *handle, uint8_t sub, uint8_t *data, uint16_t len)
{
	lsm6ds3_data__t *ptr = (lsm6ds3_data__t *)handle;
	HAL_StatusTypeDef transmit = HAL_I2C_Master_Transmit(ptr->hi2c1, ptr->add, &sub, 1, 100);
	if (transmit != HAL_OK)
	{
		if (transmit == HAL_BUSY)
		{
			I2C_ClearBusyFlagErratum(ptr->hi2c1, 100);
		}
		return transmit;
	}
	transmit = HAL_I2C_Master_Receive(ptr->hi2c1, ptr->add, data, len, 150);
	if (transmit != HAL_OK)
	{
		if (transmit == HAL_BUSY)
		{
			I2C_ClearBusyFlagErratum(ptr->hi2c1, 100);
		}
		return transmit;
	}
	return HAL_OK;
}
