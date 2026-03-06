/*
 * lsm6ds3.h
 *
 *  Created on: Mar 6, 2026
 *      Author: boron
 */

#ifndef LSM6DS3_LSM6DS3_H_
#define LSM6DS3_LSM6DS3_H_
#include "lsm6ds3_reg.h"
#include "stm32f1xx.h"

typedef struct lsm6ds3_data
{
	uint8_t add;
	I2C_HandleTypeDef *hi2c1;
}lsm6ds3_data__t;

int32_t lsm6ds3_write (void *handle, uint8_t sub, const uint8_t *data, uint16_t len);
int32_t lsm6ds3_read (void *handle, uint8_t sub, uint8_t *data, uint16_t len);


#endif /* LSM6DS3_LSM6DS3_H_ */
