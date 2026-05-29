/*
* appmain.c
*
*  Created on: Feb 27, 2027
*      Author: ChatDeepseekGPTCapilot
*
*      WE BELIVE IN THIS CODE
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
#include "fotores/fotores.h"
#include <magic/magic.h>
#include <math.h>
#include "math/math.h"

extern UART_HandleTypeDef huart1;
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim2;

#pragma pack(push, 1)
typedef struct {
uint16_t start; // OK
uint16_t team_id; // OK
uint32_t time; // OK
int16_t temp; // ok
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

typedef struct {
	float latitude_target_gps;
	float longitude_target_gps;
	float altitude_target_gps;
} dataGPS_t;

#define ZERO_1 (10) // 10
#define ZERO_2 (170) // 170  A17 -

#pragma pack(pop)

typedef enum
{
    CONTROL_TRANSPORT = 0,
    CONTROL_FORWARD = 1,
	CONTROL_DEPLOYMENT = 2,
    CONTROL_LEFT = 3,
    CONTROL_RIGHT = 4

} control_state_t;

#define ORG_PACK_SIZE (27)

void setPWM_1(float angle)
{
	if (angle > 180)
	{
		angle = 180;
	}
	else if (angle < 0) {
		angle = 0;
	}
    //TIM_OC_InitTypeDef sConfigOC;

    //sConfigOC.OCMode = TIM_OCMODE_PWM1;
    //sConfigOC.Pulse = 300;
    //sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    //sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    //HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2);

	const uint16_t value_min = 500;//1450;
	const uint16_t value_max = 2500;//7350;
	const uint16_t value = (value_max - value_min) * angle / (180) + value_min;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, value);
}

void setPWM_2(float angle)
{
	if (angle > 180)
	{
		angle = 180;
	}
	else if (angle < 0) {
		angle = 0;
	}
    //TIM_OC_InitTypeDef sConfigOC;

    //sConfigOC.OCMode = TIM_OCMODE_PWM1;
    //sConfigOC.Pulse = 300;
    //sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    //sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    //HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2);

	const uint16_t value_min = 500;
	const uint16_t value_max = 2500;
	const uint16_t value = (value_max - value_min) * angle / (180) + value_min;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, value);
}

void control_transport(void)
{
    setPWM_1(ZERO_1);
    setPWM_2(ZERO_2);
}

void control_deployment(void)
{
	setPWM_1(ZERO_1 + 90);
	setPWM_2(ZERO_2);
}

void control_forward(void)
{
    setPWM_1(ZERO_1 + 135);
    setPWM_2(ZERO_2 - 45);
}

void control_left(void)
{
    setPWM_1(ZERO_1 + 145);
    setPWM_2(ZERO_2 - 10);
}

void control_right(void)
{
    setPWM_1(ZERO_1 + 85);
    setPWM_2(ZERO_2 - 55);
}

void control_set(control_state_t state)
{
    switch (state)
    {
        case CONTROL_TRANSPORT:
            control_transport();
            break;

        case CONTROL_DEPLOYMENT:
        	control_deployment();
            break;

        case CONTROL_FORWARD:
            control_forward();
            break;

        case CONTROL_LEFT:
            control_left();
            break;

        case CONTROL_RIGHT:
            control_right();
            break;

        default:
        	control_transport();
            break;
    }
}

void appmain(void)
{
	packet_t packet = {0};
	packet.start = 0xAAAA;
	packet.team_id = 0xBBBB;

	// занулил значения для целевой точки
	dataGPS_t dataGPS = {0};


	//	GPS
	neo6mv2_Init();
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_ERR);
	GPS_Data gps_data;

	//	DS18B20 - термометр

	ds18b20_init(DS18B20_12_BIT);
	uint32_t ds_stert_time = HAL_GetTick();
	ds18b20_conv();

	// BME280 - барометр/термометр

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
	e220_set_channel(&e220, 33);
	HAL_Delay(50);
	e220_set_addr(&e220, 0xAAAA);
	HAL_Delay(50);
	e220_set_reg0(&e220, E220_AIR_RATE_19P2K, E220_UART_RATE_115200, E220_SERIAL_PARITY_8N1);
	HAL_Delay(50);
	e220_set_reg1(&e220, E220_SUB_PACKET_200_BYTES, E220_RSSI_AMBIENT_NOISE_DISABLE, E220_TRANSMITTING_POWER_22DBM);
	HAL_Delay(50);
	e220_change_mode(&e220, E220_MODE_NM);
	HAL_Delay(100);

	huart2.Init.BaudRate = 115200;
	HAL_UART_Init(&huart2);

	// sd-card

	FATFS fleska;
	FIL paket_fille;
	char paker_path[] = "paket.bin";
	FRESULT rizult_mount = f_mount(&fleska, "", 1);
	FRESULT rizult_paket = 255;
	UINT byte_count;
	uint32_t time = HAL_GetTick();

	bme280_get_sensor_data(BME280_PRESS | BME280_TEMP, &bmp_data, &bmp);
	float first_pressure = bmp_data.pressure;

	operation_algoritm_t mission_status = OA_CHECK_LIGHT;
	uint8_t count_fotores = 0;

	float fotores_aver = 2;
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);

	// Собираю значения для MadgwickAHRSupdate

	float q[4] = {1, 0, 0, 0};
	uint32_t madgwick_prev_ms = HAL_GetTick();
	const float beta = 0.10;


	while (1)
	{
		packet.fotores = fotores_read_data() * 1000;

		bme280_get_sensor_data(BME280_PRESS | BME280_TEMP, &bmp_data, &bmp);
		packet.temp = bmp_data.temperature * 100;
		packet.pressure = bmp_data.pressure;
		float altitude = 44330 * (1 - pow((float)bmp_data.pressure / first_pressure, (1.0 / 5.255)));

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

		uint32_t now_ms = HAL_GetTick();
		float dt = (now_ms - madgwick_prev_ms) / 1000;
		madgwick_prev_ms = now_ms;

		if (dt <= 0.0)
		{
			dt = 0.01;
		}

		float ax = lsm6ds3_from_fs16g_to_mg(buf_lsm_xl[0]) / 1000.0;
		float ay = lsm6ds3_from_fs16g_to_mg(buf_lsm_xl[1]) / 1000.0;
		float az = lsm6ds3_from_fs16g_to_mg(buf_lsm_xl[2]) / 1000.0;

		float gx = lsm6ds3_from_fs2000dps_to_mdps(buf_lsm_gy[0]) / 1000.0 * M_PI / 180;
		float gy = lsm6ds3_from_fs2000dps_to_mdps(buf_lsm_gy[1]) / 1000.0 * M_PI / 180;
		float gz = lsm6ds3_from_fs2000dps_to_mdps(buf_lsm_gy[2]) / 1000.0 * M_PI / 180;

		float mx = lis2mdl_from_lsb_to_mgauss(buf_lis2[0]);
		float my = lis2mdl_from_lsb_to_mgauss(buf_lis2[1]);
		float mz = lis2mdl_from_lsb_to_mgauss(buf_lis2[2]);

		MadgwickAHRSupdate(q, gx, gy, gz, ax, ay, az, mx, my, mz, dt, beta);

		packet.quaternion_a = q[0];
		packet.quaternion_b = q[1];
		packet.quaternion_c = q[2];
		packet.quaternion_d = q[3];

		gps_data = neo6mv2_GetData();
		packet.latitude_gps = gps_data.latitude;
		packet.longitude_gps = gps_data.longitude;
		packet.altitude_gps = gps_data.altitude;
		packet.fix_gps = gps_data.fixQuality;

		if (HAL_GetTick() - ds_stert_time >= 750)
		{
			packet.ds18b20 = ds18b20_read_temp();
			ds_stert_time = HAL_GetTick();
			ds18b20_conv();
		}

		switch (mission_status)
		{
			case OA_CHECK_LIGHT:
				if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_RESET)
				{

					// собираю освещенность
					count_fotores = 0;
					fotores_aver = 0;
					for (int i = 0; i < 10; i++)
					{
						fotores_aver += fotores_read_data();
						count_fotores++;
					}
					fotores_aver = fotores_aver / count_fotores;
					mission_status = OA_PREPARATION;
					time = HAL_GetTick();

					// Собираю целевую точку

//					neo6mv2_Init();
//					__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
//					__HAL_UART_ENABLE_IT(&huart1, UART_IT_ERR);
//					GPS_Data gps_data;
//
//					gps_data = neo6mv2_GetData();
//					dataGPS.latitude_target_gps = gps_data.latitude;
//					dataGPS.longitude_target_gps = gps_data.longitude;
//					dataGPS.altitude_target_gps = gps_data.altitude;

					dataGPS.latitude_target_gps =  packet.latitude_gps;
					dataGPS.longitude_target_gps = packet.longitude_gps;
					dataGPS.altitude_target_gps = packet.altitude_gps;
				}
				break;
			case OA_PREPARATION:
				if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_SET)
				{
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
					if (HAL_GetTick() - time >= 15000)
					{
						HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
						mission_status = OA_ROCKET;
						time = HAL_GetTick();
					}
				}
				else
				{
					time = HAL_GetTick();
				}
				break;

			case OA_ROCKET:
				if (fotores_read_data() > fotores_aver)
				{
					mission_status = OA_FIFRE;
					time = HAL_GetTick();

				}
				break;
			case OA_FIFRE:
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET); // Криматорий ON
				if (HAL_GetTick() - time >= 3000)
				{
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); // Криматорий OFF
					mission_status = OA_CONTROL;
					time = HAL_GetTick();
				}
				break;
			case OA_CONTROL:

			    if (packet.fix_gps)
			    {
			        rectangular_system_data_t target_vector;

			        target_vector = math(dataGPS.latitude_target_gps, dataGPS.longitude_target_gps, dataGPS.altitude_target_gps, packet.latitude_gps, packet.longitude_gps, packet.altitude_gps);

			        float alpha = atan2f(target_vector.Y, target_vector.X) * 180 / M_PI;

			        if (alpha > 10)
			        {
			            control_left();
			        }
			        else if (alpha < -10)
			        {
			            control_right();
			        }
			        else
			        {
			            control_forward();
			        }
			    }

			    if (altitude < 10)
			    {
			        mission_status = OA_DECLINE;
			    }

			    break;


			case OA_DECLINE:
				if (altitude < 10)
				{
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); // Пищалка ON
				}
				break;
		}


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
			if (rizult_paket != FR_OK)
				rizult_mount = 255;
		}
		if (rizult_paket == FR_OK && rizult_mount == FR_OK)
		{
			rizult_paket = f_write(&paket_fille, &packet, sizeof(packet_t), &byte_count);
			f_sync(&paket_fille);
		}
	}
	return;
}
