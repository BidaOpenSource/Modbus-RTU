#ifndef MODBUS_DATAGRAM_H
#define MODBUS_DATAGRAM_H

#define MBUS_MIN_PACKET_LENGTH				5
#define MBUS_MAX_PACKET_LENGTH				256
#define MBUS_MAX_PDU_HEADER_LENGTH			5
#define MBUS_MAX_PDU_LENGTH					253
#define MBUS_MAX_PDU_DATA_LENGTH			248
#define MBUS_CRC_LENGTH						2

#define MBUS_MAX_READ_DISCRETE_LENGTH		2048
#define MBUS_MAX_READ_REGISTERS_LENGTH		125
#define MBUS_MAX_WRITE_M_DATA_LENGTH		247

/*typedef enum
{
	READ_COILS = 0x01,
	READ_DISCRETE_INPUTS,
	READ_HOLDING_REGISTERS,
	READ_INPUTS,
	WRITE_SINGLE_COIL,
	WRITE_SINGLE_HOLDING_REGISTER,
	WRITE_MULTIPLE_COILS = 0x0F,
	WRITE_MULTIPLE_HOLDING_REGISTERS
}
MBusFunction;*/

#include "modbus_fnc.h"

typedef struct
{
	MBusFunctionType	FunctionCode;
	unsigned char*		Data;
	unsigned char		DataLength;
}
MBusPDU;

unsigned short MBusPDUGetRegStartAddr(MBusPDU* pdu);
unsigned short MBusPDUGetExpectedRegCount(MBusPDU* pdu);
unsigned short MBusPDUGetRegExpectedValue(MBusPDU* pdu);

typedef struct
{
	unsigned char	SlaveAddress;
	MBusPDU			PDU;
	unsigned char	CRC16[MBUS_CRC_LENGTH];
}
MBusADU;


void	MBusSetSlaveAddr(MBusADU* ADU, unsigned char slaveAddr);
void	MBusSetQueryFunction(MBusADU* ADU, MBusFunctionType fnc);

//		REGISTERS READ

void	MBusSetSlaveAddr(MBusADU* ADU, unsigned char slaveAddr);

void	MBusQueryClear(MBusADU* ADU);

void	MBusQueryRead(MBusADU* ADU, unsigned short	regAddrStart,
	unsigned short	regN);

void	MBusResponseRead(MBusADU* ADU, unsigned char* regValsPacked,
	unsigned char	bytesN);

void	MBusQueryWrite(MBusADU* ADU, unsigned short	regAddr,
	unsigned short	regVal);

void	MBusResponseWrite(MBusADU* ADU, unsigned short	regAddr,
	unsigned short	regVal);

void	MBusQueryWriteMultiple(MBusADU* ADU, unsigned short	regAddrStart,
	unsigned char* regValsPacked,
	unsigned char	regValsN,
	unsigned char	bytesN);

//		PARSE

unsigned char	MBusDatagramHeaderAppend(	MBusADU* ADU,
											unsigned char byte,
											unsigned char byteIndex);
unsigned char	MBusDatagramDataAppend(		MBusADU* ADU,
											unsigned char byte,
											unsigned int byteIndex);
unsigned char	MBusDatagramCRCAppend(		MBusADU* ADU,
											unsigned char byte,
											unsigned int byteIndex);

#endif

