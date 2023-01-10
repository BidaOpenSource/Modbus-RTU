#include "modbus_global.h"
#include "modbus_datagram.h"

#include "modbus_crc16.h"

typedef enum
{
	ADDR_SLAVE = 0x00,
	PDU_FNC_CODE = 0x01,
	PDU_DATA_START = 0x02
}
MBusADUDataOrder;

typedef enum
{
	ADDR_H = 0x00,
	ADDR_L = 0x01,

	FREAD_REG_N_H = 0x02,
	FREAD_REG_N_L = 0x03,
	FREAD_RESP_BYTES_N = 0x00,

	FWRITE_VAL_H = 0x02,
	FWRITE_VAL_L = 0x03,
	FWRITE_RESP_VAL_H = 0x02,
	FWRITE_RESP_VAL_L = 0x03,

	FWRITE_M_REG_N_H = 0x02,
	FWRITE_M_REG_N_L = 0x03,
	FWRITE_M_BYTES_N = 0x04,
	FWRITE_M_RESP_REG_N_H = 0x02,
	FWRITE_M_RESP_REG_N_L = 0x03,

	CRC16_H = 0x00,
	CRC16_L = 0x01
}
MBusPDUDataOrder;

unsigned short MBusPDUGetRegStartAddr(MBusPDU* pdu)
{
	return (pdu->DataHeader[ADDR_H] << MBUS_BITS_IN_BYTE) | pdu->DataHeader[ADDR_L];
}
unsigned short MBusPDUGetExpectedRegCount(MBusPDU* pdu)
{
	return (pdu->DataHeader[FREAD_REG_N_H] << MBUS_BITS_IN_BYTE) | pdu->DataHeader[FREAD_REG_N_L];
}
unsigned short MBusPDUGetRegExpectedValue(MBusPDU* pdu)
{
	return (pdu->DataHeader[FWRITE_VAL_H] << MBUS_BITS_IN_BYTE) | pdu->DataHeader[FWRITE_VAL_L];
}

void	MBusSetSlaveAddr(MBusADU* ADU, unsigned char slaveAddr)
{
	ADU->SlaveAddress = slaveAddr;
}

void	MBusSetQueryFunction(MBusADU* ADU, MBusFunction fnc)
{
	ADU->PDU.FunctionCode = fnc;
}

void	MBusQueryClear(MBusADU* ADU)
{
	ADU->PDU.DataHeaderLength = 0;
	ADU->PDU.DataLength = 0;

	ADU->CRC16[CRC16_H] = 0;
	ADU->CRC16[CRC16_L] = 0;
}

void	MBusQueryRead(MBusADU* ADU, unsigned short	regAddrStart,
	unsigned short	regN)
{
	ADU->PDU.DataHeader[ADDR_H] = (regAddrStart & MBUS_SENIOR_BIT_MASK)
		>> MBUS_SENIOR_BIT_SHIFT;
	ADU->PDU.DataHeader[ADDR_L] = (regAddrStart & MBUS_JUNIOR_BIT_MASK)
		>> MBUS_JUNIOR_BIT_SHIFT;

	ADU->PDU.DataHeader[FREAD_REG_N_H] = (regN & MBUS_SENIOR_BIT_MASK)
		>> MBUS_SENIOR_BIT_SHIFT;
	ADU->PDU.DataHeader[FREAD_REG_N_L] = (regN & MBUS_JUNIOR_BIT_MASK)
		>> MBUS_JUNIOR_BIT_SHIFT;

	ADU->PDU.DataHeaderLength = 4;


	unsigned short crc16 = MBusCRC16(MODBUS_CRC_START, ADU->PDU.DataHeader,
		ADU->PDU.DataHeaderLength);

	ADU->CRC16[CRC16_H] = (crc16 & MBUS_SENIOR_BIT_MASK)
		>> MBUS_SENIOR_BIT_SHIFT;
	ADU->CRC16[CRC16_L] = (crc16 & MBUS_JUNIOR_BIT_MASK)
		>> MBUS_JUNIOR_BIT_SHIFT;
}
void	MBusResponseRead(MBusADU* ADU, unsigned char* regValsPacked,
	unsigned char	bytesN)
{
	ADU->PDU.DataHeader[FREAD_RESP_BYTES_N] = bytesN;

	ADU->PDU.DataHeaderLength = 1;

	ADU->PDU.Data = regValsPacked;
	ADU->PDU.DataLength = bytesN;

	unsigned short crc16;

	crc16 = MBusCRC16(MODBUS_CRC_START, ADU->PDU.DataHeader,
		ADU->PDU.DataHeaderLength);

	crc16 = MBusCRC16(crc16, ADU->PDU.Data,
		ADU->PDU.DataLength);

	ADU->CRC16[CRC16_H] = (crc16 & MBUS_SENIOR_BIT_MASK)
		>> MBUS_SENIOR_BIT_SHIFT;
	ADU->CRC16[CRC16_L] = (crc16 & MBUS_JUNIOR_BIT_MASK)
		>> MBUS_JUNIOR_BIT_SHIFT;
}

void	MBusQueryWrite(MBusADU* ADU, unsigned short	regAddr,
	unsigned short	regVal)
{
	ADU->PDU.DataHeaderLength = 4;

	ADU->PDU.DataHeader[ADDR_H] = (regAddr & MBUS_SENIOR_BIT_MASK)
		>> MBUS_SENIOR_BIT_SHIFT;
	ADU->PDU.DataHeader[ADDR_L] = (regAddr & MBUS_JUNIOR_BIT_MASK)
		>> MBUS_JUNIOR_BIT_SHIFT;

	ADU->PDU.DataHeader[FWRITE_VAL_H] = (regVal & MBUS_SENIOR_BIT_MASK)
		>> MBUS_SENIOR_BIT_SHIFT;
	ADU->PDU.DataHeader[FWRITE_VAL_L] = (regVal & MBUS_JUNIOR_BIT_MASK)
		>> MBUS_JUNIOR_BIT_SHIFT;

	unsigned short crc16 = MBusCRC16(MODBUS_CRC_START, ADU->PDU.DataHeader,
		ADU->PDU.DataHeaderLength);

	ADU->CRC16[CRC16_H] = (crc16 & MBUS_SENIOR_BIT_MASK)
		>> MBUS_SENIOR_BIT_SHIFT;
	ADU->CRC16[CRC16_L] = (crc16 & MBUS_JUNIOR_BIT_MASK)
		>> MBUS_JUNIOR_BIT_SHIFT;
}
void	MBusResponseWrite(MBusADU* ADU, unsigned short	regAddr,
	unsigned short	regVal)
{
	return MBusQueryWrite(ADU, regAddr, regVal);
}

void	MBusQueryWriteMultiple(MBusADU* ADU, unsigned short	regAddrStart,
	unsigned char* regValsPacked,
	unsigned char	regValsN,
	unsigned char	bytesN)
{

	ADU->PDU.DataHeader[ADDR_H] = (regAddrStart & MBUS_SENIOR_BIT_MASK)
		>> MBUS_SENIOR_BIT_SHIFT;
	ADU->PDU.DataHeader[ADDR_L] = (regAddrStart & MBUS_JUNIOR_BIT_MASK)
		>> MBUS_JUNIOR_BIT_SHIFT;

	ADU->PDU.DataHeader[FWRITE_M_REG_N_H] = (regValsN & MBUS_SENIOR_BIT_MASK)
		>> MBUS_SENIOR_BIT_SHIFT;
	ADU->PDU.DataHeader[FWRITE_M_REG_N_L] = (regValsN & MBUS_JUNIOR_BIT_MASK)
		>> MBUS_JUNIOR_BIT_SHIFT;

	ADU->PDU.DataHeader[FWRITE_M_BYTES_N] = bytesN;

	ADU->PDU.DataHeaderLength = 5;

	ADU->PDU.Data = regValsPacked;
	ADU->PDU.DataLength = bytesN;

	unsigned short crc16;

	crc16 = MBusCRC16(0, ADU->PDU.DataHeader,
		ADU->PDU.DataHeaderLength);

	crc16 = MBusCRC16(crc16, ADU->PDU.Data,
		ADU->PDU.DataLength);

	ADU->CRC16[CRC16_H] = (crc16 & MBUS_SENIOR_BIT_MASK)
		>> MBUS_SENIOR_BIT_SHIFT;
	ADU->CRC16[CRC16_L] = (crc16 & MBUS_JUNIOR_BIT_MASK)
		>> MBUS_JUNIOR_BIT_SHIFT;
}

static unsigned char MBusDatagramParseResponseHeader(MBusADU* ADU,
	MBusFunction fnc,
	unsigned char* headerData)
{
	ADU->PDU.DataHeaderLength = 0;

	if (fnc == READ_COILS ||
		fnc == READ_DISCRETE_INPUTS ||
		fnc == READ_HOLDING_REGISTERS ||
		fnc == READ_INPUTS)
	{
		ADU->PDU.DataHeaderLength = 1;
		ADU->PDU.DataHeader[FREAD_RESP_BYTES_N] = headerData[FREAD_RESP_BYTES_N];
	}

	if (fnc == WRITE_SINGLE_COIL ||
		fnc == WRITE_SINGLE_HOLDING_REGISTER ||
		fnc == WRITE_MULTIPLE_COILS ||
		fnc == WRITE_MULTIPLE_HOLDING_REGISTERS)
	{
		ADU->PDU.DataHeaderLength = 4;
		ADU->PDU.DataHeader[ADDR_H] = headerData[ADDR_H];
		ADU->PDU.DataHeader[ADDR_L] = headerData[ADDR_L];
		ADU->PDU.DataHeader[FWRITE_RESP_VAL_H] = headerData[FWRITE_RESP_VAL_H];
		ADU->PDU.DataHeader[FWRITE_RESP_VAL_L] = headerData[FWRITE_RESP_VAL_L];
	}

	return ADU->PDU.DataHeaderLength;
}

void	MBusDatagramParseResponse(MBusADU* ADU,
	unsigned char*		data,
	unsigned char		dataLength)
{
	if (dataLength < MBUS_MIN_PACKET_LENGTH) return;

	ADU->SlaveAddress = data[ADDR_SLAVE];
	ADU->PDU.FunctionCode = data[PDU_FNC_CODE];

	unsigned char headerLength = MBusDatagramParseResponseHeader(ADU,
		ADU->PDU.FunctionCode,
		&data[PDU_DATA_START]);

	ADU->PDU.DataLength = 0;
	if (ADU->PDU.FunctionCode == READ_COILS ||
		ADU->PDU.FunctionCode == READ_DISCRETE_INPUTS ||
		ADU->PDU.FunctionCode == READ_HOLDING_REGISTERS ||
		ADU->PDU.FunctionCode == READ_INPUTS)
	{
		ADU->PDU.DataLength = dataLength - ADU->PDU.DataHeaderLength - MBUS_CRC_LENGTH;
		ADU->PDU.Data = &data[PDU_DATA_START + headerLength];
	}

	ADU->CRC16[CRC16_H] = (data[dataLength - MBUS_CRC_LENGTH + CRC16_H] << MBUS_SENIOR_BIT_SHIFT);
	ADU->CRC16[CRC16_L] = (data[dataLength - MBUS_CRC_LENGTH + CRC16_L] << MBUS_JUNIOR_BIT_SHIFT);
}

static unsigned char MBusDatagramParseQueryHeader(MBusADU* ADU,
	MBusFunction fnc,
	unsigned char* headerData)
{
	ADU->PDU.DataHeaderLength = 0;

	if (fnc == READ_COILS ||
		fnc == READ_DISCRETE_INPUTS ||
		fnc == READ_HOLDING_REGISTERS ||
		fnc == READ_INPUTS)
	{
		ADU->PDU.DataHeaderLength = 4;
		ADU->PDU.DataHeader[ADDR_H] = headerData[ADDR_H];
		ADU->PDU.DataHeader[ADDR_L] = headerData[ADDR_L];
		ADU->PDU.DataHeader[FREAD_REG_N_H] = headerData[FREAD_REG_N_H];
		ADU->PDU.DataHeader[FREAD_REG_N_L] = headerData[FREAD_REG_N_L];
	}

	if (fnc == WRITE_SINGLE_COIL ||
		fnc == WRITE_SINGLE_HOLDING_REGISTER)
	{
		ADU->PDU.DataHeaderLength = 4;
		ADU->PDU.DataHeader[ADDR_H] = headerData[ADDR_H];
		ADU->PDU.DataHeader[ADDR_L] = headerData[ADDR_L];
		ADU->PDU.DataHeader[FWRITE_VAL_H] = headerData[FWRITE_VAL_H];
		ADU->PDU.DataHeader[FWRITE_VAL_L] = headerData[FWRITE_VAL_L];
	}

	if (fnc == WRITE_MULTIPLE_COILS ||
		fnc == WRITE_MULTIPLE_HOLDING_REGISTERS)
	{
		ADU->PDU.DataHeaderLength = 5;
		ADU->PDU.DataHeader[ADDR_H] = headerData[ADDR_H];
		ADU->PDU.DataHeader[ADDR_L] = headerData[ADDR_L];
		ADU->PDU.DataHeader[FWRITE_M_REG_N_H] = headerData[FWRITE_M_REG_N_H];
		ADU->PDU.DataHeader[FWRITE_M_REG_N_L] = headerData[FWRITE_M_REG_N_L];
		ADU->PDU.DataHeader[FWRITE_M_BYTES_N] = headerData[FWRITE_M_BYTES_N];
	}

	return ADU->PDU.DataHeaderLength;
}
void	MBusDatagramParseQuery(MBusADU* ADU,
	unsigned char* data,
	unsigned char		dataLength)
{
	if (dataLength < MBUS_MIN_PACKET_LENGTH) return;

	ADU->SlaveAddress = data[ADDR_SLAVE];
	ADU->PDU.FunctionCode = data[PDU_FNC_CODE];

	unsigned char headerLength = MBusDatagramParseQueryHeader(ADU,
		ADU->PDU.FunctionCode,
		&data[PDU_DATA_START]);

	ADU->PDU.DataLength = 0;
	if (ADU->PDU.FunctionCode == WRITE_MULTIPLE_COILS ||
		ADU->PDU.FunctionCode == WRITE_MULTIPLE_HOLDING_REGISTERS)
	{
		ADU->PDU.DataLength = dataLength - ADU->PDU.DataHeaderLength - MBUS_CRC_LENGTH;
		ADU->PDU.Data = &data[PDU_DATA_START + headerLength];
	}

	ADU->CRC16[CRC16_H] = (data[dataLength - MBUS_CRC_LENGTH + CRC16_H] << MBUS_SENIOR_BIT_SHIFT);
	ADU->CRC16[CRC16_L] = (data[dataLength - MBUS_CRC_LENGTH + CRC16_L] << MBUS_JUNIOR_BIT_SHIFT);
}