#include "modbus_crc16.h"

static unsigned short       crc_tab16[256];

void MBusCRC16Init() {

	unsigned short i;
	unsigned short j;
	unsigned short crc;
	unsigned short c;

	for (i = 0; i < 256; i++) {

		crc = 0;
		c = i;

		for (j = 0; j < 8; j++) {

			if ((crc ^ c) & 0x0001) crc = (crc >> 1) ^ MODBUS_CRC_POLY_16;
			else                      crc = crc >> 1;

			c = c >> 1;
		}

		crc_tab16[i] = crc;
	}
}

unsigned char	MBusCRC16(unsigned short crcVal,
	unsigned char* data,
	unsigned char dataLength)
{
	for (int i = 0; i < dataLength; i++)
	{
		crcVal = (crcVal >> 8) ^ crc_tab16[(crcVal ^ (unsigned short)data[i]) & 0x00FF];
	}

	return crcVal;
}
