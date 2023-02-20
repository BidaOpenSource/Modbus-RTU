#include "modbus_global.h"
#include "modbus.h"

void OnDeadtimeElapsed(MBusChanel* mbus)
{

}
void OnByteReceived(MBusChanel* mbus, unsigned char c)
{
	if (mbus->Status == MBUS_CHANEL_STATUS_RX &&
			mbus->DataLength < MBUS_DATAGRAM_MAX_LENGTH)
	{
		mbus->Data[mbus->DataLength] = c;
		mbus->DataLength++;

		if (mbus->DeadtimeTimerReset != 0) (*mbus->DeadtimeTimerReset)();
	}
}
