/*
 * appmain.c
 *
 *  Created on: Feb 27, 2027
 *      Author: ChatDeepseekGPTCapilot
 */
#include <DS18B20/ds18b20.h>
#include "stm32f1xx.h"
#include "delay/dwt_delay.h"
#include "neo6mv2/neo6mv2.h"
#include "BPM280/bmp280.h"
#include "LSM6DS3/lsm6ds3.h"


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


//	lsm6ds3

	lsm6ds3_data__t lsm_bus;
	lsm_bus.add = 0x6A << 1;
	lsm_bus.hi2c1 = &hi2c1;

	stmdev_ctx_t lsm6ds3;
	lsm6ds3.handle = &lsm_bus;
	lsm6ds3.read_reg = lsm6ds3_read;
	lsm6ds3.write_reg = lsm6ds3_write;

	lsm6ds3_reset_set(&lsm6ds3, 1);
	lsm6ds3_xl_full_scale_set(&lsm6ds3, LSM6DS3_16g);
	lsm6ds3_xl_data_rate_set(&lsm6ds3, LSM6DS3_XL_ODR_104Hz);
	lsm6ds3_gy_full_scale_set(&lsm6ds3, LSM6DS3_2000dps);
	lsm6ds3_gy_data_rate_set(&lsm6ds3, LSM6DS3_GY_ODR_208Hz);

	int16_t buf_lsm_gy[3] = {0};
	int16_t buf_lsm_xl[3] = {0};
	volatile float gyro[3] = {0};
	volatile float acc[3] = {0};


	while (1)
	{
		bme280_get_sensor_data(BME280_PRESS | BME280_TEMP, &bmp_data, &bmp);

		lsm6ds3_acceleration_raw_get(&lsm6ds3, buf_lsm_xl);
		lsm6ds3_angular_rate_raw_get(&lsm6ds3, buf_lsm_gy);

		for (int i = 0; i < 3; i++)
		{
			acc[i] = lsm6ds3_from_fs16g_to_mg(buf_lsm_xl[i]) / 1000.0;
			gyro[i] = lsm6ds3_from_fs125dps_to_mdps(buf_lsm_gy[i])/ 1000.0;
		}

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


/*		switch(stage)
		{

			case 1:

				if (rocket_timer == 0)
					rocket_timer = HAL_GetTick();

				if (HAL_GetTick() - rocket_timer >= 15000)
				{
					stage = 2;
				}

				break;

			case 2:

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

				break;
		}*/
	}
	return;
}


