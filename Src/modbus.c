#include "modbus_global.h"
#include "modbus_datagram.h"
#include "modbus_map.h"
#include "modbus_fnc.h"
#include "modbus_crc16.h"
#include "modbus.h"

static void onException(MBusChanel* mbus, MBusException exc)
{
	if (mbus->OnException != 0) (*(mbus->OnException))(exc);
}

static MBusException fncProcessRequest(MBusChanel* mbus)
{
	MBusFunctionType fncType = *(mbus->DatagramRequest.PDU.FunctionCode);
	MBusFunction fnc = MBusFunctions[fncType];

	if (fnc.ProcessRequest == 0) return MBUS_EXC_UNEXPECTED;
	return (*fnc.ProcessRequest)(	mbus->DatagramRequest.PDU.Data,
									mbus->DatagramRequest.PDU.DataLength,
									mbus->DatagramResponse.PDU.Data,
									&(mbus->DatagramResponse.PDU.DataLength));
}

static MBusException fncProcessResponse(MBusChanel* mbus)
{
	MBusFunctionType fncType = *(mbus->DatagramResponse.PDU.FunctionCode);
	MBusFunction fnc = MBusFunctions[fncType];

	if (fnc.ProcessResponse == 0) return MBUS_EXC_UNEXPECTED;
	return (*fnc.ProcessResponse)(	mbus->DatagramRequest.PDU.Data,
									mbus->DatagramResponse.PDU.Data,
									mbus->DatagramResponse.PDU.DataLength);
}

void OnDeadtimeElapsed(MBusChanel* mbus)
{
	// elapsed deadtime after packet to send is generated
	if (mbus->Status == MBUS_CHANEL_STATUS_TX)
	{
		if (mbus->DatagramSend == 0) return;

		mbus->Status = MBUS_CHANEL_STATUS_RX;

		if (MBusIsMaster(mbus))	(*mbus->DatagramSend)(mbus->DatagramRequest.Data, mbus->DatagramRequest.DataLength);
		else					(*mbus->DatagramSend)(mbus->DatagramResponse.Data, mbus->DatagramResponse.DataLength);

		return;
	}
	// elapsed deadtime after master generated packet to send
	else
	{
		if (MBusIsMaster(mbus))
		{
			if (MBusMapCharBuffer(	&(mbus->DatagramResponse),
									mbus->DatagramResponse.Data,
									mbus->DatagramResponse.DataLength) != MBUS_EXC_NONE)
			{
				onException(mbus, *(mbus->DatagramResponse.PDU.ExceptionCode));
				return;
			}

			//	RESPONSE ADDRESS MUST BE EQUAL TO THE REQUEST ADDRESS
			if (MBusGetDatagramAddress(mbus->DatagramResponse.Data) != *(mbus->DatagramRequest.SlaveAddress))
			{
				onException(mbus, MBUS_EXC_UNEXPECTED);
				return;
			}

			if (!MBusFunctionSupported(*(mbus->DatagramRequest.PDU.FunctionCode)))
			{
				ExceptionCodeAssign(MBUS_EXC_ILLEGAL_FUNCTION, mbus->DatagramResponse.Data, &(mbus->DatagramResponse.DataLength));
				onException(mbus, MBUS_EXC_ILLEGAL_FUNCTION);
				return;
			}

			MBusException fncResult = fncProcessResponse(mbus);
			if (fncResult != MBUS_EXC_NONE) { onException(mbus, fncResult);	return;	}
		}
		else
		{
			if (MBusMapCharBuffer(	&(mbus->DatagramRequest),
									mbus->DatagramRequest.Data,
									mbus->DatagramRequest.DataLength) != MBUS_EXC_NONE)
			{
				onException(mbus, *(mbus->DatagramRequest.PDU.ExceptionCode));
				return;
			}

			if (MBusGetDatagramAddress(mbus->DatagramRequest.Data) != mbus->Address) { return; }

			if (!MBusFunctionSupported(*(mbus->DatagramRequest.PDU.FunctionCode)))
			{
				ExceptionCodeAssign(MBUS_EXC_ILLEGAL_FUNCTION, mbus->DatagramResponse.Data, &(mbus->DatagramResponse.DataLength));
				onException(mbus, MBUS_EXC_ILLEGAL_FUNCTION);
			}
			else
			{
				MBusException fncResult = fncProcessRequest(mbus);

				if (fncResult != MBUS_EXC_NONE) { onException(mbus, fncResult); }
			}
		}
	}
}
void OnByteReceived(MBusChanel* mbus, unsigned char c)
{
	if (mbus->Status == MBUS_CHANEL_STATUS_RX)
	{
		if (MBusIsMaster(mbus))
		{
			if (mbus->DatagramResponse.DataLength < MBUS_DATAGRAM_MAX_LENGTH)
				mbus->DatagramResponse.Data[mbus->DatagramResponse.DataLength++] = c;
		}
		else
		{
			if (mbus->DatagramRequest.DataLength < MBUS_DATAGRAM_MAX_LENGTH)
				mbus->DatagramRequest.Data[mbus->DatagramRequest.DataLength++] = c;
		}

		if (mbus->DeadtimeTimerReset != 0) (*mbus->DeadtimeTimerReset)();
	}
}

void MBusReadCoils(MBusChanel* mbus, unsigned char slaveAddr, unsigned short startAddr, unsigned short coilsNum)
{
	/*if (!MBusIsMaster(mbus)) { onException(mbus, MBUS_EXC_UNEXPECTED); return; }
	if (mbus->Status != MBUS_CHANEL_STATUS_IDLE) return;

	MBusMapCharBufferHeader(&(mbus->DatagramRequest));

	unsigned short args[2] = { startAddr, coilsNum };
	MBusFunctions[MBUS_FNC_READ_COILS].GenerateRequest(args, mbus->DatagramRequest.PDU.Data, &(mbus->DatagramRequest.PDU.DataLength));

	unsigned short crc16 = MBusCRC16(MODBUS_CRC_START, mbus->DatagramRequest.Data, mbus->DatagramRequest.DataLength);
	MBusMapCharBufferFooter(&(mbus->DatagramRequest), crc16);

	mbus->Status = MBUS_CHANEL_STATUS_TX;

	if (mbus->DeadtimeTimerReset != 0) (*mbus->DeadtimeTimerReset)();*/
}
void MBusReadDiscreteInputs(MBusChanel* mbus, unsigned char slaveAddr, unsigned short startAddr, unsigned short coilsNum) {}
void MBusReadHoldingRegisters(MBusChanel* mbus, unsigned char slaveAddr, unsigned short startAddr, unsigned short coilsNum) {}
void MBusReadInputRegisters(MBusChanel* mbus, unsigned char slaveAddr, unsigned short startAddr, unsigned short coilsNum) {}

void MBusWriteSingleCoil(MBusChanel* mbus, unsigned char slaveAddr, unsigned short addr, unsigned short value) {}
void MBusWriteSingleRegister(MBusChanel* mbus, unsigned char slaveAddr, unsigned short addr, unsigned short value) {}
/*
MBusChanel ch1;
unsigned int x = 11111;

void MBusTestRegisterCreate()
{
	MBusRegAdd(&Coils, 266, &x, 0b00001000);
}

void MBusTestGenerateRequest()
{
	MBusReadCoils(&ch1, 2, 266, 1);
}

void MBusTestMasterToSlave()
{
	ch1.Status = MBUS_CHANEL_STATUS_RX;

	OnByteReceived(&ch1, 2);	// addr
	OnByteReceived(&ch1, MBUS_FNC_READ_HOLDING_REGISTERS);
	OnByteReceived(&ch1, 1);
	OnByteReceived(&ch1, 10);
	OnByteReceived(&ch1, 0);
	OnByteReceived(&ch1, 1);
	OnByteReceived(&ch1, 0);
	OnByteReceived(&ch1, 0);

	OnDeadtimeElapsed(&ch1);
}

void MBusTestSlaveToMaster()
{

	ch1.Status = MBUS_CHANEL_STATUS_RX;
	ch1.Address = 2;

	OnByteReceived(&ch1, 2);
	OnByteReceived(&ch1, MBUS_FNC_READ_HOLDING_REGISTERS);
	OnByteReceived(&ch1, 0);
	OnByteReceived(&ch1, 1);
	OnByteReceived(&ch1, 0);
	OnByteReceived(&ch1, 3);
	OnByteReceived(&ch1, 0);
	OnByteReceived(&ch1, 0);

	OnDeadtimeElapsed(&ch1);
}

void MBusTest()
{
	MBusTestRegisterCreate();
	MBusTestGenerateRequest();
	MBusTestMasterToSlave();
	MBusTestSlaveToMaster();
}
*/
