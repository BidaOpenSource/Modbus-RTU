#include "modbus_global.h"
#include "modbus_datagram.h"

MBusException MBusMapCharBuffer(MBusADU* adu, unsigned char* buffer, unsigned char bufferLength)
{
	adu->SlaveAddress =		&buffer[0];
	adu->PDU.FunctionCode = &buffer[1];

	adu->CRC16 =			&buffer[bufferLength - 2];

	if (*(adu->PDU.FunctionCode) < 127)
	{
		adu->PDU.ExceptionCode = 0;
	}
	else
	{
		adu->PDU.ExceptionCode = &buffer[2];
		return *(adu->PDU.ExceptionCode);
	}

	adu->PDU.Data = &(buffer[2]);
	adu->PDU.DataLength = bufferLength - 2 - 2;

	return MBUS_EXC_NONE;
}

MBusException MBusMapCharBufferHeader(MBusADU* adu)
{
	adu->SlaveAddress =		&(adu->Data[0]);
	adu->PDU.FunctionCode = &(adu->Data[1]);

	adu->PDU.Data =			&(adu->Data[2]);

	adu->DataLength = 2;

	return MBUS_EXC_NONE;
}

MBusException MBusMapCharBufferFooter(MBusADU* adu, unsigned short crc16)
{
	adu->DataLength += adu->PDU.DataLength;

	adu->CRC16 = &(adu->Data[(adu->DataLength)]);

	adu->CRC16[MBUS_JUNIOR_BIT_INDEX] = (crc16 >> MBUS_JUNIOR_BIT_SHIFT) & 0xFF;
	adu->CRC16[MBUS_SENIOR_BIT_INDEX] = (crc16 >> MBUS_SENIOR_BIT_SHIFT) & 0xFF;

	adu->DataLength += 2;

	return MBUS_EXC_NONE;
}
