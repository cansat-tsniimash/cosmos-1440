/*
 * appmain.c
 *
 *  Created on: Feb 7, 2026
 *      Author: ChatDeepseekGPTCapilot
 */
#include "stm32f1xx.h"
#include "delay/dwt_delay.h"
#include "ds18b20/bs18b20.h"





void appmain(void)
{
	ds18b20_init(DS18B20_12_BIT);
	uint32_t ds_stert_time = HAL_GetTick();
	ds18b20_conv();
	while (1)
	{

//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
//		HAL_Delay(100);
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
//		HAL_Delay(100);

		if (HAL_GetTick() - ds_stert_time >= 750)
		{
			ds18b20_read_temp();
			ds_stert_time = HAL_GetTick();
			ds18b20_conv();
		}
		//HAL_Delay(850);

	}
	return;
}


