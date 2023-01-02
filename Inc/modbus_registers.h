#ifndef MODBUS_REGISTERS_H
#define MODBUS_REGISTERS_H

typedef struct MBusRegister MBusRegister;
struct MBusRegister
{
	unsigned int*	Variable;
	unsigned int	BitMask;
	unsigned short	RegAddress;

	MBusRegister*	RegPrevious;
	MBusRegister*	RegNext;
};

#define			MBusRegisterDiscreteInstance(varAddr, bitMask, regAddr) ((MBusRegister){.Variable=varAddr, .BitMask=bitMask, .RegAddress=regAddr})

#define			MBusRegisterDiscreteGet(reg)	((*((reg)->Variable) & (reg)->BitMask) ? 1 : 0)
#define			MBusRegisterDiscreteSet(reg)	(*(reg->Variable) |= (reg->BitMask))
#define			MBusRegisterDiscreteReset(reg)	(*(reg->Variable) &= ~(reg->BitMask))

#define			MBusRegisterInstance(varAddr, regAddr) ((MBusRegister){.Variable=varAddr,.BitMask=0xFFFF, .RegAddress=regAddr})

#define			MBusRegisterGet(reg)			(*(reg->Variable) & reg->BitMask)
#define			MBusRegisterSet(reg, val)		(*(reg->Variable) = val)

void			MBusRegisterAdd(MBusRegister* reg, MBusRegister** regset);
void			MBusRegisterRemove(MBusRegister* reg, MBusRegister** regset);

unsigned char	MBusRegistersPack(MBusRegister** regset, unsigned short startAddr, unsigned char regsCount, unsigned char* output);
unsigned char	MBusRegistersUnpack(unsigned char* input, unsigned short startAddr, unsigned char regsCount, MBusRegister** regset);

unsigned char	MBusRegistersPackDiscrete(MBusRegister** regset, unsigned short startAddr, unsigned char regsCount, unsigned char* output);
unsigned char	MBusRegistersUnpackDiscrete(unsigned char* input, unsigned short startAddr, unsigned char regsCount, MBusRegister** regset);


#endif