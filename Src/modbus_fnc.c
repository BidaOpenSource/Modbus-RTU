#include "modbus_global.h"
#include "modbus_fnc.h"

#include "modbus_map.h"

static MBusException ExceptionCodeAssign(MBusException exc, unsigned char* array, unsigned char* arrayLength)
{
	array[0] = (unsigned char)(exc);
	*arrayLength = 1;

	return exc;
}

// 0x01 | READ_COIL (figure 11. MODBUS Application Protocol Specification V1.1b)

static MBusException fncReadCoil(unsigned char* request, unsigned char requestLength, unsigned char* response, unsigned char* responseLength)
{
	if (requestLength != 4)
	{
		return ExceptionCodeAssign(MBUS_EXC_ILLEGAL_DATA_VALUE, response, responseLength);
	}

	unsigned short requestStartAddr = request[MBUS_JUNIOR_BIT_INDEX] | (request[MBUS_SENIOR_BIT_INDEX] << MBUS_BITS_IN_BYTE);
	unsigned short requestQuantityOfRegs = request[MBUS_JUNIOR_BIT_INDEX + 2] | (request[MBUS_SENIOR_BIT_INDEX + 2] << MBUS_BITS_IN_BYTE);

	if (requestStartAddr < 0x0000 || requestStartAddr > 0xFFFF)
	{
		return ExceptionCodeAssign(MBUS_EXC_ILLEGAL_DATA_ADDRESS, response, responseLength);
	}

	if (requestQuantityOfRegs < 1 || requestQuantityOfRegs > 0x07D0)
	{
		return ExceptionCodeAssign(MBUS_EXC_ILLEGAL_DATA_VALUE, response, responseLength);
	}

	MBusRegStatus regOpResult =	MBusRegSetPack1bit(&Coils, requestStartAddr, requestQuantityOfRegs, response);

	if (regOpResult == MBUS_REG_OK)
	{
		*responseLength = requestQuantityOfRegs / MBUS_BITS_IN_BYTE;
		if (requestQuantityOfRegs % MBUS_BITS_IN_BYTE != 0) *responseLength += 1;

		return MBUS_EXC_NONE;
	}

	switch (regOpResult)
	{
	case MBUS_REG_ERR_INVALID_VARIABLE: return ExceptionCodeAssign(MBUS_EXC_ILLEGAL_DATA_VALUE, response, responseLength);
	case MBUS_REG_ERR_INVALID_ADDRESS:
	case MBUS_REG_ERR_OUT_OF_BOUNDS:	return ExceptionCodeAssign(MBUS_EXC_ILLEGAL_DATA_ADDRESS, response, responseLength);
	default:							return ExceptionCodeAssign(MBUS_EXC_SLAVE_DEVICE_FAILURE, response, responseLength);
	}
}

// 0x02 | READ_DISCRETE_INPUTS (figure 12. MODBUS Application Protocol Specification V1.1b)

static MBusException fncReadDiscreteInputs(unsigned char* request, unsigned char requestLength, unsigned char* response, unsigned char* responseLength)
{
	if (requestLength != 4)
	{
		return ExceptionCodeAssign(MBUS_EXC_ILLEGAL_DATA_VALUE, response, responseLength);
	}

	unsigned short requestStartAddr = request[MBUS_JUNIOR_BIT_INDEX] | (request[MBUS_SENIOR_BIT_INDEX] << MBUS_BITS_IN_BYTE);
	unsigned short requestQuantityOfRegs = request[MBUS_JUNIOR_BIT_INDEX + 2] | (request[MBUS_SENIOR_BIT_INDEX + 2] << MBUS_BITS_IN_BYTE);

	if (requestStartAddr < 0x0000 || requestStartAddr > 0xFFFF)
	{
		return ExceptionCodeAssign(MBUS_EXC_ILLEGAL_DATA_ADDRESS, response, responseLength);
	}

	if (requestQuantityOfRegs < 1 || requestQuantityOfRegs > 0x07D0)
	{
		return ExceptionCodeAssign(MBUS_EXC_ILLEGAL_DATA_VALUE, response, responseLength);
	}

	MBusRegStatus regOpResult =	MBusRegSetPack1bit(&DiscreteInputs, requestStartAddr, requestQuantityOfRegs, response);

	if (regOpResult == MBUS_REG_OK)
	{
		*responseLength = requestQuantityOfRegs / MBUS_BITS_IN_BYTE;
		if (requestQuantityOfRegs % MBUS_BITS_IN_BYTE != 0) *responseLength += 1;

		return MBUS_EXC_NONE;
	}

	switch (regOpResult)
	{
	case MBUS_REG_ERR_INVALID_VARIABLE: return ExceptionCodeAssign(MBUS_EXC_ILLEGAL_DATA_VALUE, response, responseLength);
	case MBUS_REG_ERR_INVALID_ADDRESS:
	case MBUS_REG_ERR_OUT_OF_BOUNDS:	return ExceptionCodeAssign(MBUS_EXC_ILLEGAL_DATA_ADDRESS, response, responseLength);
	default:							return ExceptionCodeAssign(MBUS_EXC_SLAVE_DEVICE_FAILURE, response, responseLength);
	}
}

// 0x03 | READ_HOLDING_REGISTERS (figure 13. MODBUS Application Protocol Specification V1.1b)

static MBusException fncReadHoldingRegisters(unsigned char* request, unsigned char requestLength, unsigned char* response, unsigned char* responseLength)
{
	if (requestLength != 4)
	{
		return ExceptionCodeAssign(MBUS_EXC_ILLEGAL_DATA_VALUE, response, responseLength);
	}

	unsigned short requestStartAddr = request[MBUS_JUNIOR_BIT_INDEX] | (request[MBUS_SENIOR_BIT_INDEX] << MBUS_BITS_IN_BYTE);
	unsigned short requestQuantityOfRegs = request[MBUS_JUNIOR_BIT_INDEX + 2] | (request[MBUS_SENIOR_BIT_INDEX + 2] << MBUS_BITS_IN_BYTE);

	if (requestStartAddr < 0x0000 || requestStartAddr > 0xFFFF)
	{
		return ExceptionCodeAssign(MBUS_EXC_ILLEGAL_DATA_ADDRESS, response, responseLength);
	}

	if (requestQuantityOfRegs < 1 || requestQuantityOfRegs > 0x007D)
	{
		return ExceptionCodeAssign(MBUS_EXC_ILLEGAL_DATA_VALUE, response, responseLength);
	}

	MBusRegStatus regOpResult =	MBusRegSetPack16bit(&DiscreteInputs, requestStartAddr, requestQuantityOfRegs, response);

	if (regOpResult == MBUS_REG_OK)
	{
		*responseLength = requestQuantityOfRegs * MBUS_BYTES_IN_REG;

		return MBUS_EXC_NONE;
	}

	switch (regOpResult)
	{
	case MBUS_REG_ERR_INVALID_VARIABLE: return ExceptionCodeAssign(MBUS_EXC_ILLEGAL_DATA_VALUE, response, responseLength);
	case MBUS_REG_ERR_INVALID_ADDRESS:
	case MBUS_REG_ERR_OUT_OF_BOUNDS:	return ExceptionCodeAssign(MBUS_EXC_ILLEGAL_DATA_ADDRESS, response, responseLength);
	default:							return ExceptionCodeAssign(MBUS_EXC_SLAVE_DEVICE_FAILURE, response, responseLength);
	}
}

// 0x04 | READ_INPUT_REGISTERS (figure 14. MODBUS Application Protocol Specification V1.1b)

static MBusException fncReadInputRegisters(unsigned char* request, unsigned char requestLength, unsigned char* response, unsigned char* responseLength)
{
	if (requestLength != 4)
	{
		return ExceptionCodeAssign(MBUS_EXC_ILLEGAL_DATA_VALUE, response, responseLength);
	}

	unsigned short requestStartAddr = request[MBUS_JUNIOR_BIT_INDEX] | (request[MBUS_SENIOR_BIT_INDEX] << MBUS_BITS_IN_BYTE);
	unsigned short requestQuantityOfRegs = request[MBUS_JUNIOR_BIT_INDEX + 2] | (request[MBUS_SENIOR_BIT_INDEX + 2] << MBUS_BITS_IN_BYTE);

	if (requestStartAddr < 0x0000 || requestStartAddr > 0xFFFF)
	{
		return ExceptionCodeAssign(MBUS_EXC_ILLEGAL_DATA_ADDRESS, response, responseLength);
	}

	if (requestQuantityOfRegs < 1 || requestQuantityOfRegs > 0x007D)
	{
		return ExceptionCodeAssign(MBUS_EXC_ILLEGAL_DATA_VALUE, response, responseLength);
	}

	MBusRegStatus regOpResult =	MBusRegSetPack16bit(&HoldingRegisters, requestStartAddr, requestQuantityOfRegs, response);

	if (regOpResult == MBUS_REG_OK)
	{
		*responseLength = requestQuantityOfRegs * MBUS_BYTES_IN_REG;

		return MBUS_EXC_NONE;
	}

	switch (regOpResult)
	{
	case MBUS_REG_ERR_INVALID_VARIABLE: return ExceptionCodeAssign(MBUS_EXC_ILLEGAL_DATA_VALUE, response, responseLength);
	case MBUS_REG_ERR_INVALID_ADDRESS:
	case MBUS_REG_ERR_OUT_OF_BOUNDS:	return ExceptionCodeAssign(MBUS_EXC_ILLEGAL_DATA_ADDRESS, response, responseLength);
	default:							return ExceptionCodeAssign(MBUS_EXC_SLAVE_DEVICE_FAILURE, response, responseLength);
	}
}

//

MBusFunction MBusFunctions[MBUS_FNC_COUNT] =
{
	{},
	{ &fncReadCoil },							// 0x01
	{ &fncReadDiscreteInputs },					// 0x02
	{ &fncReadHoldingRegisters },				// 0x03
	{ &fncReadInputRegisters }					// 0x04
};
