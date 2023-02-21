#include "modbus_global.h"
#include "modbus_datagram.h"
#include "modbus_map.h"
#include "modbus_fnc.h"
#include "modbus.h"

static void onException(MBusChanel* mbus, MBusException exc)
{
	if (mbus->OnException != 0) (*(mbus->OnException))(exc);
}

static MBusException fncProcessRequest(MBusChanel* mbus)
{
	MBusFunctionType fncType = *(mbus->Datagram.PDU.FunctionCode);
	MBusFunction fnc = MBusFunctions[fncType];

	return (*fnc.ProcessRequest)(mbus->Datagram.PDU.Data, mbus->Datagram.PDU.DataLength, mbus->Datagram.PDU.Data, &(mbus->Datagram.PDU.DataLength));
}

static MBusException fncProcessResponse(MBusChanel* mbus)
{
	MBusFunctionType fncType = *(mbus->Datagram.PDU.FunctionCode);
	MBusFunction fnc = MBusFunctions[fncType];

	return (*fnc.ProcessResponse)(mbus->Datagram.PDU.Data, mbus->Datagram.PDU.DataLength);
}

void OnDeadtimeElapsed(MBusChanel* mbus)
{
	if (MBusIsMaster(mbus))
	{
		if (MBusMapCharBuffer(&(mbus->DatagramResponse), mbus->DatagramResponse.Data, mbus->DatagramResponse.DataLength) != MBUS_EXC_NONE)
		{
			onException(mbus, *(mbus->DatagramResponse.PDU.ExceptionCode));
		}

		//	RESPONSE ADDRESS MUST BE EQUAL TO THE REQUEST ADDRESS
		if (MBusGetDatagramAddress(mbus->Data) != mbus->Datagram.SlaveAddress)
		{
			onException(mbus, MBUS_EXC_UNEXPECTED);
			return;
		}

		MBusException fncResult = fncProcessRequest(mbus);
		if (fncResult != MBUS_EXC_NONE)
		{
			onException(mbus, fncResult);
		}
	}
	else
	{
		if (MBusMapCharBuffer(&(mbus->Datagram), mbus->Data, mbus->DataLength) != MBUS_EXC_NONE)
		{
			onException(mbus, *(mbus->Datagram.PDU.ExceptionCode));
			return;
		}

		if (!MBusFunctionSupported(*(mbus->Datagram.PDU.FunctionCode)))
		{
			onException(mbus, MBUS_EXC_UNEXPECTED);
			return;
		}

		MBusException fncResult = fncProcessResponse(mbus);
		if (fncResult != MBUS_EXC_NONE)
		{
			onException(mbus, fncResult);
		}
	}
}
void OnByteReceived(MBusChanel* mbus, unsigned char c)
{
	if (mbus->Status == MBUS_CHANEL_STATUS_RX)
	{
		if (mbus->Address)
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

MBusChanel ch1;
unsigned int x = 11111;
void MBusTest()
{
	MBusRegAdd(&HoldingRegisters, 266, &x, 0xFFFF);

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
