#include "modbus_variable.h"
#include "modbus_registers.h"

#define				bitAddrToByteAddr(bitAddr) (bitAddr / 8)
#define				bitAddrToBitOffset(bitAddr) (bitAddr % 8)

MBusRegStatus		MBusRegAdd(MBusRegisterSet* regSet, unsigned short regAddr, unsigned int* variablePointer, unsigned int bitMask)
{
	if (regSet->RegistersCount >= MBUS_REG_MAX_REGISTERS_IN_REGSET) return MBUS_REG_ERR_UNEXPECTED;

	(regSet->Registers)[(regSet->RegistersCount++)] = MBusVariableInstance(variablePointer, bitMask);
}
MBusRegStatus		MBusRegRemove(MBusRegisterSet* regSet, unsigned short regAddr)
{
	for (int i = 0; i < regSet->RegistersCount; i++)
	{
		if (regSet->Registers[i].Address == regAddr)
		{
			i++;
			for ( ; i < regSet->RegistersCount; i++)
			{
				regSet->Registers[i - 1] = regSet->Registers[i];
			}
			regSet->RegistersCount--;

			return MBUS_REG_OK;
		}
	}

	return MBUS_INVALID_ADDRESS;
}

MBusRegStatus		MBusRegGet(MBusRegisterSet* regSet, unsigned short regAddr, MBusVariable* out)
{
	for (int i = 0; i < regSet->RegistersCount; i++)
		if (regSet->Registers[i] == regAddr)
		{
			out = &(regSet->Registers[i].Variable);
			return MBUS_REG_OK;
		}

	return MBUS_REG_ERR_INVALID_ADDRESS;
}

MBusRegStatus		MBusRegSetPack16bit(MBusRegisterSet* regSet, unsigned short startAddr, unsigned short regCount, unsigned char* buffer)
{
	if (startAddr >= 0xFFFF - regCount) return MBUS_REG_ERR_OUT_OF_BOUNDS;

	int byteCounter = 0;

	MBusVariable* varToPack;
	unsigned int varValue;

	for (int i = startAddr; i < startAddr + regCount; i++)
	{
		if (MBusRegGet(regSet, i, varToPack) == MBUS_REG_OK)
		{
			varValue = MBusVariableGet(varToPack);
			buffer[byteCounter] = varValue & 0x00FF;
			byteCounter++;
			buffer[byteCounter] = varValue & 0xFF00;
			byteCounter++;
		}
		else
		{
			return MBUS_REG_ERR_INVALID_ADDRESS;
		}
	}

	return MBUS_REG_OK;
}
MBusRegStatus		MBusRegSetUnpack16bit(MBusRegisterSet* regSet, unsigned short startAddr, unsigned short regCount, unsigned char* buffer)
{
	if (startAddr >= 0xFFFF - regCount) return MBUS_REG_ERR_OUT_OF_BOUNDS;

	int byteCounter = 0;

	MBusVariable* varToUnpack;
	unsigned int varValue;

	for (int i = startAddr; i < startAddr + regCount; i++)
	{
		if (MBusRegGet(regSet, i, varToUnpack) == MBUS_REG_OK)
		{
			varValue =	buffer[byteCounter] &	0x00FF;
			byteCounter++;
			varValue |=	buffer[byteCounter] <<	0xF;
			byteCounter++;
		}
		else
		{
			return MBUS_REG_ERR_INVALID_ADDRESS;
		}
	}

	return MBUS_REG_OK;
}

MBusRegStatus		MBusRegSetPack1bit(MBusRegisterSet* regSet, unsigned short startAddr, unsigned short regCount, unsigned char* buffer)
{
	if (startAddr >= 0xFFFF - regCount) return MBUS_REG_ERR_OUT_OF_BOUNDS;

	int bitCounter = 0;

	MBusVariable* varToUnpack;
	unsigned int varValue;

	for (int i = startAddr; i < startAddr + regCount; i++)
	{
		if (MBusRegGet(regSet, i, varToUnpack) == MBUS_REG_OK)
		{
		}
		else
		{
			return MBUS_REG_ERR_INVALID_ADDRESS;
		}
	}
}
MBusRegStatus		MBusRegSetUnpack1bit(MBusRegisterSet* regSet, unsigned short startAddr, unsigned short regCount, unsigned char* buffer)
{

}
