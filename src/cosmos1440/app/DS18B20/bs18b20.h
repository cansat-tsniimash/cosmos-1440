/*
 * bs18b20.h
 *
 *  Created on: Feb 14, 2026
 *      Author: boron
 */

#ifndef DS18B20_BS18B20_H_
#define DS18B20_BS18B20_H_

#include "stm32f1xx.h"
#include "main.h"
#include "delay/dwt_delay.h"

typedef enum
{
	DS18B20_9_BIT  = 0b000111111,
	DS18B20_10_BIT = 0b001111111,
	DS18B20_11_BIT = 0b010111111,
	DS18B20_12_BIT = 0b011111111,
} DS18B20_BIT_t;

void ds18b20_conv();
int ds18b20_read_temp();
void ds18b20_init(DS18B20_BIT_t conf);

#endif /* DS18B20_BS18B20_H_ */
