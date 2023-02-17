#ifndef MODBUS_FNC_H
#define MODBUS_FNC_H

//	MODBUS EXCEPTION CODES (ref. "MODBUS Application Protocol Specification V1.1b", section 7)

typedef enum
{
	MBUS_EXC_NONE,
	MBUS_EXC_ILLEGAL_FUNCTION,
	MBUS_EXC_ILLEGAL_DATA_ADDRESS,
	MBUS_EXC_ILLEGAL_DATA_VALUE,
	MBUS_EXC_SLAVE_DEVICE_FAILURE,
	MBUS_EXC_ACKNOWLEDGE,
	MBUS_EXC_SLAVE_DEVICE_BUSY,
	MBUS_EXC_MEMORY_PARITY_ERR =						0x08,
	MBUS_EXC_GATEWAY_PATH_UNAVAILABLE =					0x0A,
	MBUS_EXC_GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND =	0x0B
}
MBusException;

//	MODBUS FUNCTION TYPE (ref. "MODBUS Application Protocol Specification V1.1b", section 5.1)

typedef enum
{
	MBUS_FNC_READ_COILS =				1,
	MBUS_FNC_READ_DISCRETE_INPUTS,
	MBUS_FNC_READ_HOLDING_REGISTERS,
	MBUS_FNC_READ_INPUT_REGISTER,
	MBUS_FNC_WRITE_SINGLE_COIL,
	MBUS_FNC_WRITE_SINGLE_REGISTER,
	MBUS_FNC_READ_EXCEPTION_STATUS,
	MBUS_FNC_DIAGNOSTIC,
	MBUS_FNC_GET_COM_EVENT_COUNTER =	11,
	MBUS_FNC_GET_COM_EVENT_LOG,
	MBUS_FNC_WRITE_MULTIPLE_COILS =		15,
	MBUS_FNC_WRITE_MULTIPLE_REGISTERS,
	MBUS_FNC_REPORT_SLAVE_ID,
	MBUS_FNC_READ_FILE_RECORD =			20,
	MBUS_FNC_WRITE_FILE_RECORD,
	MBUS_FNC_MASK_WRITE_REGISTER,
	MBUS_FNC_RW_MULTIPLE_REGISTERS,
	MBUS_FNC_READ_FIFO_QUEUE,
	MBUS_FNC_COUNT
}
MBusFunctionType;

typedef struct
{
	MBusException (*execute)(	unsigned char* request,
								unsigned char requestLength,
								unsigned char* response,
								unsigned char* responseLength);
}
MBusFunction;

extern MBusFunction MBusFunctions[MBUS_FNC_COUNT];

#define				MBusFunctionSupported(fnc)			(MBusFunctions[fnc].execute != 0)

#endif