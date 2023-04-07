#include "modbus_registers.h"
#include "modbus_map.h"

MBusRegister RegisterCoils[MBUS_MAP_COILS_COUNT];
MBusRegisterSet Coils =
{
	.IsSorted = 0,
	.Registers = RegisterCoils,
	.RegistersCount = 0,
	.RegistersLimit = MBUS_MAP_COILS_COUNT
};

MBusRegister RegisterDiscreteInputs[MBUS_MAP_DISCRETE_INPUTS_COUNT];
MBusRegisterSet DiscreteInputs =
{
	.IsSorted = 0,
	.Registers = RegisterDiscreteInputs,
	.RegistersCount = 0,
	.RegistersLimit = MBUS_MAP_DISCRETE_INPUTS_COUNT
};

MBusRegister	RegisterInputRegisters[MBUS_MAP_INPUT_REGISTERS_COUNT];
MBusRegisterSet InputRegisters =
{
	.IsSorted = 0,
	.Registers = RegisterInputRegisters,
	.RegistersCount = 0,
	.RegistersLimit = MBUS_MAP_INPUT_REGISTERS_COUNT
};

MBusRegister	RegisterHoldingRegisters[MBUS_MAP_HOLDING_REGISTERS_COUNT];
MBusRegisterSet HoldingRegisters =
{
	.IsSorted = 0,
	.Registers = RegisterHoldingRegisters,
	.RegistersCount = 0,
	.RegistersLimit = MBUS_MAP_HOLDING_REGISTERS_COUNT
};

void	MBusMapInit()
{
	MBusRegsetSort(&Coils);
	MBusRegsetSort(&DiscreteInputs);
	MBusRegsetSort(&InputRegisters);
	MBusRegsetSort(&HoldingRegisters);
}
