#ifndef MODBUS_CRC16_H
#define MODBUS_CRC16_H

#define			MODBUS_CRC_START	0xFFFF
#define			MODBUS_CRC_POLY_16	0xA001

//void			MBusCRC16Init();
unsigned char	MBusCRC16(unsigned short crcVal, unsigned char* data, unsigned char dataLength);

#endif
