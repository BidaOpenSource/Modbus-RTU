#include "modbus_chanels.h"

MBusChanel	MBusVFD;
MBusChanel	MBusPC;

#define		MBUS_VFD_ADDRESS	81

void		MBusChanelsInit()
{
	MBusVFD = (MBusChanel)
	{
		.Address = MBUS_VFD_ADDRESS,
		.Status = MBUS_CHANEL_STATUS_IDLE,
		// UNFINISHED
	};

	MBusPC = (MBusChanel)
	{
		.Address = 0x01,
		.Status = MBUS_CHANEL_STATUS_
		// UNFINISHED
	};
}
