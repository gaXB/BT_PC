#ifndef _HEX_CONFIG_
#define _HEX_CONFIG_

//HEX 的数据说明
#define   HEXDATA_MAXLEN       500000
#define   HEX_MAX_REGION        10


typedef struct 
{
	int HexDataLen;
	int BeginAddr;
	int regionchecksum;
	unsigned char *pData;
}HEXREGION;

typedef struct
{
	HEXREGION   sRegionMsg[HEX_MAX_REGION];
	int maxLength;
	int TotalLength;
	int checksum;
	int i16RegionNum;
	unsigned char HexData[HEXDATA_MAXLEN];
}HEXDATA_ALL;

#define   HEXDATA_ERROR_RANGE    0xffff0000
#define   HEXDATA_ERROR_REGION    0xffff0001
#define   HEXDATA_ERROR_NUM      0xffff0002
#endif
