/*
 * crc8.c
 *
 *  Created on: 22-Oct-2018
 *      Author: MaNoJ KuMaR
 */

#include "contiki.h"
#include <string.h>

uint8_t crc8_cal(uint8_t *data, uint16_t length) {
	uint8_t crc = 0x00;

	uint8_t extract;
	uint8_t sum;


	for (uint16_t i = 0; i < length; i++) {
		extract = *data;
		for (uint8_t tempI = 8; tempI; tempI--) {
			sum = (crc ^ extract) & 0x01;
			crc >>= 1;
			if (sum)
				crc ^= 0x8C;
			extract >>= 1;
		}
		data++;
	}

	return crc;
}
