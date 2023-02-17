#ifndef MODBUS_VARIABLE_H
#define	MODBUS_VARIABLE_H

typedef struct
{
	unsigned int*	VariablePointer;
	unsigned int	BitMask;
}
MBusVariable;

#define				MBusVariableInstance(var, bitMask)	((MBusVariable){.VariablePointer=var, .BitMask=bitMask})

#define				MBusVariableGet(mbusVar)			(*(mbusVar->VariablePointer) & mbusVar->BitMask)
#define				MBusVariableSet(mbusVar, value)		(*(mbusVar->VariablePointer) = (value & mbusVar->BitMask))

#define				MBusVariableMaskSet(mbusVar)		(*(mbusVar->VariablePointer) |= mbusVar->BitMask)
#define				MBusVariableMaskReset(mbusVar)		(*(mbusVar->VariablePointer) &=~(mbusVar->BitMask))

#endif
