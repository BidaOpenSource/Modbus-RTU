#include "modbus_global.h"
#include "modbus_datagram.h"

MBusException MBusMapCharBuffer(MBusADU* adu, unsigned char* buffer, unsigned char bufferLength)
{
	adu->SlaveAddress = &buffer[0];
	adu->CRC16 = &buffer[bufferLength - 2];

	adu->PDU.FunctionCode = &buffer[1];
	adu->PDU.Data = &buffer[2];
	adu->PDU.DataLength = bufferLength - 2 - 2;

	return MBUS_EXC_NONE;
}
