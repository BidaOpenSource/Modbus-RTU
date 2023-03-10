#include "modbus_global.h"
#include "modbus_datagram.h"
#include "modbus_map.h"
#include "modbus_fnc.h"
#include "modbus_crc16.h"
#include "modbus.h"

static void onTransactionCompleted(MBusChanel* mbus, MBusException exc)
{
	if (mbus->OnTransactionCompleted != 0) (*(mbus->OnTransactionCompleted))(exc);
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

void MBusOnWatchdogElapsed(MBusChanel* mbus)
{
	if (MBusIsMaster(mbus))
	{
		mbus->Status = MBUS_CHANEL_STATUS_IDLE;
		onTransactionCompleted(mbus, MBUS_EXC_SLAVE_DEVICE_FAILURE);
	}
}

void MBusOnDeadtimeElapsed(MBusChanel* mbus)
{
	if (MBusIsMaster(mbus))
	{
		if (mbus->Status == MBUS_CHANEL_STATUS_TX_DEADTIME)
		{
			mbus->Status = MBUS_CHANEL_STATUS_RX;
			return;
		}
		else
			mbus->Status = MBUS_CHANEL_STATUS_IDLE;

		if (MBusMapCharBuffer(	&(mbus->DatagramResponse),
								mbus->DatagramResponse.Data,
								mbus->DatagramResponse.DataLength) != MBUS_EXC_NONE)
		{
			onTransactionCompleted(mbus,	*(mbus->DatagramResponse.PDU.ExceptionCode));
			return;
		}

		//	RESPONSE ADDRESS MUST BE EQUAL TO THE REQUEST ADDRESS
		if (  MBusGetDatagramAddress(mbus->DatagramResponse.Data) !=
							*(mbus->DatagramRequest.SlaveAddress))
		{
			onTransactionCompleted(mbus, MBUS_EXC_UNEXPECTED);
			return;
		}

		if (!MBusFunctionSupported(*(mbus->DatagramRequest.PDU.FunctionCode)))
		{
			ExceptionCodeAssign(MBUS_EXC_ILLEGAL_FUNCTION, mbus->DatagramResponse.Data, &(mbus->DatagramResponse.DataLength));
			onTransactionCompleted(mbus, MBUS_EXC_ILLEGAL_FUNCTION);
			return;
		}

		MBusException fncResult = fncProcessResponse(mbus);
		if (fncResult != MBUS_EXC_NONE) { onTransactionCompleted(mbus, fncResult);	return;	}
	}
	else
	{
		if (MBusMapCharBuffer(	&(mbus->DatagramRequest),
								mbus->DatagramRequest.Data,
								mbus->DatagramRequest.DataLength) != MBUS_EXC_NONE)
		{
			onTransactionCompleted(mbus, *(mbus->DatagramRequest.PDU.ExceptionCode));
			return;
		}

		if (MBusGetDatagramAddress(mbus->DatagramRequest.Data) != mbus->Address) { return; }

		if (!MBusFunctionSupported(*(mbus->DatagramRequest.PDU.FunctionCode)))
		{
			ExceptionCodeAssign(MBUS_EXC_ILLEGAL_FUNCTION, mbus->DatagramResponse.Data, &(mbus->DatagramResponse.DataLength));
			onTransactionCompleted(mbus, MBUS_EXC_ILLEGAL_FUNCTION);
		}
		else
		{
			MBusException fncResult = fncProcessRequest(mbus);

			if (fncResult != MBUS_EXC_NONE) { onTransactionCompleted(mbus, fncResult); }
		}
	}
}
void MBusOnByteReceived(MBusChanel* mbus, unsigned char c)
{
	if (mbus->Status == MBUS_CHANEL_STATUS_RX)
	{
		if (MBusIsMaster(mbus))
		{
			if (mbus->WatchdogTimerStop != 0) (*mbus->WatchdogTimerStop)();

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

void MBusOnDatagramTransmitted(MBusChanel* mbus)
{
	mbus->Status = MBUS_CHANEL_STATUS_TX_DEADTIME;

	if (mbus->DeadtimeTimerReset != 0) (*mbus->DeadtimeTimerReset)();

	if (MBusIsMaster(mbus) && mbus->WatchdogTimerReset != 0) (*mbus->WatchdogTimerReset)();
}

MBusException MBusRequest(MBusChanel* chanel, unsigned char slaveAddr, MBusFunctionType fnc, unsigned short* arguments)
{
	if (!MBusIsMaster(chanel))						return MBUS_EXC_UNEXPECTED;
	if (chanel->Status != MBUS_CHANEL_STATUS_IDLE)	return MBUS_EXC_SLAVE_DEVICE_BUSY;
	if (!MBusFunctionSupported(fnc))				return MBUS_EXC_ILLEGAL_FUNCTION;

	MBusMapCharBufferHeader(&(chanel->DatagramRequest));
	*(chanel->DatagramRequest.SlaveAddress) = slaveAddr;
	*(chanel->DatagramRequest.PDU.FunctionCode) = (unsigned char)fnc;

	MBusException err;
	err = MBusFunctions[fnc].GenerateRequest(		arguments,
													chanel->DatagramRequest.PDU.Data,
													&(chanel->DatagramRequest.PDU.DataLength));
	if (err != MBUS_EXC_NONE) return err;

	unsigned short crc16 = MBusCRC16(				MODBUS_CRC_START,
													chanel->DatagramRequest.Data,
													chanel->DatagramRequest.DataLength);

	MBusMapCharBufferFooter(						&(chanel->DatagramRequest),
													crc16);

	chanel->Status = MBUS_CHANEL_STATUS_TX;

	if (chanel->DatagramSend != 0) (*chanel->DatagramSend)(	chanel->DatagramRequest.Data,
															chanel->DatagramRequest.DataLength);

	return MBUS_EXC_NONE;
}
