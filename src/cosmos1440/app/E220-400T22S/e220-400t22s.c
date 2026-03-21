/*
 * E220-400T22S.c
 *
 *  Created on: Mar 20, 2026
 *      Author: ChatDeepseekGPTCapilot
 */

#include "e220-400t22s.h"

void e220_change_mode(e220_bus_t *bus, e220_mode_t mode)
{
	switch (mode)
	{
	    case E220_MODE_NM:
	    	HAL_GPIO_WritePin(bus->m1_port, bus->m1_pin, GPIO_PIN_RESET);
	    	HAL_GPIO_WritePin(bus->m0_port, bus->m0_pin, GPIO_PIN_RESET);
			break;
	    case E220_MODE_WOR_SM:
	    	HAL_GPIO_WritePin(bus->m0_port, bus->m0_pin, GPIO_PIN_RESET);
	    	HAL_GPIO_WritePin(bus->m1_port, bus->m1_pin, GPIO_PIN_SET);
	    	break;
	    case E220_MODE_WOR_RM:
			HAL_GPIO_WritePin(bus->m0_port, bus->m0_pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(bus->m1_port, bus->m1_pin, GPIO_PIN_RESET);
			break;
	    case E220_MODE_SM:
			HAL_GPIO_WritePin(bus->m0_port, bus->m0_pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(bus->m1_port, bus->m1_pin, GPIO_PIN_SET);
			break;
	}
}

void e220_write_reg(e220_bus_t *bus, uint8_t addr, uint8_t *byte, uint8_t len)
{
	if (len > 9)
		len = 9;
	uint8_t buf[12] = {0xC0, addr, len, 0};
	for (int i = 0; i < len; i++)
	{
		buf[i + 3] = byte[i];
	}

	HAL_UART_Transmit(bus->huart, buf, len + 3, 100);
}

void e220_set_channel(e220_bus_t *bus, uint8_t ch)
{
	if (ch > 83)
		return

    e220_write_reg(bus, 0x04, &ch, 1);
}

void e220_set_addr(e220_bus_t *bus, uint16_t addr)
{
	e220_write_reg(bus, 0x2, (uint8_t *)&addr, 2);
}

void e220_set_air_rate(e220_bus_t *bus, e220_air_rate_t rate)
{
	uint8_t data = 0;
	data = data | rate;
	e220_write_reg(bus, 0x02, &data, 1);
}

void e220_set_reg0(e220_bus_t *bus, uint8_t uart_rate, uint8_t parity, e220_air_rate_t air_rate)
{
	uint8_t data = 0;
	data = data | air_rate;
	data = data | (parity << 3);
	data = data | (uart_rate << 5);
	e220_write_reg(bus, 0x02, &data, 1);
}

void e220_set_reg1(e220_bus_t *bus, e220_sub_packet_t sub_packet, e220_rssi_ambient_noise_t rssi_ambient_noise, e220_transmitting_power_t power)
{
	uint8_t data = 0;
	data = data | power;
	data = data | (rssi_ambient_noise << 5);
	data = data | (sub_packet << 6);
	e220_write_reg(bus, 0x03, &data, 1);
}
