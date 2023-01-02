#include "modbus_registers.h"

#define			bitAddrToByteAddr(bitAddr) (bitAddr / 8)
#define			bitAddrToBitOffset(bitAddr) (bitAddr % 8)

static MBusRegister* mbusRegSetGetFirst(MBusRegister* regsetItem)
{
	if (regsetItem == 0) return 0;

	MBusRegister* currentItem = regsetItem;
	while (currentItem->RegPrevious != 0)
		currentItem = currentItem->RegPrevious;

	return currentItem;
}
static MBusRegister* mbusRegSetGetLast(MBusRegister* regsetItem)
{
	if (regsetItem == 0) return 0;

	MBusRegister* currentItem = regsetItem;
	while (currentItem->RegNext != 0)
		currentItem = currentItem->RegNext;

	return currentItem;
}

static MBusRegister* mbusRegSetGetClosest(MBusRegister** regset, unsigned short addr)
{
	if (*regset == 0) return 0;
	MBusRegister* currentItem = mbusRegSetGetFirst(*regset);

	while (currentItem->RegNext != 0 && currentItem->RegAddress < addr)
		currentItem = currentItem->RegNext;

	return currentItem;
}

void	MBusRegisterAdd(MBusRegister* reg, MBusRegister** regset)
{
	if (*regset == 0)
	{
		*regset = reg;
		return;
	}

	MBusRegister* nearestItem = mbusRegSetGetClosest(regset, reg->RegAddress);

	if (nearestItem->RegAddress == reg->RegAddress) return;

	MBusRegister* followingItem = 0;
	MBusRegister* previousItem = 0;

	if (nearestItem->RegAddress > reg->RegAddress)
	{
		followingItem = nearestItem;
		previousItem = nearestItem->RegPrevious;
	}
	if (nearestItem->RegAddress < reg->RegAddress)
	{
		followingItem = nearestItem->RegNext;
		previousItem = nearestItem;
	}

	reg->RegNext = 0;
	reg->RegPrevious = 0;
	if (followingItem != 0)
	{
		reg->RegNext = followingItem;
		followingItem->RegPrevious = reg;
	}
	if (previousItem != 0)
	{
		reg->RegPrevious = previousItem;
		previousItem->RegNext = reg;
	}
}
void	MBusRegisterRemove(MBusRegister* reg, MBusRegister** regset)
{
	MBusRegister* itemToDelete = mbusRegSetGetClosest(regset, reg->RegAddress);
	if (itemToDelete == 0) return;

	MBusRegister* followingItem = itemToDelete->RegNext;
	MBusRegister* previousItem = itemToDelete->RegPrevious;

	itemToDelete->RegNext = 0;
	itemToDelete->RegPrevious = 0;

	if (previousItem != 0 && followingItem != 0)
	{
		followingItem->RegPrevious = previousItem;
		previousItem->RegNext = followingItem;
	}
	else
	{
		if (followingItem != 0)
			followingItem->RegPrevious = 0;

		if (previousItem != 0)
			previousItem->RegNext = 0;
	}
}

unsigned char	MBusRegistersPack(MBusRegister** regset, unsigned short startAddr, unsigned char regsCount, unsigned char* output)
{
	unsigned int outputByteCounter = 0;

	MBusRegister* nearestItem = mbusRegSetGetClosest(regset, startAddr);
	if (nearestItem == 0) return outputByteCounter;

	for (int i = startAddr; i < startAddr + regsCount; i++)
	{
		if (nearestItem == 0 || nearestItem->RegAddress != i)
		{
			output[outputByteCounter] = 0;
			outputByteCounter++;
			output[outputByteCounter] = 0;
			outputByteCounter++;
		}
		else
		{
			output[outputByteCounter] = MBusRegisterGet(nearestItem) >> 8;
			outputByteCounter++;
			output[outputByteCounter] = MBusRegisterGet(nearestItem);
			outputByteCounter++;
			nearestItem = nearestItem->RegNext;
		}
	}

	return outputByteCounter;
}
unsigned char	MBusRegistersUnpack(unsigned char* input, unsigned short startAddr, unsigned char regsCount, MBusRegister** regset)
{
	unsigned int inputByteCounter = 0;

	MBusRegister* nearestItem = mbusRegSetGetClosest(regset, startAddr);
	if (nearestItem == 0) return inputByteCounter;

	for (int i = startAddr; i < startAddr + regsCount; i++)
	{
		if (nearestItem != 0 && nearestItem->RegAddress == i)
		{
			MBusRegisterSet(nearestItem, (input[inputByteCounter] << 8) | (input[inputByteCounter + 1]));
			nearestItem = nearestItem->RegNext;
		}
		inputByteCounter += 2;
	}

	return bitAddrToByteAddr(inputByteCounter);
}

unsigned char	MBusRegistersPackDiscrete(MBusRegister** regset, unsigned short startAddr, unsigned char regsCount, unsigned char* output)
{
	unsigned int outputBitCounter = 0;
	unsigned int outputByteCounter = 0;

	MBusRegister* nearestItem = mbusRegSetGetClosest(regset, startAddr);
	if (nearestItem == 0) return outputByteCounter;

	for (int i = startAddr; i < startAddr + regsCount; i++)
	{
		outputByteCounter = bitAddrToByteAddr(outputBitCounter);

		if (nearestItem == 0 || nearestItem->RegAddress != i)
		{
			output[outputByteCounter] &= ~(1 << bitAddrToBitOffset(outputBitCounter));
		}
		else
		{
			output[outputByteCounter] |= (MBusRegisterDiscreteGet(nearestItem) << bitAddrToBitOffset(outputBitCounter));
			nearestItem = nearestItem->RegNext;
		}

		outputBitCounter++;
	}

	return bitAddrToByteAddr(outputBitCounter);
}
unsigned char	MBusRegistersUnpackDiscrete(unsigned char* input, unsigned short startAddr, unsigned char regsCount, MBusRegister** regset)
{
	unsigned int inputBitCounter = 0;
	unsigned int inputByteCounter = 0;

	MBusRegister* nearestItem = mbusRegSetGetClosest(regset, startAddr);
	if (nearestItem == 0) return inputByteCounter;

	for (int i = startAddr; i < startAddr + regsCount; i++)
	{
		inputByteCounter = bitAddrToByteAddr(inputBitCounter);

		if (nearestItem != 0 && nearestItem->RegAddress == i)
		{
			if (input[inputByteCounter] & (1 << bitAddrToBitOffset(inputBitCounter)))
				MBusRegisterDiscreteSet(nearestItem);
			else
				MBusRegisterDiscreteReset(nearestItem);
			nearestItem = nearestItem->RegNext;
		}

		inputBitCounter++;
	}

	return bitAddrToByteAddr(inputBitCounter);
}