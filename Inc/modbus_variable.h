#ifndef MODBUS_VARIABLE_H
#define	MODBUS_VARIABLE_H

typedef struct
{
	unsigned int*	VariablePointer;
	unsigned int	BitMask;
}
MBusVariable;

#define				MBusVariableInstance(var, bitMask)	((ModbusVariable){.VariablePointer=var, .BitMask=bitMask})

#define				MBusVariableGet(mbusVar)			(*(mbusVar->VariablePointer) & mbusVar->BitMask)
#define				MBusVariableSet(mbusVar, value)		(*(mbusVar->VariablePointer) = value)

#endif
