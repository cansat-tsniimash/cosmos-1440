/*
 * lis2mdl.h
 *
 *  Created on: Mar 7, 2026
 *      Author: ChatDeepseekGPTCapilot
 */

#ifndef LIS2MDL_LIS2MDL_H_
#define LIS2MDL_LIS2MDL_H_
#include "lis2mdl_reg.h"
#include "stm32f1xx.h"
#include "lis2mdl.h"

typedef struct lis2mdl_data
{
	uint8_t add;
	I2C_HandleTypeDef *hi2c1;
}lis2mdl_data__t;

int32_t lis2mdl_write (void *handle, uint8_t sub, uint8_t *data, uint16_t len);
int32_t lis2mdl_read (void *handle, uint8_t sub, uint8_t *data, uint16_t len);

#endif /* LIS2MDL_LIS2MDL_H_ */
