#ifndef MODBUS_MAP_H
#define MODBUS_MAP_H

#include "modbus_registers.h"

#define MBUS_MAP_COILS_COUNT				10
#define MBUS_MAP_DISCRETE_INPUTS_COUNT		50
#define MBUS_MAP_INPUT_REGISTERS_COUNT		100
#define MBUS_MAP_HOLDING_REGISTERS_COUNT	100

extern	MBusRegisterSet Coils;
extern	MBusRegisterSet DiscreteInputs;
extern	MBusRegisterSet InputRegisters;
extern	MBusRegisterSet HoldingRegisters;

void	MBusMapInit();

#endif
