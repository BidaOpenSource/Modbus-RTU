#include "modbus_global.h"
#include "modbus_fnc.h"

#include "modbus_map.h"

MBusException 			ExceptionCodeAssign(MBusException exc, unsigned char* array, unsigned char* arrayLength)
{
	array[0] = (unsigned char)(exc);
	*arrayLength = 1;

	return exc;
}

// 0x01 | READ_COILS (figure 11. MODBUS Application Protocol Specification V1.1b)

static MBusException	fncReadCoilsGenerateRequest(unsigned short* arguments, unsigned char* requestData, unsigned char* requestDataLength)
{
	unsigned short startAddress = arguments[0];
	unsigned short quantityOfRegs = arguments[1];

	*requestDataLength = 4;

	requestData[0] = startAddress & 0xFF;
	requestData[1] = (startAddress >> MBUS_BITS_IN_BYTE) & 0xFF;

	requestData[2] = quantityOfRegs & 0xFF;
	requestData[3] = (quantityOfRegs >> MBUS_BITS_IN_BYTE) & 0xFF;

	if (startAddress < 0x0000 || 0xFFFF < startAddress) return MBUS_EXC_ILLEGAL_DATA_ADDRESS;
	if (quantityOfRegs < 1 || 0x07D0 < quantityOfRegs) return MBUS_EXC_ILLEGAL_DATA_VALUE;

	return MBUS_EXC_NONE;
}
static MBusException	fncReadCoilsProcessRequest(unsigned char* request, unsigned char requestLength, unsigned char* response, unsigned char* responseLength)
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
static MBusException	fncReadCoilsProcessResponse(unsigned char* request, unsigned char* response, unsigned char responseLength)
{
	if (responseLength < 2)
	{
		return MBUS_EXC_ILLEGAL_DATA_VALUE;
	}

	unsigned short startAddr = request[MBUS_JUNIOR_BIT_INDEX] | (request[MBUS_SENIOR_BIT_INDEX] << MBUS_SENIOR_BIT_SHIFT);
	unsigned short regCount = request[2 + MBUS_JUNIOR_BIT_INDEX] | (request[2 + MBUS_SENIOR_BIT_INDEX] << MBUS_SENIOR_BIT_SHIFT);

	if (responseLength * MBUS_BITS_IN_BYTE < regCount)
	{
		regCount = responseLength * MBUS_BITS_IN_BYTE;
	}

	if (MBusRegSetUnpack1bit(&Coils, startAddr, regCount, &(response[1])) != MBUS_REG_OK)
	{
		return MBUS_EXC_ILLEGAL_DATA_VALUE;
	}

	return MBUS_REG_OK;
}

// 0x02 | READ_DISCRETE_INPUTS (figure 12. MODBUS Application Protocol Specification V1.1b)

static MBusException	fncReadDiscreteInputsGenerateRequest(unsigned short* arguments, unsigned char* requestData, unsigned char* requestDataLength)
{
	unsigned short startAddress = arguments[0];
	unsigned short quantityOfRegs = arguments[1];

	*requestDataLength = 4;

	requestData[0] = startAddress & 0xFF;
	requestData[1] = (startAddress >> MBUS_BITS_IN_BYTE) & 0xFF;

	requestData[2] = quantityOfRegs & 0xFF;
	requestData[3] = (quantityOfRegs >> MBUS_BITS_IN_BYTE) & 0xFF;

	if (startAddress < 0x0000 || 0xFFFF < startAddress) return MBUS_EXC_ILLEGAL_DATA_ADDRESS;
	if (quantityOfRegs < 1 || 0x07D0 < quantityOfRegs) return MBUS_EXC_ILLEGAL_DATA_VALUE;

	return MBUS_EXC_NONE;
}
static MBusException	fncReadDiscreteInputsProcessRequest(unsigned char* request, unsigned char requestLength, unsigned char* response, unsigned char* responseLength)
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
static MBusException	fncReadDiscreteInputsProcessResponse(unsigned char* request, unsigned char* response, unsigned char responseLength)
{
	if (responseLength < 1)
	{
		return MBUS_EXC_ILLEGAL_DATA_VALUE;
	}

	unsigned short startAddr = request[MBUS_JUNIOR_BIT_INDEX] | (request[MBUS_SENIOR_BIT_INDEX] << MBUS_SENIOR_BIT_SHIFT);
	unsigned short regCount = request[2 + MBUS_JUNIOR_BIT_INDEX] | (request[2 + MBUS_SENIOR_BIT_INDEX] << MBUS_SENIOR_BIT_SHIFT);

	if (responseLength * MBUS_BITS_IN_BYTE < regCount)
	{
		regCount = responseLength * MBUS_BITS_IN_BYTE;
	}

	if (MBusRegSetUnpack1bit(&DiscreteInputs, startAddr, regCount, &(response[1])) != MBUS_REG_OK)
	{
		return MBUS_EXC_ILLEGAL_DATA_VALUE;
	}

	return MBUS_REG_OK;
}

// 0x03 | READ_HOLDING_REGISTERS (figure 13. MODBUS Application Protocol Specification V1.1b)

static MBusException	fncReadHoldingRegistersGenerateRequest(unsigned short* arguments, unsigned char* requestData, unsigned char* requestDataLength)
{
	unsigned short startAddress = arguments[0];
	unsigned short quantityOfRegs = arguments[1];

	*requestDataLength = 4;

	requestData[0] = startAddress & 0xFF;
	requestData[1] = (startAddress >> MBUS_BITS_IN_BYTE) & 0xFF;

	requestData[2] = quantityOfRegs & 0xFF;
	requestData[3] = (quantityOfRegs >> MBUS_BITS_IN_BYTE) & 0xFF;

	if (startAddress < 0x0000 || 0xFFFF < startAddress) return MBUS_EXC_ILLEGAL_DATA_ADDRESS;
	if (quantityOfRegs < 1 || 0x007D < quantityOfRegs) return MBUS_EXC_ILLEGAL_DATA_VALUE;

	return MBUS_EXC_NONE;
}
static MBusException	fncReadHoldingRegistersProcessRequest(unsigned char* request, unsigned char requestLength, unsigned char* response, unsigned char* responseLength)
{
	// tested
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

	MBusRegStatus regOpResult =	MBusRegSetPack16bit(&HoldingRegisters, requestStartAddr, requestQuantityOfRegs, &(response[1]));

	if (regOpResult == MBUS_REG_OK)
	{
		response[0] = requestQuantityOfRegs * 2;
		*responseLength = requestQuantityOfRegs * MBUS_BYTES_IN_REG + 1;

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
static MBusException	fncReadHoldingRegistersProcessResponse(unsigned char* request, unsigned char* response, unsigned char responseLength)
{
	if (responseLength < 2)
	{
		return MBUS_EXC_ILLEGAL_DATA_VALUE;
	}

	unsigned short startAddr = request[MBUS_JUNIOR_BIT_INDEX] | (request[MBUS_SENIOR_BIT_INDEX] << MBUS_SENIOR_BIT_SHIFT);
	unsigned short regCount = request[2 + MBUS_JUNIOR_BIT_INDEX] | (request[2 + MBUS_SENIOR_BIT_INDEX] << MBUS_SENIOR_BIT_SHIFT);

	if (responseLength / MBUS_BYTES_IN_REG < regCount)
	{
		regCount = responseLength / MBUS_BYTES_IN_REG;
	}

	if (MBusRegSetUnpack16bit(&HoldingRegisters, startAddr, regCount, &(response[1])) != MBUS_REG_OK)
	{
		return MBUS_EXC_ILLEGAL_DATA_VALUE;
	}

	return MBUS_REG_OK;
}

// 0x04 | READ_INPUT_REGISTERS (figure 14. MODBUS Application Protocol Specification V1.1b)

static MBusException	fncReadInputRegistersGenerateRequest(unsigned short* arguments, unsigned char* requestData, unsigned char* requestDataLength)
{
	unsigned short startAddress = arguments[0];
	unsigned short quantityOfRegs = arguments[1];

	*requestDataLength = 4;

	requestData[0] = startAddress & 0xFF;
	requestData[1] = (startAddress >> MBUS_BITS_IN_BYTE) & 0xFF;

	requestData[2] = quantityOfRegs & 0xFF;
	requestData[3] = (quantityOfRegs >> MBUS_BITS_IN_BYTE) & 0xFF;

	if (startAddress < 0x0000 || 0xFFFF < startAddress) return MBUS_EXC_ILLEGAL_DATA_ADDRESS;
	if (quantityOfRegs < 1 || 0x007D < quantityOfRegs) return MBUS_EXC_ILLEGAL_DATA_VALUE;

	return MBUS_EXC_NONE;
}
static MBusException	fncReadInputRegistersProcessRequest(unsigned char* request, unsigned char requestLength, unsigned char* response, unsigned char* responseLength)
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

	MBusRegStatus regOpResult =	MBusRegSetPack16bit(&InputRegisters, requestStartAddr, requestQuantityOfRegs, response);

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
static MBusException	fncReadInputRegistersProcessResponse(unsigned char* request, unsigned char* response, unsigned char responseLength)
{
	if (responseLength < 2)
	{
		return MBUS_EXC_ILLEGAL_DATA_VALUE;
	}

	unsigned short startAddr = request[MBUS_JUNIOR_BIT_INDEX] | (request[MBUS_SENIOR_BIT_INDEX] << MBUS_SENIOR_BIT_SHIFT);
	unsigned short regCount = request[2 + MBUS_JUNIOR_BIT_INDEX] | (request[2 + MBUS_SENIOR_BIT_INDEX] << MBUS_SENIOR_BIT_SHIFT);

	if (responseLength / MBUS_BYTES_IN_REG < regCount)
	{
		regCount = responseLength / MBUS_BYTES_IN_REG;
	}

	if (MBusRegSetUnpack16bit(&InputRegisters, startAddr, regCount, &(response[1])) != MBUS_REG_OK)
	{
		return MBUS_EXC_ILLEGAL_DATA_VALUE;
	}

	return MBUS_REG_OK;
}

// 0x05 | WRITE_SINGLE_COIL (figure 15. MODBUS Application Protocol Specification V1.1b)

static MBusException	fncWriteSingleCoilGenerateRequest(unsigned short* arguments, unsigned char* requestData, unsigned char* requestDataLength)
{
	unsigned short address = arguments[0];
	unsigned short value = arguments[1];

	*requestDataLength = 4;

	requestData[0] = address & 0xFF;
	requestData[1] = (address >> MBUS_BITS_IN_BYTE) & 0xFF;

	requestData[2] = 0x00;
	requestData[3] = value ? 0xFF : 0x00;

	if (address < 0x0000 || 0xFFFF < address) return MBUS_EXC_ILLEGAL_DATA_ADDRESS;

	return MBUS_EXC_NONE;
}
static MBusException	fncWriteSingleCoilProcessRequest(unsigned char* request, unsigned char requestLength, unsigned char* response, unsigned char* responseLength)
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
static MBusException	fncWriteSingleCoilProcessResponse(unsigned char* request, unsigned char* response, unsigned char responseLength)
{
	if (responseLength < 2)
	{
		return MBUS_EXC_ILLEGAL_DATA_VALUE;
	}

	unsigned short startAddr = request[MBUS_JUNIOR_BIT_INDEX] | (request[MBUS_SENIOR_BIT_INDEX] << MBUS_SENIOR_BIT_SHIFT);
	unsigned short regCount = request[2 + MBUS_JUNIOR_BIT_INDEX] | (request[2 + MBUS_SENIOR_BIT_INDEX] << MBUS_SENIOR_BIT_SHIFT);

	unsigned short startAddrR = response[MBUS_JUNIOR_BIT_INDEX] | (response[MBUS_SENIOR_BIT_INDEX] << MBUS_SENIOR_BIT_SHIFT);
	unsigned short regCountR = response[2 + MBUS_JUNIOR_BIT_INDEX] | (response[2 + MBUS_SENIOR_BIT_INDEX] << MBUS_SENIOR_BIT_SHIFT);


	if (startAddr != startAddrR || regCount != regCountR)
	{
		return MBUS_EXC_ILLEGAL_DATA_VALUE;
	}

	return MBUS_REG_OK;
}


// 0x06 | WRITE_SINGLE_REGISTER (figure 16. MODBUS Application Protocol Specification V1.1b)

static MBusException	fncWriteSingleRegisterGenerateRequest(unsigned short* arguments, unsigned char* requestData, unsigned char* requestDataLength)
{
	unsigned short address = arguments[0];
	unsigned short value = arguments[1];

	*requestDataLength = 4;

	requestData[0] = (address >> MBUS_SENIOR_BIT_SHIFT) & 0xFF;
	requestData[1] = (address >> MBUS_JUNIOR_BIT_SHIFT) & 0xFF;

	requestData[2] = (value >> MBUS_SENIOR_BIT_SHIFT) & 0xFF;
	requestData[3] = (value >> MBUS_JUNIOR_BIT_SHIFT) & 0xFF;

	if (address < 0x0000 || 0xFFFF < address) return MBUS_EXC_ILLEGAL_DATA_ADDRESS;

	return MBUS_EXC_NONE;
}
static MBusException	fncWriteSingleRegisterProcessRequest(unsigned char* request, unsigned char requestLength, unsigned char* response, unsigned char* responseLength)
{
	// tested
	if (requestLength != 4)
	{
		return ExceptionCodeAssign(MBUS_EXC_ILLEGAL_DATA_VALUE, response, responseLength);
	}

	unsigned short requestAddr = request[MBUS_JUNIOR_BIT_INDEX] | (request[MBUS_SENIOR_BIT_INDEX] << MBUS_BITS_IN_BYTE);

	MBusRegStatus result = MBusRegSetUnpack16bit(&HoldingRegisters, requestAddr, 1, &request[2]);

	for (int i = 0; i < 4; i++) response[i] = request[i];
	*responseLength = 4;

	if (result == MBUS_REG_OK) return MBUS_EXC_NONE;

	return ExceptionCodeAssign(MBUS_EXC_ILLEGAL_DATA_ADDRESS, response, responseLength);
}
static MBusException	fncWriteSingleRegisterProcessResponse(unsigned char* request, unsigned char* response, unsigned char responseLength)
{
	if (responseLength < 2)
	{
		return MBUS_EXC_ILLEGAL_DATA_VALUE;
	}

	unsigned short startAddr = request[MBUS_JUNIOR_BIT_INDEX] | (request[MBUS_SENIOR_BIT_INDEX] << MBUS_SENIOR_BIT_SHIFT);
	unsigned short regCount = request[2 + MBUS_JUNIOR_BIT_INDEX] | (request[2 + MBUS_SENIOR_BIT_INDEX] << MBUS_SENIOR_BIT_SHIFT);

	unsigned short startAddrR = response[MBUS_JUNIOR_BIT_INDEX] | (response[MBUS_SENIOR_BIT_INDEX] << MBUS_SENIOR_BIT_SHIFT);
	unsigned short regCountR = response[2 + MBUS_JUNIOR_BIT_INDEX] | (response[2 + MBUS_SENIOR_BIT_INDEX] << MBUS_SENIOR_BIT_SHIFT);


	if (startAddr != startAddrR || regCount != regCountR)
	{
		return MBUS_EXC_ILLEGAL_DATA_VALUE;
	}

	return MBUS_REG_OK;
}

MBusFunction MBusFunctions[MBUS_FNC_COUNT] =
{
	{ 0, 0, 0, 0 },
	{ MBUS_FNC_ENABLED, &fncReadCoilsGenerateRequest,			&fncReadCoilsProcessRequest,				&fncReadCoilsProcessResponse },					// 0x01
	{ MBUS_FNC_ENABLED, &fncReadDiscreteInputsGenerateRequest,	&fncReadDiscreteInputsProcessRequest,		&fncReadDiscreteInputsProcessResponse },		// 0x02
	{ MBUS_FNC_ENABLED, &fncReadHoldingRegistersGenerateRequest,&fncReadHoldingRegistersProcessRequest, 	&fncReadHoldingRegistersProcessResponse },		// 0x03
	{ MBUS_FNC_ENABLED, &fncReadInputRegistersGenerateRequest,	&fncReadInputRegistersProcessRequest,		&fncReadInputRegistersProcessResponse },		// 0x04
	{ MBUS_FNC_ENABLED, &fncWriteSingleCoilGenerateRequest,		&fncWriteSingleCoilProcessRequest,			&fncWriteSingleCoilProcessResponse },			// 0x05
	{ MBUS_FNC_ENABLED, &fncWriteSingleRegisterGenerateRequest,	&fncWriteSingleRegisterProcessRequest,		&fncWriteSingleRegisterProcessResponse },		// 0x06
};
