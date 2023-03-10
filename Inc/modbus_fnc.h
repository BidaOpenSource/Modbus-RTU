#ifndef MODBUS_FNC_H
#define MODBUS_FNC_H

#include "modbus_global.h"

typedef enum
{
	MBUS_FNC_DISABLED,
	MBUS_FNC_ENABLED
}
MBusFunctionStatus;

typedef struct
{
	MBusFunctionStatus					Status;

	MBusException	(*GenerateRequest)(	unsigned short* arguments,
										unsigned char* 	requestData,
										unsigned char*  requestDataLength);

	MBusException	(*ProcessRequest)(	unsigned char*  request,
										unsigned char   requestLength,
										unsigned char*  response,
										unsigned char*  responseLength);

	MBusException	(*ProcessResponse)( unsigned char*  request,
										unsigned char*  response,
										unsigned char   responseLength);
}
MBusFunction;

extern MBusFunction MBusFunctions[MBUS_FNC_COUNT];

#define				MBusFunctionSupported(fnc)			(fnc < MBUS_FNC_COUNT && MBusFunctions[fnc].Status == MBUS_FNC_ENABLED)

MBusException		ExceptionCodeAssign(MBusException exc, unsigned char* array, unsigned char* arrayLength);

#endif
