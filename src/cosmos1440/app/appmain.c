/*
 * appmain.c
 *
 *  Created on: Feb 7, 2027
 *      Author: ChatDeepseekGPTCapilot
 */
#include "stm32f1xx.h"
#include "delay/dwt_delay.h"
#include "ds18b20/bs18b20.h"
#include "neo6mv2/neo6mv2.h"
#include "BPM280/bmp280.h"


extern UART_HandleTypeDef huart1;
extern I2C_HandleTypeDef hi2c1;



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

	// BMP280

//	сделать функцию
	struct bme280_dev bmp;
	bmp.intf = BME280_I2C_INTF;
	bmp.read = bmp280_read_reg;
	bmp.write = bmp280_write_reg;
	bmp.delay_us = bmp280_delay_us;

	bmp.settings.osr_p = BME280_OVERSAMPLING_16X;
	bmp.settings.osr_t = BME280_OVERSAMPLING_16X;
	bmp.settings.filter = BME280_FILTER_COEFF_16;
	bmp.settings.standby_time = BME280_STANDBY_TIME_20_MS;

	bmp280_bus_t bmp_bus;
	bmp_bus.addr = 0x76 << 1;
	bmp_bus.hi2c1 = &hi2c1;
	bmp.intf_ptr = &bmp_bus;

	bme280_init(&bmp);
	bme280_set_sensor_settings(BME280_ALL_SETTINGS_SEL, &bmp);
	bme280_set_sensor_mode(BME280_NORMAL_MODE, &bmp);


	struct bme280_data bmp_data;



	while (1)
	{
		bme280_get_sensor_data(BME280_PRESS | BME280_TEMP, &bmp_data, &bmp);

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

		// Алгоритм работы аппарата.

		uint8_t stage = 0;

		uint32_t rocket_timer = 0;
		uint32_t light_timer = 0;


		/*switch(stage)
		    {
		        case 0:

		            neo6mv2_Init();
		            ds18b20_init(DS18B20_12_BIT);
		            ds18b20_conv();

		            stage = 1;
		            break;

		        case 1:

		            if (rocket_timer == 0)
		                rocket_timer = HAL_GetTick();

		            neo6mv2_work();
		            GPS_Data gps = neo6mv2_GetData();

		            if (HAL_GetTick() - rocket_timer >= 15000)
		            {
		                stage = 2;
		            }

		            break;

		        case 2:

		            neo6mv2_work();
		            GPS_Data gps = neo6mv2_GetData();

		            if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET)
		            {
		                if (light_timer == 0)
		                    light_timer = HAL_GetTick();

		                if (HAL_GetTick() - light_timer >= 2000)
		                {
		                    // подтверждено изменение освещенности
		                }
		            }
		            else
		            {
		                light_timer = 0;
		            }

		           // если высота не изменяется, ставим strage = 3;
		           //+- есть идея по реализации, использовать GPS, вытащить из нее данные высоты и сравнивать их в промежутке времени или что-то вроде того.

		            break;

		        case 3:

		            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET); // Пищалка

		            neo6mv2_work();
		            gps = neo6mv2_GetData();

		            break;
		    }*/
	}
	return;
}


