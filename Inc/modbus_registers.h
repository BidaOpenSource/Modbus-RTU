#ifndef MODBUS_REGISTERS_H
#define MODBUS_REGISTERS_H

#define	MODBUS_REGISTERS_ENABLED

#include "modbus_variable.h"

typedef enum
{
	MBUS_REG_OK,
	MBUS_REG_ERR_UNEXPECTED,
	MBUS_REG_ERR_ALREADY_EXISTS,
	MBUS_REG_ERR_INVALID_VARIABLE,
	MBUS_REG_ERR_INVALID_ADDRESS,
	MBUS_REG_ERR_OUT_OF_BOUNDS,
	MBUS_REG_ERR_OVERFLOW,
	MBUS_REG_ERR_UNSORTED
}
MBusRegStatus;

typedef struct
{
	MBusVariable	Variable;
	unsigned short	Address;
}
MBusRegister;

typedef struct
{
	unsigned int	IsSorted;
	unsigned short	RegistersCount;
	unsigned short	RegistersLimit;
	MBusRegister* 	Registers;
}
MBusRegisterSet;

void				MBusRegsetSort(MBusRegisterSet* regSet);

MBusRegStatus		MBusRegAdd(MBusRegisterSet* regSet, unsigned short regAddr, unsigned int* variablePointer, unsigned int bitMask);
MBusRegStatus		MBusRegRemove(MBusRegisterSet* regSet, unsigned short regAddr);
MBusRegStatus		MBusRegAddFloat(MBusRegisterSet* regSet, unsigned short regAddr, float* variablePointer);

MBusRegStatus		MBusVarGet(MBusRegisterSet* regSet, unsigned short regAddr, MBusVariable** out);

MBusRegStatus		MBusRegSetPack16bit(MBusRegisterSet* regSet, unsigned short startAddr, unsigned short regCount, unsigned char* buffer);
MBusRegStatus		MBusRegSetUnpack16bit(MBusRegisterSet* regSet, unsigned short startAddr, unsigned short regCount, unsigned char* buffer);

MBusRegStatus		MBusRegSetPack1bit(MBusRegisterSet* regSet, unsigned short startAddr, unsigned short regCount, unsigned char* buffer);
MBusRegStatus		MBusRegSetUnpack1bit(MBusRegisterSet* regSet, unsigned short startAddr, unsigned short regCount, unsigned char* buffer);

#endif
