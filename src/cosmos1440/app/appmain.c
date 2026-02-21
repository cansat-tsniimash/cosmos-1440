/*
 * appmain.c
 *
 *  Created on: Feb 7, 2026
 *      Author: ChatDeepseekGPTCapilot
 */
#include "stm32f1xx.h"
#include "delay/dwt_delay.h"
#include "ds18b20/bs18b20.h"
#include "neo6mv2/neo6mv2.h"


extern UART_HandleTypeDef huart1;


void appmain(void)
{
//	GPS
	neo6mv2_Init();
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_ERR);
	GPS_Data gps_data;


//	DS18B20
	ds18b20_init(DS18B20_12_BIT);
	uint32_t ds_stert_time = HAL_GetTick();
	ds18b20_conv();

	while (1)
	{
		for (int i = 0; i < 10; i++)
		{
			if (neo6mv2_work())
			{
				break;
			}
		}
		gps_data = neo6mv2_GetData();
		printf("latitude = %f\n", gps_data.latitude);
		printf("longitude = %f\n", gps_data.longitude);
		printf("altitude = %f\n", gps_data.altitude);
		printf("fixQuality = %i\n", gps_data.fixQuality);

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


