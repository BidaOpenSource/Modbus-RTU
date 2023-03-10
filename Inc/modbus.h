#ifndef MODBUS_H
#define MODBUS_H

#include "modbus_global.h"
#include "modbus_datagram.h"

typedef enum
{
	MBUS_CHANEL_STATUS_IDLE,
	MBUS_CHANEL_STATUS_RX,
	MBUS_CHANEL_STATUS_TX,
	MBUS_CHANEL_STATUS_TX_DEADTIME
}
MBusChanelStatus;

typedef struct
{
	unsigned char		Address;

	MBusChanelStatus	Status;

	MBusADU				DatagramRequest;
	MBusADU				DatagramResponse;

	void				(*DeadtimeTimerReset)();

	void				(*WatchdogTimerReset)();
	void				(*WatchdogTimerStop)();

	void				(*DatagramSend)(unsigned char*, unsigned char);

	void				(*OnTransactionCompleted)(MBusException);
}
MBusChanel;

#define MBusIsMaster(adu)	(!(adu->Address))

void MBusOnWatchdogElapsed(MBusChanel* mbus);
void MBusOnDeadtimeElapsed(MBusChanel* mbus);

void MBusOnByteReceived(MBusChanel* mbus, unsigned char c);
void MBusOnDatagramTransmitted(MBusChanel* mbus);

MBusException MBusRequest(MBusChanel* chanel, unsigned char slaveAddr, MBusFunctionType fnc, unsigned short* arguments);

#endif
