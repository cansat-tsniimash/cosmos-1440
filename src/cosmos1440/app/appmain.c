/*
* appmain.c
*
*  Created on: Feb 27, 2027
*      Author: ChatDeepseekGPTCapilot
*/
#include <bitwise_XOR/bitwise_XOR.h>
#include <DS18B20/ds18b20.h>
#include "stm32f1xx.h"
#include "delay/dwt_delay.h"
#include "neo6mv2/neo6mv2.h"
#include "BPM280/bmp280.h"
#include "LSM6DS3/lsm6ds3.h"
#include "lis2mdl/lis2mdl_reg.h"
#include "lis2mdl/lis2mdl.h"
#include "ff.h"
#include "ff_gen_drv.h"
#include "E220-400T22S/e220-400t22s.h"
#include "operation_algoritm/algoritm_operation.h"
#include "bitwise_XOR/bitwise_XOR.h"


extern UART_HandleTypeDef huart1;
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;
extern operation_algoritm_t status_t;

#pragma pack(push, 1)
typedef struct {
uint16_t start; // OK
uint16_t team_id; // OK
uint32_t time; // OK
int16_t temp; // notOK
uint32_t pressure; // давление  OK
int16_t acc[3]; // ускорение OK
int16_t gyro[3]; // угловая скорость OK
uint8_t sum; //

uint16_t number;
uint8_t status;
float latitude_gps; // OK
float longitude_gps; // OK
float altitude_gps; // OK
uint8_t fix_gps; // OK
uint16_t fotores;
int16_t mag[3]; // OK
int16_t ds18b20; // OK
float quaternion_a;
float quaternion_b;
float quaternion_c;
float quaternion_d;
int16_t corner_right;
int16_t corner_left;
uint8_t cosmos1440_sum;
} packet_t;
#pragma pack(pop)

#define ORG_PACK_SIZE (27)

void appmain(void)
{
	packet_t packet = {0};
	packet.start = 0xAAAA;
	packet.team_id = 0xBBBB;


	//	GPS
	neo6mv2_Init();
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_ERR);
	GPS_Data gps_data;

	//	DS18B20 - термометр

	ds18b20_init(DS18B20_12_BIT);
	uint32_t ds_stert_time = HAL_GetTick();
	ds18b20_conv();

	// BMP280 - барометр/термометр

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


	//	lis2mdl

	lis2mdl_data__t lis2_bus;
	lis2_bus.add = 0x1E << 1;
	lis2_bus.hi2c1 = &hi2c1;

	lis2mdl_ctx_t lis2mdl;

	lis2mdl.handle = &lis2_bus;
	lis2mdl.read_reg = lis2mdl_read;
	lis2mdl.write_reg = lis2mdl_write;

	lis2mdl_reset_set(&lis2mdl, 1);
	lis2mdl_data_rate_set(&lis2mdl, LIS2MDL_ODR_50Hz);
	lis2mdl_power_mode_set(&lis2mdl, LIS2MDL_HIGH_RESOLUTION);
	lis2mdl_operating_mode_set(&lis2mdl, LIS2MDL_CONTINUOUS_MODE);

	int16_t buf_lis2[3] = {0};
	volatile float mag[3] = {0};

	// E220-400T22S

	e220_bus_t e220;
	e220.huart = &huart2;
	e220.aux_pin = GPIO_PIN_3;
	e220.aux_port = GPIOB;
	e220.m0_pin = GPIO_PIN_1;
	e220.m0_port = GPIOB;
	e220.m1_pin = GPIO_PIN_0;
	e220.m1_port = GPIOB;

	e220_change_mode(&e220, E220_MODE_SM);
	HAL_Delay(100);
	e220_set_channel(&e220, 5);
	HAL_Delay(50);
	e220_set_addr(&e220, 0xAAAA);
	HAL_Delay(50);
	e220_set_reg0(&e220, E220_AIR_RATE_9P6K, E220_UART_RATE_9600, E220_SERIAL_PARITY_8N1);
	HAL_Delay(50);
	e220_set_reg1(&e220, E220_SUB_PACKET_200_BYTES, E220_RSSI_AMBIENT_NOISE_DISABLE, E220_TRANSMITTING_POWER_17DBM);
	HAL_Delay(50);
	e220_change_mode(&e220, E220_MODE_NM);
	HAL_Delay(100);

	// sd-card

	FATFS fleska;
	FIL paket_fille;
	char paker_path[] = "paket.bin";
	FRESULT rizult_mount = f_mount(&fleska, "", 1);
	FRESULT rizult_paket = 255;
	UINT byte_count;
	uint8_t time = HAL_GetTick();

	float first_pressure = bmp_data.pressure;

	operation_algoritm_t mission_status = OA_PREPARATION;
	uint8_t count_fotores = 0;


	while (1)
	{
		float altitude = 44330 * (1 - pow((float)bmp_data.pressure / first_pressure, (1.0 / 5.255)));

		switch (mission_status)
		{
			case OA_CHECK_LIGHT:
				if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_SET)
				{
//					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
					if (count_fotores <= 10)
					{
						// ТУТ СОБИРАЕМ ДАННЫЕ ОБ ОСВЕЩЕННОСТИ И ЗАПИСЫВАЕМ В ПЕРЕМЕННУЮ. СДЕЛАТЬ, ЧТОБЫ В ПЕРЕМЕННУЮ ЗАПИСЫВАЛОСЬ УРОВЕНЬ ОСВЕЩЕННОСТИ.
						for (int i = 0; i < count_fotores; i++)
						{
							// получаю уровень освещенности и записываю в массив с индекосм count_fotores


							count_fotores++;
						}
						break;
					}
				else
				{
//					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
					time = HAL_GetTick();
					mission_status = OA_PREPARATION;
					// считаю среднее значение элемента массива и записываю в переменную
					break;
				}



				}
			case OA_PREPARATION:
				if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_SET)
				{
//					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
					if (HAL_GetTick() - time >= 15000)
					{
//						HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
						mission_status = OA_ROCKET;
						time = HAL_GetTick();
						break;
					}
				}
				else
				{
					time = HAL_GetTick();
					break;
				}


			case OA_ROCKET:
//				if (/*ЕСЛИ СВЕТ СЕЙЧАС > ПЕРЕМЕННОЙ С ОСВЕЩЕННОСТЬЮ */)
//				{
//					if (HAL_GetTick() - time >= 2000)
//					{
//						mission_status = OA_FIFRE;
//						time = HAL_GetTick();
//						break;
//					}
//				}

			case OA_FIFRE:
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET); // Криматорий вкл
				if (HAL_GetTick() - time >= 2000)
				{
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET); // Криматорий выкл
					mission_status = OA_DECLINE;
					time = HAL_GetTick();
					break;
				}

			case OA_DECLINE:
				if (altitude < 10)
				{
//					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
					break;
				}



		}




		bme280_get_sensor_data(BME280_PRESS | BME280_TEMP, &bmp_data, &bmp);
		packet.temp = bmp_data.temperature * 100;
		packet.pressure = bmp_data.pressure;

		lsm6ds3_acceleration_raw_get(&lsm6ds3, buf_lsm_xl);
		lsm6ds3_angular_rate_raw_get(&lsm6ds3, buf_lsm_gy);
		for (int i = 0; i < 3; i++)
		{
			packet.acc[i] = buf_lsm_xl[i];
			packet.gyro[i] = buf_lsm_gy[i];

		}

		lis2mdl_magnetic_raw_get(&lis2mdl, buf_lis2);
		for (int i = 0; i < 3; i++)
		{
			packet.mag[i] = buf_lis2[i];
		}

		for (int i = 0; i < 10; i++)
		{
			if (neo6mv2_work())
			{
				break;
			}
		}
		gps_data = neo6mv2_GetData();
		packet.latitude_gps = gps_data.latitude;
		packet.longitude_gps = gps_data.longitude;
		packet.altitude_gps = gps_data.altitude;
		packet.fix_gps = gps_data.fixQuality;

	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
	//HAL_Delay(100);
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	//HAL_Delay(100);

		if (HAL_GetTick() - ds_stert_time >= 750)
		{
			packet.ds18b20 = ds18b20_read_temp();
			ds_stert_time = HAL_GetTick();
			ds18b20_conv();
		}

		//HAL_Delay(850);
		packet.time = HAL_GetTick();
		packet.number++;
		packet.sum = xor_summ((uint8_t *)&packet, ORG_PACK_SIZE - 1);
		packet.cosmos1440_sum = xor_summ(((uint8_t *)&packet + ORG_PACK_SIZE), sizeof(packet_t) - ORG_PACK_SIZE - 1);


		e220_send_packet(&e220, (uint8_t *)&packet, sizeof(packet_t));

		if (rizult_mount != FR_OK)
		{
			f_mount(NULL, "", 1);
			extern Disk_drvTypeDef disk;
			disk.is_initialized[0] = 0;
			rizult_mount = f_mount(&fleska, "", 1);
		}

		if (rizult_paket != FR_OK && rizult_mount == FR_OK)
		{
			if (rizult_paket != 255)
			f_close(&paket_fille);
			rizult_paket = f_open(&paket_fille, (const TCHAR*)&paker_path, FA_WRITE | FA_OPEN_ALWAYS | FA__WRITTEN);
			rizult_mount = f_mount(&fleska, "", 1);
		}
		if (rizult_paket == FR_OK && rizult_mount == FR_OK)
		{
			rizult_paket = f_write(&paket_fille, &packet, sizeof(packet_t), &byte_count);
			f_sync(&paket_fille);
		}
	}
	return;
}

// TODO: реализовать подсчет суммы организаторов и нашу

