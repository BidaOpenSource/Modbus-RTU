#ifndef MODBUS_REGISTERS_H
#define MODBUS_REGISTERS_H

#include "modbus_variable.h"

typedef enum
{
	MBUS_REG_OK,
	MBUS_REG_ERR_UNEXPECTED,
	MBUS_REG_ERR_ALREADY_EXISTS,
	MBUS_REG_ERR_INVALID_VARIABLE,
	MBUS_REG_ERR_INVALID_ADDRESS,
	MBUS_REG_ERR_OUT_OF_BOUNDS
}
MBusRegStatus;

typedef struct
{
	MBusVariable	Variable;
	unsigned short	Address;
}
MBusRegister;

#define				MBUS_REG_MAX_REGISTERS_IN_REGSET	20
typedef struct
{
	unsigned short	RegistersCount;
	MBusRegister 	Registers[MBUS_REG_MAX_REGISTERS_IN_REGSET];
}
MBusRegisterSet;

MBusRegStatus		MBusRegAdd(MBusRegisterSet* regSet, unsigned short regAddr, unsigned int* variablePointer, unsigned int bitMask);
MBusRegStatus		MBusRegRemove(MBusRegisterSet* regSet, unsigned short regAddr);

MBusRegStatus		MBusVarGet(MBusRegisterSet* regSet, unsigned short regAddr, MBusVariable** out);

MBusRegStatus		MBusRegSetPack16bit(MBusRegisterSet* regSet, unsigned short startAddr, unsigned short regCount, unsigned char* buffer);
MBusRegStatus		MBusRegSetUnpack16bit(MBusRegisterSet* regSet, unsigned short startAddr, unsigned short regCount, unsigned char* buffer);

MBusRegStatus		MBusRegSetPack1bit(MBusRegisterSet* regSet, unsigned short startAddr, unsigned short regCount, unsigned char* buffer);
MBusRegStatus		MBusRegSetUnpack1bit(MBusRegisterSet* regSet, unsigned short startAddr, unsigned short regCount, unsigned char* buffer);

#endif
