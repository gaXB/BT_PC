#include "StdAfx.h"
#include "DatatoGrid.h"
#include "SystemConfig.h"

const unsigned  char  tbl_td_bmpv[] ={15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22,	                      //-30.0~-23.0
	                                22, 23, 24, 24, 25, 25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 36,	  //-22.5~-13.0
	                                36, 37, 38, 39, 40, 41, 42, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 55,	  //-12.5~ -3.0
	                                56, 57, 58, 59, 60, 61, 62, 64, 65, 66, 67, 68, 70, 71, 72, 73, 75, 76, 77, 78,	  // -2.5~  7.0
	                                80, 81, 82, 84, 85, 86, 88, 89, 90, 92, 93, 94, 96, 97, 98,100,101,103,104,105,	  //  7.5~ 17.0
	                                107,108,109,111,112,114,115,116,118,119,121,122,123,125,126,128,129,130,132,133,	// 17.5~ 27.0
	                                134,136,137,138,140,141,142,144,145,146,148,149,150,151,153,154,155,156,158,159,	// 27.5~ 37.0
	                                160,161,162,164,165,166,167,168,169,170,172,173,174,175,176,177,178,179,180,181,	// 37.5~ 47.0
	                                182,183,184,185,186,187};   // 47.5~ 50.0


#define   _TE(x)  (x*10)
#define   int16   signed short  

signed short TBL_SEGTAMB[12] = 
{//先取消-40℃和-30℃的点（最终版程序需加上），方便标定，增加35℃的点，
//	_T(-30), _T(-20), _T(-10), _T(0), _T(10), _T(20), _T(25), _T(30), _T(40),_T(50)	
	_TE(-40),_TE(-30), _TE(-20), _TE(-10), _TE(0), _TE(10), _TE(20), _TE(25), _TE(30),_TE(35), _TE(40),_TE(50)	
};

signed short TBL_OFFSETAMB[12]=
{
// 11, 31, 51, 71, 91, 122, 142, 162, 184, 209
//	100, 110, 120, 130, 140, 150, 155, 160, 170, 180		//2016-6-19 mdd add
//	42, 50, 62, 84, 130, 145, 158, 165, 177, 197		//2016-7-14 mdd
//	50, 62, 84, 130, 145, 158, 165, 177, 197, 207		//2016-7-15 mdd
	-123, -115, -103, -81, -35, -20, -7, 0, 10, 20, 32, 42
};


float  CalSeg_Line(int16* TBL_X, int16* TBL_Y, int16 X, int16 i16Length)
{
	int i;

	for (i = 0; i < i16Length && X >= TBL_X[i]; i++)
	{

	}

	if (i == 0)
	{
		return TBL_Y[0];
	}
	else if (i >= i16Length)
	{
		return TBL_Y[i16Length - 1];
	}
	else 
	{
		return (float) (TBL_Y[i] - TBL_Y[i-1])*(X - TBL_X[i-1])  / (TBL_X[i] - TBL_X[i-1]) + TBL_Y[i-1];
	}
}


CDatatoGrid::CDatatoGrid(void)

{
}

CDatatoGrid::~CDatatoGrid(void)
{
}

enum
{
	DTYPE_UINT8_UINT8 = 0, //相等
	DTYPE_UINT8_INT8,
	DTYPE_INT16_D10,     //int16 /10
	DTYPE_UINT16_D10,
	DTYPE_UINT16_UINT16,
	DTYPE_INT16_INT16,

};

CString  DataToString(unsigned char *Data, unsigned char Type)
{
	float fData;
	CString Str;
	switch(Type)
	{
	case DTYPE_UINT8_UINT8:
		Str.Format(_T("%d"), Data[0]);
		break;
	case DTYPE_UINT8_INT8:
		Str.Format(_T("%d"), (signed char)Data[0]);
		break;
	case DTYPE_UINT16_D10:
		fData = ((float)Data[0] +  Data[1]*256) /10;
		Str.Format(_T("%0.1f"), fData);
		break;
	case DTYPE_INT16_D10:
		fData = ((float) (signed short)(Data[0] +  Data[1]*256) ) /10;
		Str.Format(_T("%0.1f"), fData);
		break;
	case DTYPE_UINT16_UINT16:
		Str.Format(_T("%d"), Data[0] +  Data[1]*256);
		break;
	case DTYPE_INT16_INT16:
		Str.Format(_T("%d"), (unsigned short)(Data[0] +  Data[1]*256));
		break;
	}
	
	return Str;
}


void CDatatoGrid ::SetTdParameter(TDPARAMTER tdp)
{
	m_Tdpa =tdp;
}
// 将rxdata[] 送入 此类，并进行转换，返回 显示的个数
// rxdata 最大长度为62  //60+fo+checksum
#define  TEMP_CHARGE_OFFSET   80
int  KTin = 100 , KTset = 80;
int CDatatoGrid::TransData(unsigned char* rtx)
{
	float fTD, i16TDRamb;
	signed short i16Tset, i16Tin, OFFSET, i16TDRsun, i16Tamb;

	CString strText;
	GridString.RemoveAll();
	GridLong=0;
	
	
     i16Tset = rtx[0] + 256 * rtx[1];
     i16Tin = rtx[14] + 256 * rtx[15];
     i16Tamb = rtx[20] + 256 * rtx[21];
     i16TDRsun = rtx[22] + 256 * rtx[23];
	 OFFSET = 117; 
	//rxt[0]=tset

	 i16TDRamb = CalSeg_Line(TBL_SEGTAMB, TBL_OFFSETAMB, i16Tamb , 12);
     i16TDRamb = i16TDRamb*10;
	fTD = KTset*(i16Tset - 250) / 10 + KTin*(i16Tset - i16Tin)/10 - i16TDRamb + i16TDRsun + OFFSET*10 ;  

	if (fTD <= 0) fTD = 0;
	fTD = fTD/10;


	GridString.Add( DataToString(rtx, DTYPE_INT16_D10));
	

	//rxt[1]=MdMotorModeGet
	switch(rtx[2])
	{
		case 0:  strText= _T("VENT"); break;
		case 1:  strText=_T("B/L");break;
		case 2:  strText=_T("FOOT");break;
		case 3:  strText=_T("D/T");break;
		case 4:  strText=_T("DEF");break;
		case  85: strText=_T("AUTO");break;
		default:  strText=_T("XX");break;
	}
	GridString.Add(strText);


	//NcfModeGet
	switch(rtx[3])
	{
		case 0:  strText= _T("FRE"); break;
		case 2:
		case 1:  strText=_T("REC");break;
		case  85: strText=_T("AUTO");break;
		default:  strText=_T("XX");break;
	}
	GridString.Add(strText);

	//FanModeGet()
	if(rtx[4]<=8) strText.Format(_T("%d"),rtx[4]);
	else
	{
		switch(rtx[4])
			{
				case  85: strText=_T("AUTO");break;
				default:  strText=_T("XX");break;
			}
	}
	GridString.Add(strText);

	//ACModeGet()
	switch(rtx[5])
	{
		case  0: strText=_T("OFF");break;
		case  1: strText=_T("ON");break;
		case  2: strText=_T("ACECO");break;
		case	85: strText=_T("AUTO");break;
		default: strText=_T("XX");break;
	}
	GridString.Add(strText);

	
	GridString.Add( DataToString(&rtx[12], DTYPE_INT16_D10));

		// 34-37 为 显示温度
	/*for(int i=31;i<=33;i++)
	{
		strText.Format(_T("%0.1f"),(float)rtx[i]/2-TEMP_CHARGE_OFFSET/2);
		GridString.Add(strText);
	}*/

	GridString.Add( DataToString(&rtx[14], DTYPE_INT16_D10));

	GridString.Add( DataToString(&rtx[44], DTYPE_INT16_D10));

	GridString.Add( DataToString(&rtx[20], DTYPE_INT16_D10));

	GridString.Add( DataToString(&rtx[46], DTYPE_INT16_D10));

	GridString.Add( DataToString(&rtx[16], DTYPE_INT16_D10));

    GridString.Add( DataToString(&rtx[18], DTYPE_INT16_D10));



	strText.Format(_T("%d"),rtx[40]);
	GridString.Add(strText);

	// 5-16 为Ad等信号，直接输出显示
	for(int i=6;i<=11;i++)
	{
		//if(i==8) {strText.Format(_T("%0.2f"),(float)rtx[i]/51);}
		//else	
		strText.Format(_T("%d"),rtx[i]);
		GridString.Add(strText);

	}

	//strText.Format(_T("%0.1f"),(float)rtx[14]/2-TEMP_CHARGE_OFFSET/2 + 10);
	//GridString.Add(strText);

	strText.Format(_T("%d"),rtx[15]);
	GridString.Add(strText);

	strText.Format(_T("%d"),rtx[16]);
	GridString.Add(strText);

	//26 27 td
	GridString.Add( DataToString(&rtx[26], DTYPE_INT16_D10));
		
	//ramb
    GridString.Add( DataToString(&rtx[24], DTYPE_INT16_D10));

	GridString.Add( DataToString(&rtx[22], DTYPE_INT16_D10));



	strText.Format(_T("%0.1f"),(float)rtx[30]/10);//风挡占空比
	GridString.Add(strText);

	strText.Format(_T("%d"),rtx[31]);//风挡
	GridString.Add(strText);

	//23 get_sgcool
	if(rtx[32])
	strText=_T("ON");
	else strText=_T("OFF");
	GridString.Add(strText);

	//NCF GET
	switch(rtx[33])
	{
		case 0:  strText= _T("FRE"); break;
		case	2:
		case 1:  strText=_T("REC");break;
		default:  strText=_T("XX");break;
	}
	GridString.Add(strText);



	//mode 实际位置
	switch(rtx[34])
	{
		case 0:  strText= _T("VENT"); break;
		case 1:  strText=_T("B/L");break;
		case 2:  strText=_T("FOOT");break;
		case 3:  strText=_T("D/T");break;
		case 4:  strText=_T("DEF");break;
		default:  strText=_T("XX");break;
	}
	GridString.Add(strText);

	



	//电机位置
	strText.Format(_T("%d"),rtx[35]);
	GridString.Add(strText);

	strText.Format(_T("%d"),rtx[41]);
	GridString.Add(strText);

	float fHmix = (float)rtx[35];
	if (fHmix <= 10)  fHmix =0;
	else if (fHmix >= 245) fHmix = 100;
	else 
	{
		fHmix = (fHmix - 10)*100 / 235;
	}
	strText.Format(_T("%0.1f"),fHmix );
	GridString.Add(strText);



	strText.Format(_T("%d"),rtx[37]);
	GridString.Add(strText);

	
	strText.Format(_T("%d"),rtx[42]);
	GridString.Add(strText);


	//strText.Format(_T("%d"),rtx[29]);
	//GridString.Add(strText);




	switch(rtx[39])
	{
		case 1:   strText= _T("制冷保护1"); break;
		case 2:   strText= _T("制冷保护2"); break;
		case 0x11:  strText=_T("热风保护1");break;
		case 0x12:  strText=_T("热风保护2");break;
		case 0:   strText=_T("无保护");break;
		default:  strText=_T("XX");break;
	}
	GridString.Add(strText);

#if TEST_MOTOR
	for(int i=46;i<=51;i++)
	{
		//if(i==8) {strText.Format(_T("%0.2f"),(float)rtx[i]/51);}
		//else	
		strText.Format(_T("%d"),rtx[i]);
		GridString.Add(strText);

	}
#endif

	GridString.Add( DataToString(&rtx[50], DTYPE_INT16_D10));
	GridString.Add( DataToString(&rtx[52], DTYPE_INT16_D10));		
	GridString.Add( DataToString(&rtx[54], DTYPE_INT16_D10));
	
	GridString.Add( DataToString(&rtx[76], DTYPE_INT16_D10));
	GridString.Add( DataToString(&rtx[78], DTYPE_INT16_D10));		
	GridString.Add( DataToString(&rtx[80], DTYPE_INT16_D10));


	GridString.Add( DataToString(&rtx[82], DTYPE_INT16_D10));
	GridString.Add( DataToString(&rtx[84], DTYPE_INT16_D10));		
	GridString.Add( DataToString(&rtx[86], DTYPE_INT16_D10));
	GridString.Add( DataToString(&rtx[88], DTYPE_INT16_D10));
	//GridString.Add( DataToString(&rtx[90], DTYPE_INT16_D10));		
	//GridString.Add( DataToString(&rtx[92], DTYPE_INT16_D10));
//	strText.Format(_T("%0.1f"),fTD);
//	GridString.Add(strText);

//	strText.Format(_T("%0.1f"),i16TDRamb);/
//	GridString.Add(strText);

	//系统状态
/*	switch(rtx[39])
	{
		case 0:  strText= _T("初始化"); break;
		case 1:  strText=_T("OFF状态");break;
		case 2:  strText=_T("自检状态");break;
		case 3:  strText=_T("工作状态");break;
		default:   strText=_T("XXXX");break;
	}
	GridString.Add(strText);
*/


	strText =_T(""); ///后面补空格，一定要大于等于 总长度
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridString.Add(strText);
	GridLong = GridString.GetSize();
	


	ASSERT(GridLong >= CAILBRATION_RAM_NUM);  //长度必须相等
	return GridLong;
}
