/*
 * E220-400T22S.h
 *
 *  Created on: Mar 20, 2026
 *      Author: ChatDeepseekGPTCapilot
 */

#ifndef E220_400T22S_E220_400T22S_H_
#define E220_400T22S_E220_400T22S_H_

#include "stm32f1xx.h"

typedef struct {
	UART_HandleTypeDef *huart;
	GPIO_TypeDef *m0_port;
	uint16_t m0_pin;
	GPIO_TypeDef *m1_port;
	uint16_t m1_pin;
	GPIO_TypeDef *aux_port;
	uint16_t aux_pin;
} e220_bus_t;

typedef enum {
	E220_MODE_NM = 0,
	E220_MODE_WOR_SM = 1,
	E220_MODE_WOR_RM = 2,
	E220_MODE_SM = 3,
} e220_mode_t;

typedef enum {
	E220_UART_RATE_1200 = 0,
	E220_UART_RATE_2400 = 1,
	E220_UART_RATE_4800 = 2,
	E220_UART_RATE_9600 = 3,
	E220_UART_RATE_19200 = 4,
	E220_UART_RATE_38400 = 5,
	E220_UART_RATE_57600 = 6,
	E220_UART_RATE_115200 = 7,
} e220_uart_rate_t;

typedef enum {
	E220_SERIAL_PARITY_8N1 = 0,
	E220_SERIAL_PARITY_8O1 = 1,
	E220_SERIAL_PARITY_8E1 = 2,
	E220_SERIAL_PARITY_8N1_2 = 3,
} e220_serial_parity_t;

typedef enum {
	E220_AIR_RATE_2P4K_0 = 0,
	E220_AIR_RATE_2P4K_1 = 1,
	E220_AIR_RATE_2P4K = 2,
	E220_AIR_RATE_4P8K = 3,
	E220_AIR_RATE_9P6K = 4,
	E220_AIR_RATE_19P2K = 5,
	E220_AIR_RATE_38P4K = 6,
	E220_AIR_RATE_62P5K = 7,
} e220_air_rate_t;

typedef enum {
	E220_SUB_PACKET_200_BYTES = 0,
	E220_SUB_PACKET_128_BYTES = 1,
	E220_SUB_PACKET_64_BYTES = 2,
	E220_SUB_PACKET_32_BYTES = 3,
} e220_sub_packet_t;

typedef enum {
	E220_RSSI_AMBIENT_NOISE_DISABLE = 0,
	E220_RSSI,AMBIENT_NOISE_ENABLE = 1,
} e220_rssi_ambient_noise_t;

typedef enum {
	E220_TRANSMITTING_POWER_22DBM = 0,
	E220_TRANSMITTING_POWER_17DBM = 1,
	E220_TRANSMITTING_POWER_13DBM = 2,
	E220_TRANSMITTING_POWER_10DBM = 3,
} e220_transmitting_power_t;

#endif /* E220_400T22S_E220_400T22S_H_ */
