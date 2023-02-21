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
	MBusFunctionStatus Status;
	MBusException	(*GenerateRequest)(	unsigned short* arguments,
										unsigned char* 	requestData,
										unsigned char*  requestDataLength);

	MBusException	(*ProcessRequest)(	unsigned char* request,
										unsigned char  requestLength,
										unsigned char* response,
										unsigned char* responseLength);

	MBusException	(*ProcessResponse)( unsigned char* response,
										unsigned char  responseLength);
}
MBusFunction;

extern MBusFunction MBusFunctions[MBUS_FNC_COUNT];

#define				MBusFunctionSupported(fnc)			(fnc < MBUS_FNC_COUNT && MBusFunctions[fnc].Status == MBUS_FNC_ENABLED)

MBusException		ExceptionCodeAssign(MBusException exc, unsigned char* array, unsigned char* arrayLength);

MBusException		FncReadCoilRequest(unsigned short startAddress, unsigned short quantityOfRegs, unsigned char* requestData, unsigned char* requestDataLength);
MBusException		FncReadDiscreteInputsRequest(unsigned short startAddress, unsigned short quantityOfRegs, unsigned char* requestData, unsigned char* requestDataLength);
MBusException		FncReadHoldingRegistersRequest(unsigned short startAddress, unsigned short quantityOfRegs, unsigned char* requestData, unsigned char* requestDataLength);
MBusException		FncReadInputRegistersRequest(unsigned short startAddress, unsigned short quantityOfRegs, unsigned char* requestData, unsigned char* requestDataLength);
MBusException		FncWriteSingleCoilRequest(unsigned short address, unsigned short value, unsigned char* requestData, unsigned char* requestDataLength);
MBusException		FncWriteSingleRegisterRequest(unsigned short address, unsigned short value, unsigned char* requestData, unsigned char* requestDataLength);

#endif
