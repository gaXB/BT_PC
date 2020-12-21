#include "Security.h"

uint32  SECURITY_MASK_LEVEL1  = 0X33E84278;
uint32  SECURITY_MASK_LEVEL2  = 0X46370AB3;


uint32 seedToKey (uint32 nSeed, uint8 SecurityLevel)
{
	uint32 i;
	uint32 u32key;
	union 
	{ 
		uint8 byte[4];
		uint32 wort;
	} seedlokal; 

	uint8 key[4];
	
	seedlokal.wort = nSeed ;
	uint32 mask;
	
	if (SecurityLevel == 1)
	{
		mask = SECURITY_MASK_LEVEL1;
	}
	else
	{
		mask = SECURITY_MASK_LEVEL2;
	}
	/* The array for SEED starts with [1], the array for KEY starts with [0] */
	/* seed contains the SEED from the ECU */
	/* length contains the number of bytes of seed */
	/* key contains the KEY to send to ECU */
	/* retLen contains the number of bytes to send to ECU as key */

	for (i = 0; i < 28; i++)
	{
		if (seedlokal.wort & 0x80000000)
		{
			seedlokal.wort = seedlokal.wort << 1;
			seedlokal.wort = seedlokal.wort ^ mask;
		}
		else
		{
			seedlokal.wort = seedlokal.wort << 1;
		}
	}
	for (i = 0; i < 4; i++)
	{
		key[3-i] = seedlokal.byte[i];
	}
	u32key = key[3] + (key[2]<<8) + (key[1]<<16) + (key[0]<<24);
	
	return u32key;
}

void SetSecurityMask(uint32 u32Level1Mask, uint32 u32Level2Mask)
{
	SECURITY_MASK_LEVEL1 = u32Level1Mask;
	SECURITY_MASK_LEVEL2 = u32Level2Mask;
}