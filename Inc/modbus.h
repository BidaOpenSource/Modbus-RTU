#ifndef MODBUS_H
#define MODBUS_H

#include "modbus_global.h"
#include "modbus_datagram.h"

typedef enum
{
	MBUS_CHANEL_STATUS_IDLE,
	MBUS_CHANEL_STATUS_RX,
	MBUS_CHANEL_STATUS_TX,
	MBUS_CHANEL_STATUS_PROCESSING
}
MBusChanelStatus;

typedef struct
{
	unsigned char		Address;

	MBusChanelStatus	Status;

	MBusADU				DatagramRequest;
	MBusADU				DatagramResponse;

	void				(*DeadtimeTimerReset)();
	void				(*DatagramSend)(unsigned char*, unsigned char);

	void				(*OnException)(MBusException);
}
MBusChanel;

#define MBusIsMaster(adu)	(!(adu->Address))

void OnDeadtimeElapsed(MBusChanel* mbus);
void OnByteReceived(MBusChanel* mbus, unsigned char c);

void MBusTestSlaveToMaster();
void MBusTestMasterToSlave();

#endif
