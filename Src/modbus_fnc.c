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

MBusException			FncReadCoilRequest(unsigned short startAddress, unsigned short quantityOfRegs, unsigned char* requestData, unsigned char* requestDataLength)
{
	*requestDataLength = 4;

	requestData[0] = startAddress & 0xFF;
	requestData[1] = (startAddress >> MBUS_BITS_IN_BYTE) & 0xFF;

	requestData[2] = quantityOfRegs & 0xFF;
	requestData[3] = (quantityOfRegs >> MBUS_BITS_IN_BYTE) & 0xFF;

	if (startAddress < 0x0000 || 0xFFFF < startAddress) return MBUS_EXC_ILLEGAL_DATA_ADDRESS;
	if (quantityOfRegs < 1 || 0x07D0 < quantityOfRegs) return MBUS_EXC_ILLEGAL_DATA_VALUE;

	return MBUS_EXC_NONE;
}
static MBusException	fncReadCoilResponse(unsigned char* request, unsigned char requestLength, unsigned char* response, unsigned char* responseLength)
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

MBusException			FncReadDiscreteInputsRequest(unsigned short startAddress, unsigned short quantityOfRegs, unsigned char* requestData, unsigned char* requestDataLength)
{
	*requestDataLength = 4;

	requestData[0] = startAddress & 0xFF;
	requestData[1] = (startAddress >> MBUS_BITS_IN_BYTE) & 0xFF;

	requestData[2] = quantityOfRegs & 0xFF;
	requestData[3] = (quantityOfRegs >> MBUS_BITS_IN_BYTE) & 0xFF;

	if (startAddress < 0x0000 || 0xFFFF < startAddress) return MBUS_EXC_ILLEGAL_DATA_ADDRESS;
	if (quantityOfRegs < 1 || 0x07D0 < quantityOfRegs) return MBUS_EXC_ILLEGAL_DATA_VALUE;

	return MBUS_EXC_NONE;
}
static MBusException	fncReadDiscreteInputsResponse(unsigned char* request, unsigned char requestLength, unsigned char* response, unsigned char* responseLength)
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

MBusException			FncReadHoldingRegistersRequest(unsigned short startAddress, unsigned short quantityOfRegs, unsigned char* requestData, unsigned char* requestDataLength)
{
	*requestDataLength = 4;

	requestData[0] = startAddress & 0xFF;
	requestData[1] = (startAddress >> MBUS_BITS_IN_BYTE) & 0xFF;

	requestData[2] = quantityOfRegs & 0xFF;
	requestData[3] = (quantityOfRegs >> MBUS_BITS_IN_BYTE) & 0xFF;

	if (startAddress < 0x0000 || 0xFFFF < startAddress) return MBUS_EXC_ILLEGAL_DATA_ADDRESS;
	if (quantityOfRegs < 1 || 0x007D < quantityOfRegs) return MBUS_EXC_ILLEGAL_DATA_VALUE;

	return MBUS_EXC_NONE;
}
static MBusException	fncReadHoldingRegistersResponse(unsigned char* request, unsigned char requestLength, unsigned char* response, unsigned char* responseLength)
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

MBusException			FncReadInputRegistersRequest(unsigned short startAddress, unsigned short quantityOfRegs, unsigned char* requestData, unsigned char* requestDataLength)
{
	*requestDataLength = 4;

	requestData[0] = startAddress & 0xFF;
	requestData[1] = (startAddress >> MBUS_BITS_IN_BYTE) & 0xFF;

	requestData[2] = quantityOfRegs & 0xFF;
	requestData[3] = (quantityOfRegs >> MBUS_BITS_IN_BYTE) & 0xFF;

	if (startAddress < 0x0000 || 0xFFFF < startAddress) return MBUS_EXC_ILLEGAL_DATA_ADDRESS;
	if (quantityOfRegs < 1 || 0x007D < quantityOfRegs) return MBUS_EXC_ILLEGAL_DATA_VALUE;

	return MBUS_EXC_NONE;
}
static MBusException	fncReadInputRegistersResponse(unsigned char* request, unsigned char requestLength, unsigned char* response, unsigned char* responseLength)
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

// 0x05 | WRITE_SINGLE_COIL (figure 15. MODBUS Application Protocol Specification V1.1b)

MBusException			FncWriteSingleCoilRequest(unsigned short address, unsigned short value, unsigned char* requestData, unsigned char* requestDataLength)
{
	*requestDataLength = 4;

	requestData[0] = address & 0xFF;
	requestData[1] = (address >> MBUS_BITS_IN_BYTE) & 0xFF;

	requestData[2] = 0x00;
	requestData[3] = value ? 0xFF : 0x00;

	if (address < 0x0000 || 0xFFFF < address) return MBUS_EXC_ILLEGAL_DATA_ADDRESS;

	return MBUS_EXC_NONE;
}
static MBusException	fncWriteSingleCoilResponse(unsigned char* request, unsigned char requestLength, unsigned char* response, unsigned char* responseLength)
{
	if (requestLength != 4)
	{
		return ExceptionCodeAssign(MBUS_EXC_ILLEGAL_DATA_VALUE, response, responseLength);
	}

	unsigned short requestAddr = request[MBUS_JUNIOR_BIT_INDEX] | (request[MBUS_SENIOR_BIT_INDEX] << MBUS_BITS_IN_BYTE);

	MBusRegStatus result = MBusRegSetUnpack1bit(&Coils, requestAddr, 1, &request[2]);
	if (result == MBUS_REG_OK) return MBUS_EXC_NONE;

	return ExceptionCodeAssign(MBUS_EXC_ILLEGAL_DATA_ADDRESS, response, responseLength);
}

// 0x06 | WRITE_SINGLE_REGISTER (figure 16. MODBUS Application Protocol Specification V1.1b)

MBusException			FncWriteSingleRegisterRequest(unsigned short address, unsigned short value, unsigned char* requestData, unsigned char* requestDataLength)
{
	*requestDataLength = 4;

	requestData[0] = address & 0xFF;
	requestData[1] = (address >> MBUS_BITS_IN_BYTE) & 0xFF;

	requestData[2] = value & 0xFF;
	requestData[3] = (value >> MBUS_BITS_IN_BYTE) & 0xFF;

	if (address < 0x0000 || 0xFFFF < address) return MBUS_EXC_ILLEGAL_DATA_ADDRESS;

	return MBUS_EXC_NONE;
}
static MBusException	fncWriteSingleRegisterResponse(unsigned char* request, unsigned char requestLength, unsigned char* response, unsigned char* responseLength)
{
	if (requestLength != 4)
	{
		return ExceptionCodeAssign(MBUS_EXC_ILLEGAL_DATA_VALUE, response, responseLength);
	}

	unsigned short requestAddr = request[MBUS_JUNIOR_BIT_INDEX] | (request[MBUS_SENIOR_BIT_INDEX] << MBUS_BITS_IN_BYTE);

	MBusRegStatus result = MBusRegSetUnpack16bit(&Coils, requestAddr, 1, &request[2]);
	if (result == MBUS_REG_OK) return MBUS_EXC_NONE;

	return ExceptionCodeAssign(MBUS_EXC_ILLEGAL_DATA_ADDRESS, response, responseLength);
}


MBusFunction MBusFunctions[MBUS_FNC_COUNT] =
{
	{},
	{ &fncReadCoilResponse },							// 0x01
	{ &fncReadDiscreteInputsResponse },					// 0x02
	{ &fncReadHoldingRegistersResponse },				// 0x03
	{ &fncReadInputRegistersResponse },					// 0x04
	{ &fncWriteSingleCoilResponse },					// 0x05
	{ &fncWriteSingleRegisterResponse },				// 0x06
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{},
	{}
};
