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

unsigned char	MBusDatagramHeaderAppend(	MBusADU* ADU,
											unsigned char byte,
											unsigned char byteIndex)
{
	switch (ADU->PDU.FunctionCode)
	{
	case READ_COILS:
	case READ_DISCRETE_INPUTS:
	case READ_INPUTS:
	case READ_HOLDING_REGISTERS:
		ADU->PDU.DataHeaderLength = 1;
		ADU->PDU.DataHeader[byteIndex] = byte;
		return 1;
	case WRITE_SINGLE_COIL:
	case WRITE_SINGLE_HOLDING_REGISTER:
	case WRITE_MULTIPLE_COILS:
	case WRITE_MULTIPLE_HOLDING_REGISTERS:
		ADU->PDU.DataHeaderLength = 4;

		ADU->PDU.DataHeader[byteIndex] = byte;

		if (ADU->PDU.DataHeaderLength == byteIndex + 1) return 1;
		return 0;
	default:
		return 1;
	}
}
unsigned char	MBusDatagramDataAppend(		MBusADU* ADU,
											unsigned char byte,
											unsigned int byteIndex)
{
	ADU->PDU.DataLength = byteIndex + 1;
	ADU->PDU.Data[byteIndex] = byte;

	return byteIndex >= MBUS_MAX_PDU_DATA_LENGTH;
}
unsigned char	MBusDatagramCRCAppend(		MBusADU* ADU,
											unsigned char byte,
											unsigned int byteIndex)
{
	ADU->CRC16[byteIndex] = byte;
	return byteIndex >= MBUS_CRC_LENGTH;
}
