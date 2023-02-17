#include "modbus_variable.h"
#include "modbus_registers.h"

#define				bitAddrToByteAddr(bitAddr) (bitAddr / 8)
#define				bitAddrToBitOffset(bitAddr) (bitAddr % 8)

MBusRegStatus		MBusRegGet(MBusRegisterSet* regSet, unsigned short regAddr, MBusRegister** out)
{
	MBusRegister* regRef = 0;

	for (int i = 0; i < regSet->RegistersCount; i++)
	{
		regRef = &((regSet->Registers)[i]);

		if (regRef->Address == regAddr)
		{
			*out = regRef;
			return MBUS_REG_OK;
		}
	}

	return MBUS_REG_ERR_INVALID_ADDRESS;
}
MBusRegStatus		MBusVarGet(MBusRegisterSet* regSet, unsigned short regAddr, MBusVariable** out)
{
	MBusRegister* regRef = 0;

	if (MBusRegGet(regSet, regAddr, &regRef) != MBUS_REG_OK) return MBUS_REG_ERR_INVALID_ADDRESS;

	*out = &(regRef->Variable);

	return MBUS_REG_OK;
}

MBusRegStatus		MBusRegAdd(MBusRegisterSet* regSet, unsigned short regAddr, unsigned int* variablePointer, unsigned int bitMask)
{
	if (regSet->RegistersCount >= MBUS_REG_MAX_REGISTERS_IN_REGSET) return MBUS_REG_ERR_UNEXPECTED;

	MBusRegister* regRef = (MBusRegister*)(&(regSet->Registers[(regSet->RegistersCount)++]));
	regRef->Address = regAddr;

	MBusVariable* varRef = (MBusVariable*)(&(regRef->Variable));
	*varRef = MBusVariableInstance(variablePointer, bitMask);

	return MBUS_REG_OK;
}
MBusRegStatus		MBusRegRemove(MBusRegisterSet* regSet, unsigned short regAddr)
{
	MBusRegister regRef;

	for (int i = 0; i < regSet->RegistersCount; i++)
	{
		regRef = ((MBusRegister*)((regSet->Registers)))[i];

		if (regRef.Address == regAddr)
		{
			i++;
			for (; i < regSet->RegistersCount; i++)
			{
				regSet->Registers[i - 1] = regSet->Registers[i];
			}
			return MBUS_REG_OK;
		}
	}

	return MBUS_REG_ERR_INVALID_VARIABLE;
}

MBusRegStatus		MBusRegSetPack16bit(MBusRegisterSet* regSet, unsigned short startAddr, unsigned short regCount, unsigned char* buffer)
{
	if (startAddr >= 0xFFFF - regCount) return MBUS_REG_ERR_OUT_OF_BOUNDS;

	int byteCounter = 0;

	MBusVariable* varToPack = 0;
	unsigned int varValue;

	for (int i = startAddr; i < startAddr + regCount; i++)
	{
		if (MBusVarGet(regSet, i, &varToPack) == MBUS_REG_OK)
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

	MBusVariable* varToUnpack = 0;
	unsigned int varValue;

	for (int i = startAddr; i < startAddr + regCount; i++)
	{
		if (MBusVarGet(regSet, i, &varToUnpack) == MBUS_REG_OK)
		{
			varValue =	buffer[byteCounter] &	0x00FF;
			byteCounter++;
			varValue |=	buffer[byteCounter] <<	0xF;
			byteCounter++;

			MBusVariableSet(varToUnpack, varValue);
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

	MBusVariable* varToPack = 0;
	unsigned int varValue;

	for (int i = startAddr; i < startAddr + regCount; i++)
	{
		if (MBusVarGet(regSet, i, &varToPack) == MBUS_REG_OK)
		{
			varValue = MBusVariableGet(varToPack) ?		1 : 0;
			buffer[bitAddrToByteAddr(bitCounter)] &=~	(1 << bitAddrToBitOffset(bitCounter));
			buffer[bitAddrToByteAddr(bitCounter)] |=	(varValue << bitAddrToBitOffset(bitCounter));
		}
		else
		{
			return MBUS_REG_ERR_INVALID_ADDRESS;
		}

		bitCounter++;
	}

	return MBUS_REG_OK;
}
MBusRegStatus		MBusRegSetUnpack1bit(MBusRegisterSet* regSet, unsigned short startAddr, unsigned short regCount, unsigned char* buffer)
{
	if (startAddr >= 0xFFFF - regCount) return MBUS_REG_ERR_OUT_OF_BOUNDS;

	int bitCounter = 0;

	MBusVariable* varToUnpack = 0;
	unsigned int varValue;

	for (int i = startAddr; i < startAddr + regCount; i++)
	{
		if (MBusVarGet(regSet, i, &varToUnpack) == MBUS_REG_OK)
		{
			varValue = buffer[bitAddrToByteAddr(bitCounter)] & (1 << bitAddrToBitOffset(bitCounter));

			if (varValue)
				MBusVariableMaskSet(varToUnpack);
			else
				MBusVariableMaskReset(varToUnpack);
		}
		else
		{
			return MBUS_REG_ERR_INVALID_ADDRESS;
		}

		bitCounter++;
	}

	return MBUS_REG_OK;
}

MBusRegisterSet mrs;
void				ModbusRegistersTest()
{

	unsigned int a = 1, b = 0, c = 0, d = 1;

	MBusRegAdd(&mrs, 1, &a, 0x1);
	MBusRegAdd(&mrs, 2, &b, 0x1);
	MBusRegAdd(&mrs, 3, &c, 0x1);
	MBusRegAdd(&mrs, 4, &d, 0x1);

	//MBusRegRemove(&mrs, 3);
	//MBusRegRemove(&mrs, 3);

	unsigned char x[10];

	MBusRegSetPack1bit(&mrs, 1, 4, x);
	MBusRegSetUnpack1bit(&mrs, 1, 4, x);
}
