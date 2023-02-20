#ifndef MODBUS_DATAGRAM_H
#define MODBUS_DATAGRAM_H

#include "modbus_global.h"

typedef struct
{
	MBusFunctionType*	FunctionCode;
	unsigned char*		Data;
	unsigned char		DataLength;
}
MBusPDU;

typedef struct
{
	unsigned char*		SlaveAddress;
	MBusPDU				PDU;
	unsigned char*		CRC16;
}
MBusADU;

MBusException MBusMapCharBuffer(MBusADU* adu, unsigned char* buffer, unsigned char bufferLength);

#endif

