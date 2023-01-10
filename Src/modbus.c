#include "modbus.h"
#include "modbus_global.h"
#include "modbus_datagram.h"
#include "modbus_registers.h"

static void	 MBusStartTx(MBusMasterChanel* mbus) { MBusMasterTxBytesCompleted(mbus); }

unsigned int MBusMasterIsReady(MBusMasterChanel* mbus)
{
	return mbus->Status == MBUS_CH_READY ? 1 : 0;
}

unsigned int MBusCoilsRead(MBusMasterChanel* mbus,
	unsigned char		slaveAddr,
	unsigned short		startAddr,
	unsigned char		coilsCount,
	MBusRegister**		responseHolder)
{
	if (!MBusMasterIsReady(mbus)) return 0;

	mbus->Status = MBUS_CH_BUSY;

	MBusSetSlaveAddr(&mbus->DatagramQuery, slaveAddr);

	MBusQueryClear(&mbus->DatagramQuery);

	MBusSetQueryFunction(&mbus->DatagramQuery, READ_COILS);
	MBusQueryRead(&mbus->DatagramQuery, startAddr, coilsCount);

	mbus->ptrResponseStruct = responseHolder;

	mbus->Status = MBUS_CH_TX_READY;
	MBusStartTx(mbus);

	return 1;
}

unsigned int MBusDiscreteInputsRead(MBusMasterChanel* mbus,
	unsigned char		slaveAddr,
	unsigned short		startAddr,
	unsigned char		inputsCount,
	MBusRegister**		responseHolder)
{
	if (!MBusMasterIsReady(mbus)) return 0;

	mbus->Status = MBUS_CH_BUSY;

	MBusSetSlaveAddr(&mbus->DatagramQuery, slaveAddr);

	MBusQueryClear(&mbus->DatagramQuery);

	MBusSetQueryFunction(&mbus->DatagramQuery, READ_DISCRETE_INPUTS);
	MBusQueryRead(&mbus->DatagramQuery, startAddr, inputsCount);

	mbus->ptrResponseStruct = responseHolder;

	mbus->Status = MBUS_CH_TX_READY;
	MBusStartTx(mbus);

	return 1;
}
unsigned int MBusHoldingRegistersRead(MBusMasterChanel* mbus,
	unsigned char		slaveAddr,
	unsigned short		startAddr,
	unsigned char		regCount,
	MBusRegister**		responseHolder)
{
	if (!MBusMasterIsReady(mbus)) return 0;

	mbus->Status = MBUS_CH_BUSY;

	MBusSetSlaveAddr(&mbus->DatagramQuery, slaveAddr);

	MBusQueryClear(&mbus->DatagramQuery);

	MBusSetQueryFunction(&mbus->DatagramQuery, READ_DISCRETE_INPUTS);
	MBusQueryRead(&mbus->DatagramQuery, startAddr, regCount);

	mbus->ptrResponseStruct = responseHolder;

	mbus->Status = MBUS_CH_TX_READY;
	MBusStartTx(mbus);

	return 1;
}
unsigned int MBusInputsRead(MBusMasterChanel* mbus,
	unsigned char		slaveAddr,
	unsigned short		startAddr,
	unsigned char		inputsCount,
	MBusRegister**		responseHolder)
{
	if (!MBusMasterIsReady(mbus)) return 0;

	mbus->Status = MBUS_CH_BUSY;

	MBusSetSlaveAddr(&mbus->DatagramQuery, slaveAddr);

	MBusQueryClear(&mbus->DatagramQuery);

	MBusSetQueryFunction(&mbus->DatagramQuery, READ_DISCRETE_INPUTS);
	MBusQueryRead(&mbus->DatagramQuery, startAddr, inputsCount);

	mbus->ptrResponseStruct = responseHolder;

	mbus->Status = MBUS_CH_TX_READY;
	MBusStartTx(mbus);

	return 1;
}

unsigned int MBusCoilWrite(MBusMasterChanel* mbus,
	unsigned char		slaveAddr,
	unsigned short		regAddr,
	unsigned short		regVal,
	unsigned char*		successFlag)
{
	if (!MBusMasterIsReady(mbus)) return 0;

	mbus->Status = MBUS_CH_BUSY;

	MBusSetSlaveAddr(&mbus->DatagramQuery, slaveAddr);

	MBusQueryClear(&mbus->DatagramQuery);

	MBusSetQueryFunction(&mbus->DatagramQuery, WRITE_SINGLE_COIL);
	MBusQueryWrite(&mbus->DatagramQuery, regAddr, regVal);

	mbus->ptrResponseStruct = successFlag;

	mbus->Status = MBUS_CH_TX_READY;
	MBusStartTx(mbus);

	return 1;
}

unsigned int MBusHoldingRegisterWrite(	MBusMasterChanel* mbus,
										unsigned char		slaveAddr,
										unsigned short		regAddr,
										unsigned short		regVal,
										unsigned char*		successFlag)
{
	if (!MBusMasterIsReady(mbus)) return 0;

	mbus->Status = MBUS_CH_BUSY;

	MBusSetSlaveAddr(&mbus->DatagramQuery, slaveAddr);

	MBusQueryClear(&mbus->DatagramQuery);

	MBusSetQueryFunction(&mbus->DatagramQuery, WRITE_SINGLE_HOLDING_REGISTER);
	MBusQueryWrite(&mbus->DatagramQuery, regAddr, regVal);

	mbus->ptrResponseStruct = successFlag;

	mbus->Status = MBUS_CH_TX_READY;
	MBusStartTx(mbus);

	return 1;
}

unsigned int MBusCoilsWrite(			MBusMasterChanel*	mbus,
										unsigned char		slaveAddr,
										unsigned short		startAddr,
										unsigned char		coilCount,
										MBusRegister**		coils,
										unsigned char*		coilsBuff,
										unsigned char*		successFlag)
{
	if (!MBusMasterIsReady(mbus)) return 0;

	mbus->Status = MBUS_CH_BUSY;

	MBusSetSlaveAddr(&mbus->DatagramQuery, slaveAddr);

	MBusQueryClear(&mbus->DatagramQuery);

	MBusSetQueryFunction(&mbus->DatagramQuery, WRITE_MULTIPLE_COILS);

	unsigned char bytesN = MBusRegistersPackDiscrete(coils, startAddr, coilCount, coilsBuff);

	MBusQueryWriteMultiple(&mbus->DatagramQuery, startAddr, coilsBuff, coilCount, bytesN);

	mbus->ptrResponseStruct = successFlag;

	mbus->Status = MBUS_CH_TX_READY;
	MBusStartTx(mbus);

	return 1;
}
unsigned int MBusHoldingRegistersWrite(	MBusMasterChanel* mbus,
										unsigned char		slaveAddr,
										unsigned short		startAddr,
										unsigned char		regCount,
										MBusRegister**		holdingRegisters,
										unsigned char*		holdingRegistersBuff,
										unsigned char*		successFlag)
{
	if (!MBusMasterIsReady(mbus)) return 0;

	mbus->Status = MBUS_CH_BUSY;

	MBusSetSlaveAddr(&mbus->DatagramQuery, slaveAddr);

	MBusQueryClear(&mbus->DatagramQuery);

	MBusSetQueryFunction(&mbus->DatagramQuery, WRITE_MULTIPLE_COILS);

	unsigned char bytesN = MBusRegistersPack(holdingRegisters, startAddr, regCount, holdingRegistersBuff);

	MBusQueryWriteMultiple(&mbus->DatagramQuery,
		startAddr,
		holdingRegistersBuff, 
		regCount, bytesN);

	mbus->ptrResponseStruct = successFlag;

	mbus->Status = MBUS_CH_TX_READY;
	MBusStartTx(mbus);

	return 1;
}

void MBusMasterTxBytesCompleted(		MBusMasterChanel* mbus)
{
	switch (mbus->Status)
	{
	case MBUS_CH_TX_READY:
		MBusMasterOnTxBytes_EXT(mbus, (unsigned char *)(&mbus->DatagramQuery.SlaveAddress), 1);
		mbus->Status = MBUS_CH_ADU_HEADER_TX;
		break;
	case MBUS_CH_ADU_HEADER_TX:
		MBusMasterOnTxBytes_EXT(mbus, (unsigned char *)(&mbus->DatagramQuery.PDU.FunctionCode), 1);
		mbus->Status = MBUS_CH_PDU_FUNC_CODE_TX;
		break;
	case MBUS_CH_PDU_FUNC_CODE_TX:
		if (mbus->DatagramQuery.PDU.DataHeaderLength == 0)
		{
			mbus->Status = MBUS_CH_ERR_UNEXPECTED;
			break;
		}

		MBusMasterOnTxBytes_EXT(mbus, mbus->DatagramQuery.PDU.DataHeader, mbus->DatagramQuery.PDU.DataHeaderLength);
		mbus->Status = MBUS_CH_PDU_HEADER_TX;
		break;
	case MBUS_CH_PDU_HEADER_TX:
		if (mbus->DatagramQuery.PDU.DataLength > 0)
		{
			MBusMasterOnTxBytes_EXT(mbus, mbus->DatagramQuery.PDU.Data, mbus->DatagramQuery.PDU.DataLength);
			mbus->Status = MBUS_CH_PDU_DATA_TX;
		}
		else
		{
			MBusMasterOnTxBytes_EXT(mbus, mbus->DatagramQuery.CRC16, 2);
			mbus->Status = MBUS_CH_ADU_CRC_TX;
		}
		break;
	case MBUS_CH_PDU_DATA_TX:
		MBusMasterOnTxBytes_EXT(mbus, mbus->DatagramQuery.CRC16, 2);
		mbus->Status = MBUS_CH_ADU_CRC_TX;
		break;
	case MBUS_CH_ADU_CRC_TX:
		mbus->Status = MBUS_CH_DEADTIME;
		break;
	default:
		mbus->Status = MBUS_CH_ERR_WRONG_TX_SEQUENCE;
		MBusMasterOnException_EXT(mbus);
		break;
	}
}

void MBusMasterRxByte(					MBusMasterChanel* mbus,
										unsigned char	  byte)
{
	static unsigned char byteIndex;
	switch(mbus->Status)
	{
	case MBUS_CH_ADU_HEADER_RX:
		MBusSetSlaveAddr(mbus->DatagramResponse, byte);
		mbus->Status = MBUS_CH_PDU_FUNC_CODE_RX;
		break;
	case MBUS_CH_PDU_FUNC_CODE_RX:
		MBusSetQueryFunction(mbus->DatagramResponse, (MBusFunction)byte);
		mbus->Status = MBUS_CH_PDU_HEADER_RX;
		byteIndex = 0;
		break;
	case MBUS_CH_PDU_HEADER_RX:
		if (MBusDatagramHeaderAppend(	mbus->DatagramResponse,
										byte,
										byteIndex++))
		{
			byteIndex = 0;
			MBusFunction fnc = mbus->DatagramResponse.PDU.FunctionCode;
			if (fnc == READ_COILS ||
					fnc == READ_DISCRETE_INPUTS ||
					fnc == READ_INPUTS ||
					fnc == READ_HOLDING_REGISTERS)
				mbus->Status = MBUS_CH_PDU_DATA_RX;
			else
				mbus->Status = MBUS_CH_ADU_CRC_RX;
		}
		break;
	case MBUS_CH_PDU_DATA_RX:
		if(MBusDatagramDataAppend(	mbus->DatagramResponse,
									byte,
									byteIndex++))
		{
			byteIndex = 0;
			mbus->Status = MBUS_CH_ADU_CRC_RX;
		}
		break;
	case MBUS_CH_ADU_CRC_RX:
		if(MBusDatagramCRCAppend(	mbus->DatagramResponse,
									byte,
									byteIndex++))
		{
			byteIndex = 0;
			mbus->Status = MBUS_CH_DEADTIME;
		}
		break;
	default:
		mbus->Status = MBUS_CH_ERR_WRONG_RESPONSE;
		break;
	}
}
void MBusMasterRxCompleted(			MBusMasterChanel* mbus)
{
	if (mbus->Status == MBUS_CH_PDU_DATA_RX &&
			mbus->DatagramResponse.PDU.DataLength >= MBUS_CRC_LENGTH)
	{
		mbus->DatagramResponse.PDU.DataLength -= MBUS_CRC_LENGTH;

		unsigned short crc_offset = mbus->DatagramResponse.PDU.DataLength;
		mbus->DatagramResponse.CRC16[MBUS_JUNIOR_BIT_INDEX] =
				mbus->DatagramResponse.PDU.Data[crc_offset + MBUS_JUNIOR_BIT_INDEX];
		mbus->DatagramResponse.CRC16[MBUS_SENIOR_BIT_INDEX] =
				mbus->DatagramResponse.PDU.Data[crc_offset + MBUS_SENIOR_BIT_INDEX];

		mbus->Status = MBUS_CH_DEADTIME;
	}

	if (mbus->Status != MBUS_CH_DEADTIME)
	{
		mbus->Status = MBUS_CH_ERR_DEADTIME_WRONG_TIMING;
		return;
	}

	switch (mbus->DatagramResponse.PDU.FunctionCode)
	{
	case READ_COILS:
	case READ_DISCRETE_INPUTS:
		MBusRegistersUnpackDiscrete(	mbus->DatagramResponse.PDU.Data,
										mbus->DatagramResponse.PDU.DataLength,
										mbus->ptrResponseStruct);
		break;
	case READ_HOLDING_REGISTERS:
	case READ_INPUTS:
		MBusRegistersUnpack(			mbus->DatagramResponse.PDU.Data,
										mbus->DatagramResponse.PDU.DataLength,
										mbus->ptrResponseStruct);
		break;
	case WRITE_SINGLE_COIL: break;
	case WRITE_SINGLE_HOLDING_REGISTER: break;
	case WRITE_MULTIPLE_COILS: break;
	case WRITE_MULTIPLE_HOLDING_REGISTERS: break;
	default:
		mbus->Status = MBUS_CH_ERR_WRONG_RESPONSE;
		return;
	}
}

void MBusSlaveOnRxBytes(MBusSlaveChanel* mbus,
	unsigned char* data,
	unsigned char dataLength)
{

}
