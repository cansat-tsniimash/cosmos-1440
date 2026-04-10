/*
 * bitwise_XOR.c
 *
 *  Created on: Apr 10, 2026
 *      Author: ChatDeepseekGPTCapilot
 */

#include <stdint.h>

uint8_t xor_summ(uint8_t *data, uint8_t len)
{
	uint8_t summ = 0;

	for (int i = 0; i < len; i++)
	{
		summ = summ ^ data[i];
	}

	return summ;
}
