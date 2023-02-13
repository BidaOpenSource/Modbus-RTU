#ifndef MODBUS_H
#define MODBUS_H

#include "modbus_datagram.h"
#include "modbus_registers.h"

typedef enum
{
	MBUS_OK,
	MBUS_TRANSMITTING,
	MBUS_ERR,
	MBUS_NOT_READY
}
MBusStatus;

typedef enum
{
	MBUS_CH_READY,
	MBUS_CH_BUSY,
	MBUS_CH_TX_READY,
	MBUS_CH_ADU_HEADER_TX,
	MBUS_CH_PDU_FUNC_CODE_TX,
	MBUS_CH_PDU_HEADER_TX,
	MBUS_CH_PDU_DATA_TX,
	MBUS_CH_ADU_CRC_TX,
	MBUS_CH_ADU_HEADER_RX,
	MBUS_CH_PDU_FUNC_CODE_RX,
	MBUS_CH_PDU_HEADER_RX,
	MBUS_CH_PDU_DATA_RX,
	MBUS_CH_ADU_CRC_RX,
	MBUS_CH_DEADTIME,
	MBUS_CH_ERR_CRC_CHECK,
	MBUS_CH_ERR_UNEXPECTED,
	MBUS_CH_ERR_WRONG_TX_SEQUENCE,
	MBUS_CH_ERR_WRONG_RX_SEQUENCE,
	MBUS_CH_ERR_WRONG_RESPONSE,
	MBUS_CH_ERR_DEADTIME_WRONG_TIMING
}
MBusChanelStatus;

typedef struct
{
	MBusChanelStatus	Status;

	MBusADU				DatagramQuery;

	unsigned int		datagramResponseLastUpdate_ms;
	unsigned int		datagramResponseStageBytesReceived;
	unsigned int		datagramResponseStageBytesCountdown;
	MBusADU				DatagramResponse;

	void*				ptrResponseStruct;
}
MBusMasterChanel;

unsigned int MBusMasterIsReady(			MBusMasterChanel*	mbus);

unsigned int MBusCoilsRead(				MBusMasterChanel* mbus,
										unsigned char		slaveAddr,
										unsigned short		startAddr,
										unsigned char		coilsCount,
										MBusRegister** responseHolder);
unsigned int MBusDiscreteInputsRead(	MBusMasterChanel* mbus,
										unsigned char		slaveAddr,
										unsigned short		startAddr,
										unsigned char		inputsCount,
										MBusRegister** responseHolder);
unsigned int MBusHoldingRegistersRead(	MBusMasterChanel* mbus,
										unsigned char		slaveAddr,
										unsigned short		startAddr,
										unsigned char		regCount,
										MBusRegister** responseHolder);
unsigned int MBusInputsRead(			MBusMasterChanel* mbus,
										unsigned char		slaveAddr,
										unsigned short		startAddr,
										unsigned char		inputsCount,
										MBusRegister** responseHolder);

unsigned int MBusCoilWrite(				MBusMasterChanel* mbus,
										unsigned char		slaveAddr,
										unsigned short		regAddr,
										unsigned short		regVal,
										unsigned char* successFlag);
unsigned int MBusHoldingRegisterWrite(	MBusMasterChanel* mbus,
										unsigned char		slaveAddr,
										unsigned short		regAddr,
										unsigned short		regVal,
										unsigned char*		successFlag);

unsigned int MBusCoilsWrite(			MBusMasterChanel*	mbus,
										unsigned char		slaveAddr,
										unsigned short		startAddr,
										unsigned char		coilCount,
										MBusRegister**		coils,
										unsigned char*		coilsBuff,
										unsigned char*		successFlag);
unsigned int MBusHoldingRegistersWrite(	MBusMasterChanel*	mbus,
										unsigned char		slaveAddr,
										unsigned short		startAddr,
										unsigned char		regCount,
										MBusRegister**		holdingRegisters,
										unsigned char*		holdingRegistersBuff,
										unsigned char*		successFlag);

// external interrupts

void MBusMasterOnException_EXT(			MBusMasterChanel*	mbus);

void MBusMasterOnTxBytes_EXT(			MBusMasterChanel*	mbus,
										unsigned char* 		data,
										unsigned char		dataLength);
void MBusMasterTxBytesCompleted(		MBusMasterChanel*	mbus);

void MBusMasterRxByte(					MBusMasterChanel*	mbus,
										unsigned char	 	byte);
void MBusMasterOnRxReady_EXT(			MBusMasterChanel*	mbus);
void MBusMasterRxCompleted(				MBusMasterChanel*	mbus);

void MBusMasterOnDeadtimeRestart_EXT(	MBusMasterChanel*	mbus);
void MBusMasterDeadtimeElapsed(			MBusMasterChanel*	mbus);

typedef struct
{
	MBusChanelStatus	Status;

	MBusADU				Datagram;
}
MBusSlaveChanel;

//void MBusSlaveOnQuery(MBusSlaveChanel* mbus, unsigned char* data, unsigned char dataLength);
//void MBusSlaveOnTransmissionEnd(MBusSlaveChanel* mbus);

//void MBusSlaveOnException(MBusSlaveChanel* mbus);

void MBusSlaveOnTxBytes_IT(				MBusSlaveChanel* mbus,
										unsigned char*	data,
										unsigned char	dataLength);
void MBusSlaveOnRxBytes(				MBusSlaveChanel* mbus,
										unsigned char* data,
										unsigned char dataLength);

#endif
