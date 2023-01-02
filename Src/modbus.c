#include "modbus.h"
#include "modbus_datagram.h"
#include "modbus_registers.h"

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

	MBusSetSlaveAddr(&mbus->Datagram, slaveAddr);

	MBusQueryClear(&mbus->Datagram);

	MBusSetQueryFunction(&mbus->Datagram, READ_COILS);
	MBusQueryRead(&mbus->Datagram, startAddr, coilsCount);

	mbus->ptrResponseStruct = responseHolder;

	mbus->Status = MBUS_CH_TX_READY;

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

	MBusSetSlaveAddr(&mbus->Datagram, slaveAddr);

	MBusQueryClear(&mbus->Datagram);

	MBusSetQueryFunction(&mbus->Datagram, READ_DISCRETE_INPUTS);
	MBusQueryRead(&mbus->Datagram, startAddr, inputsCount);

	mbus->ptrResponseStruct = responseHolder;

	mbus->Status = MBUS_CH_TX_READY;

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

	MBusSetSlaveAddr(&mbus->Datagram, slaveAddr);

	MBusQueryClear(&mbus->Datagram);

	MBusSetQueryFunction(&mbus->Datagram, READ_DISCRETE_INPUTS);
	MBusQueryRead(&mbus->Datagram, startAddr, regCount);

	mbus->ptrResponseStruct = responseHolder;

	mbus->Status = MBUS_CH_TX_READY;

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

	MBusSetSlaveAddr(&mbus->Datagram, slaveAddr);

	MBusQueryClear(&mbus->Datagram);

	MBusSetQueryFunction(&mbus->Datagram, READ_DISCRETE_INPUTS);
	MBusQueryRead(&mbus->Datagram, startAddr, inputsCount);

	mbus->ptrResponseStruct = responseHolder;

	mbus->Status = MBUS_CH_TX_READY;

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

	MBusSetSlaveAddr(&mbus->Datagram, slaveAddr);

	MBusQueryClear(&mbus->Datagram);

	MBusSetQueryFunction(&mbus->Datagram, WRITE_SINGLE_COIL);
	MBusQueryWrite(&mbus->Datagram, regAddr, regVal);

	mbus->ptrResponseStruct = successFlag;

	mbus->Status = MBUS_CH_TX_READY;

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

	MBusSetSlaveAddr(&mbus->Datagram, slaveAddr);

	MBusQueryClear(&mbus->Datagram);

	MBusSetQueryFunction(&mbus->Datagram, WRITE_SINGLE_HOLDING_REGISTER);
	MBusQueryWrite(&mbus->Datagram, regAddr, regVal);

	mbus->ptrResponseStruct = successFlag;

	mbus->Status = MBUS_CH_TX_READY;

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

	MBusSetSlaveAddr(&mbus->Datagram, slaveAddr);

	MBusQueryClear(&mbus->Datagram);

	MBusSetQueryFunction(&mbus->Datagram, WRITE_MULTIPLE_COILS);

	unsigned char bytesN = MBusRegistersPackDiscrete(coils, startAddr, coilCount, coilsBuff);

	MBusQueryWriteMultiple(&mbus->Datagram, startAddr, coilsBuff, coilCount, bytesN);

	mbus->ptrResponseStruct = successFlag;

	mbus->Status = MBUS_CH_TX_READY;

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

	MBusSetSlaveAddr(&mbus->Datagram, slaveAddr);

	MBusQueryClear(&mbus->Datagram);

	MBusSetQueryFunction(&mbus->Datagram, WRITE_MULTIPLE_COILS);

	unsigned char bytesN = MBusRegistersPack(holdingRegisters, startAddr, regCount, holdingRegistersBuff);

	MBusQueryWriteMultiple(&mbus->Datagram, 
		startAddr,
		holdingRegistersBuff, 
		regCount, bytesN);

	mbus->ptrResponseStruct = successFlag;

	mbus->Status = MBUS_CH_TX_READY;

	return 1;
}

void MBusMasterOnTxDeadtime(MBusMasterChanel* mbus)
{
	if (mbus->Status == MBUS_CH_TX_READY)
	{
		mbus->Status = MBUS_CH_ADU_HEADER_TX;
		MBusMasterOnTxBytes_IT(mbus, &mbus->Datagram.SlaveAddress, 1);
	}
}
void MBusMasterOnTxBytesEnd(MBusMasterChanel* mbus)
{
	switch (mbus->Status)
	{
	case MBUS_CH_ADU_HEADER_TX:
		mbus->Status = MBUS_CH_PDU_FUNC_CODE_TX;
		MBusMasterOnTxBytes_IT(mbus, (unsigned char *)(&mbus->Datagram.PDU.FunctionCode), 1);
		break;
	case MBUS_CH_PDU_FUNC_CODE_TX:
		mbus->Status = MBUS_CH_PDU_HEADER_TX;
		MBusMasterOnTxBytes_IT(mbus, mbus->Datagram.PDU.DataHeader, mbus->Datagram.PDU.DataHeaderLength);
		break;
	case MBUS_CH_PDU_HEADER_TX:
		mbus->Status = MBUS_CH_PDU_DATA_TX;
		MBusMasterOnTxBytes_IT(mbus, mbus->Datagram.PDU.Data, mbus->Datagram.PDU.DataLength);
		break;
	case MBUS_CH_PDU_DATA_TX:
		mbus->Status = MBUS_CH_ADU_CRC_TX;
		MBusMasterOnTxBytes_IT(mbus, mbus->Datagram.CRC16, 2);
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
void MBusMasterOnRxBytes(MBusMasterChanel* mbus,
	unsigned char* data,
	unsigned char	dataLength)
{
	if (mbus->Status != MBUS_CH_PENDING_RESPONSE)
	{
		mbus->Status = MBUS_CH_ERR_WRONG_RX_SEQUENCE;
		MBusMasterOnException_IT(mbus);
		return;
	}

	MBusDatagramParseResponse(&mbus->Datagram,
								data,
								dataLength);
}

