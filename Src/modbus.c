#include "modbus.h"
#include "modbus_global.h"
#include "modbus_datagram.h"
#include "modbus_registers.h"

static void	 MBusStartTx(MBusMasterChanel* mbus) { MBusMasterOnTxBytesEnd(mbus); }

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

static char mBusMasterQueryResponseValidate(MBusMasterChanel* mbus)
{
	if (mbus->DatagramQuery.SlaveAddress != mbus->DatagramResponse.SlaveAddress)
		return 0;
	if (mbus->DatagramQuery.PDU.FunctionCode != mbus->DatagramResponse.SlaveAddress)
		return 0;

	return 1;
}

void MBusMasterOnTxDeadtime(MBusMasterChanel* mbus)
{
	if (mbus->Status == MBUS_CH_DEADTIME)
	{
		mbus->Status = MBUS_CH_READY;
	}
}
void MBusMasterOnTxBytesEnd(MBusMasterChanel* mbus)
{
	switch (mbus->Status)
	{
	case MBUS_CH_TX_READY:
		mbus->Status = MBUS_CH_ADU_HEADER_TX;
		MBusMasterOnTxBytes_IT(mbus, (unsigned char *)(&mbus->DatagramQuery.SlaveAddress), 1);
		break;
	case MBUS_CH_ADU_HEADER_TX:
		mbus->Status = MBUS_CH_PDU_FUNC_CODE_TX;
		MBusMasterOnTxBytes_IT(mbus, (unsigned char *)(&mbus->DatagramQuery.PDU.FunctionCode), 1);
		break;
	case MBUS_CH_PDU_FUNC_CODE_TX:
		if (mbus->DatagramQuery.PDU.DataHeaderLength == 0)
		{
			mbus->Status = MBUS_CH_ERR_UNEXPECTED;
			break;
		}
		else
			mbus->Status = MBUS_CH_PDU_HEADER_TX;
		MBusMasterOnTxBytes_IT(mbus, mbus->DatagramQuery.PDU.DataHeader, mbus->DatagramQuery.PDU.DataHeaderLength);
		break;
	case MBUS_CH_PDU_HEADER_TX:
		if (mbus->DatagramQuery.PDU.DataLength > 0)
		{
			mbus->Status = MBUS_CH_PDU_DATA_TX;
			MBusMasterOnTxBytes_IT(mbus, mbus->DatagramQuery.PDU.Data, mbus->DatagramQuery.PDU.DataLength);
		}
		else
		{
			mbus->Status = MBUS_CH_ADU_CRC_TX;
			MBusMasterOnTxBytes_IT(mbus, mbus->DatagramQuery.CRC16, 2);
		}
		break;
	case MBUS_CH_PDU_DATA_TX:
		mbus->Status = MBUS_CH_ADU_CRC_TX;
		MBusMasterOnTxBytes_IT(mbus, mbus->DatagramQuery.CRC16, 2);
		break;
	case MBUS_CH_ADU_CRC_TX:
		mbus->Status = MBUS_CH_PENDING_RESPONSE;
		break;
	default:
		mbus->Status = MBUS_CH_ERR_WRONG_TX_SEQUENCE;
		MBusMasterOnException_IT(mbus);
		break;
	}
}

void MBusMasterOnRxByte(				MBusMasterChanel* mbus,
										unsigned char	  byte)
{
	if (mbus->datagramResponseBytesReceived >= MBUS_MAX_PACKET_LENGTH)
		return;

	switch (mbus->Status)
	{
	case MBUS_CH_ADU_HEADER_RX:
		mbus->DatagramResponse.SlaveAddress = byte;
		mbus->Status = MBUS_CH_PDU_FUNC_CODE_RX;
		break;
	case MBUS_CH_PDU_FUNC_CODE_RX:
		mbus->DatagramResponse.PDU.FunctionCode = byte;

		datagramResponseStageBytesReceived = 0;
		switch (mbus->DatagramResponse.PDU.FunctionCode)
		{
		case READ_COILS:
		case READ_DISCRETE_INPUTS:
		case READ_INPUTS:
		case READ_HOLDING_REGISTERS:
			mbus->datagramResponseStageBytesCountdown = 1;
			break;
		case WRITE_SINGLE_COIL:
		case WRITE_SINGLE_HOLDING_REGISTER:
		case WRITE_MULTIPLE_COILS:
		case WRITE_MULTIPLE_HOLDING_REGISTERS:
			mbus->datagramResponseStageBytesCountdown = 4;
			break;
		default:
			mbus->Status = MBUS_CH_ERR_UNEXPECTED;
			return;
		}

		mbus->Status = MBUS_CH_PDU_HEADER_RX;
		break;
	case MBUS_CH_PDU_HEADER_RX:
		mbus->DatagramResponse.PDU.DataHeader[datagramResponseStageBytesReceived] = byte;
		mbus->datagramResponseStageBytesReceived++;

		mbus->datagramResponseStageBytesCountdown--;
		if (!mbus->datagramResponseStageBytesCountdown ||
				mbus->datagramResponseStageBytesReceived >= MBUS_MAX_PDU_HEADER_LENGTH)
		{
			mbus->datagramResponseStageBytesReceived = 0;
			switch (mbus->DatagramResponse.PDU.FunctionCode)
			{
			case READ_COILS:
			case READ_DISCRETE_INPUTS:
			case READ_INPUTS:
			case READ_HOLDING_REGISTERS:
				mbus->datagramResponseStageBytesCountdown = MBUS_MAX_PDU_LENGTH;
				mbus->Status = MBUS_CH_PDU_DATA_RX;
				break;
			case WRITE_SINGLE_COIL:
			case WRITE_SINGLE_HOLDING_REGISTER:
			case WRITE_MULTIPLE_COILS:
			case WRITE_MULTIPLE_HOLDING_REGISTERS:
				mbus->datagramResponseStageBytesCountdown = MBUS_CRC_LENGTH;
				mbus->Status = MBUS_CH_ADU_CRC_RX;
				break;
			default:
				mbus->Status = MBUS_CH_ERR_UNEXPECTED;
				return;
			}
		}
		break;
	case MBUS_CH_PDU_DATA_RX:
		if (!mbus->datagramResponseStageBytesCountdown) return;

		mbus->DatagramResponse.PDU.DataHeader[datagramResponseStageBytesReceived] = byte;
		mbus->datagramResponseStageBytesReceived++;
		mbus->datagramResponseStageBytesCountdown--;
		break;
	case MBUS_CH_ADU_CRC_RX:
		if (!mbus->datagramResponseStageBytesCountdown) return;

		mbus->DatagramResponse.CRC16[mbus->datagramResponseStageBytesReceived];
		mbus->datagramResponseStageBytesReceived++;
		mbus->datagramResponseStageBytesCountdown--;
		break;
	}
	default:
		mbus->Status = MBUS_CH_ERR_WRONG_RX_SEQUENCE;
		return;
}
void MBusMasterOnRxCompleted(			MBusMasterChanel* mbus)
{
	if (mbus->Status != MBUS_CH_PDU_DATA_RX &&
			mbus->Status != MBUS_CH_ADU_CRC_RX)
	{
		mbus->Status = MBUS_CH_ERR_WRONG_RX_SEQUENCE;
		MBusMasterOnException_IT(mbus);
		return;
	}

	if (!mBusMasterQueryResponseValidate(mbus))
	{
		mbus->Status = MBUS_CH_ERR_WRONG_RESPONSE;
		MBusMasterOnException_IT(mbus);
		return;
	}

	if (mbus->Status == MBUS_CH_PDU_DATA_RX)
	{
		mbus->DatagramResponse.PDU.DataLength -= 2;
		mbus->DatagramResponse.CRC16[0] =
				mbus->DatagramResponse.PDU.Data[mbus->DatagramResponse.PDU.DataLength];
		mbus->DatagramResponse.CRC16[1] =
				mbus->DatagramResponse.PDU.Data[mbus->DatagramResponse.PDU.DataLength + 1];
	}

	switch (mbus->DatagramResponse.PDU.FunctionCode)
	{
	case READ_COILS:
	case READ_DISCRETE_INPUTS:

		break;
	case READ_HOLDING_REGISTERS:
	case READ_INPUTS:
		break;
	}

	mbus->Status = MBUS_CH_READY;
}

void MBusSlaveOnRxBytes(MBusSlaveChanel* mbus,
	unsigned char* data,
	unsigned char dataLength)
{
	if (mbus->Status != MBUS_CH_PENDING_RESPONSE)
	{
		mbus->Status = MBUS_CH_ERR_WRONG_RX_SEQUENCE;
		;//MBusOnException_IT(mbus);
		return;
	}

	MBusDatagramParseQuery(&mbus->Datagram, data, dataLength);
}
