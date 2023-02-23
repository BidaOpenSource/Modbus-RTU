#ifndef MODBUS_DATAGRAM_H
#define MODBUS_DATAGRAM_H

#include "modbus_global.h"

typedef struct
{
	MBusFunctionType*	FunctionCode;

	MBusException*		ExceptionCode;

	unsigned char*		Data;
	unsigned char		DataLength;
}
MBusPDU;

typedef struct
{
	unsigned char		Data[255];
	unsigned char		DataLength;

	unsigned char*		SlaveAddress;
	MBusPDU				PDU;
	unsigned char*		CRC16;
}
MBusADU;

#define		MBusGetDatagramAddress(buffer)	(buffer[0])
#define		MBusGetDatagramFunction(buffer)	(buffer[1])

MBusException MBusMapCharBuffer(MBusADU* adu, unsigned char* buffer, unsigned char bufferLength);

MBusException MBusMapCharBufferHeader(MBusADU* adu);
MBusException MBusMapCharBufferFooter(MBusADU* adu, unsigned short crc16);

#endif
