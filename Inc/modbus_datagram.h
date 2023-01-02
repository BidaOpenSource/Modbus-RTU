#ifndef MODBUS_DATAGRAM_H
#define MODBUS_DATAGRAM_H

#define MBUS_MIN_PACKET_LENGTH				5
#define MBUS_MAX_PACKET_LENGTH				256
#define MBUS_MAX_PDU_HEADER_LENGTH			5
#define MBUS_MAX_PDU_LENGTH					253
#define MBUS_CRC_LENGTH						2

#define MBUS_MAX_READ_DISCRETE_LENGTH		2048
#define MBUS_MAX_READ_REGISTERS_LENGTH		125
#define MBUS_MAX_WRITE_M_DATA_LENGTH		247

typedef enum
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
MBusFunction;

typedef struct
{
	MBusFunction	FunctionCode;
	unsigned char	DataHeader[MBUS_MAX_PDU_HEADER_LENGTH];
	unsigned char	DataHeaderLength;
	unsigned char*	Data;
	unsigned char	DataLength;
}
MBusPDU;

typedef struct
{
	unsigned char	SlaveAddress;
	MBusPDU			PDU;
	unsigned char	CRC16[MBUS_CRC_LENGTH];
}
MBusADU;


void	MBusSetSlaveAddr(MBusADU* ADU, unsigned char slaveAddr);
void	MBusSetQueryFunction(MBusADU* ADU, MBusFunction fnc);

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

void	MBusDatagramParseResponse(	MBusADU* ADU,
									unsigned char* data,
									unsigned char		dataLength);
#endif

