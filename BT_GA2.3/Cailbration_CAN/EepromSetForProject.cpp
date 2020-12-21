#include "stdafx.h"
//#include "ToolBar.h"
#include "EepromDialog1.h"
#include"StdioFileEx.h"
// CEepromDialog1 对话框
#define   DISPLAY_FLG_ALL   100
#define   DISPLAY_FLAG      100
#if (DISPLAY_FLAG == DISPLAY_FLG_ALL)  
int Eeprom_DisplayFlg[200]=
{
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

};

#else

int Eeprom_DisplayFlg[200]=
{
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

};
#endif

float ChangeFloat10(float data)
{
	data=data*10;

	return data;
}

float ChangeINT255100(int data)
{
	float fResult;
	fResult = (float)data*100/255;

	if (fResult - (int)fResult > 0.5) fResult += 1;
	
	fResult = (int)fResult;
	return fResult;
	
}

unsigned char ChangeFloat100255(float data)
{
	int intResult;
	intResult = (int)(data*255/10);
	if (intResult%10 > 5) intResult += 10;

	intResult = intResult / 10;

	if (intResult > 255)  intResult = 255;

	return (unsigned char)intResult;
}
void FirstRowSet(CStringArray  *FirstRow)
{


    FirstRow->Add(L"编号");                 //我加入字段的值
	FirstRow->Add(L"名称");
	FirstRow->Add(L"值");
	
	FirstRow->Add(L"单位");
	FirstRow->Add(L"精度");
	FirstRow->Add(L"范围");

	ASSERT(FirstRow->GetSize()>=5);
}


int	GridNameSet(EEPROMDATA *EepromData)
{
int  num=1;

	EepromData->Number =num;
	EepromData->Name =L"Sun_d";
	EepromData->Unit = L"sec" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"0~25.5";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"A";
	EepromData->Unit = L"%" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"0~25.5";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"B";
	EepromData->Unit = L"%" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"0-25.5";
	EepromData++;
	num++;


	EepromData->Number =num;
	EepromData->Name =L"C";
	EepromData->Unit = L"%" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"0-25.5";
	EepromData++;
	num++;

		EepromData->Number =num;
	EepromData->Name =L"D";
	EepromData->Unit = L"%" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"0-25.5";
	EepromData++;
	num++;


	EepromData->Number =num;
	EepromData->Name =L"EX1";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"-1000 ~1000 ";
	EepromData++;
	num++;

    EepromData->Number =num;
	EepromData->Name =L"EX2";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"-1000 ~1000";
	EepromData++;
	num++;

    EepromData->Number =num;
	EepromData->Name =L"EX3";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"-1000 ~1000";
	EepromData++;
	num++;

    EepromData->Number =num;
	EepromData->Name =L"EX4";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"-1000 ~1000";
	EepromData++;
	num++;


    EepromData->Number =num;
	EepromData->Name =L"EX5";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"-1000 ~1000";
	EepromData++;
	num++;

    EepromData->Number =num;
	EepromData->Name =L"EX6";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"-1000 ~1000";
	EepromData++;
	num++;

    EepromData->Number =num;
	EepromData->Name =L"EX7";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"-1000 ~1000";
	EepromData++;
	num++;

    EepromData->Number =num;
	EepromData->Name =L"EX8";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"-1000 ~1000";
	EepromData++;
	num++;


    EepromData->Number =num;
	EepromData->Name =L"EX9";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"-1000 ~1000";
	EepromData++;
	num++;

    EepromData->Number =num;
	EepromData->Name =L"EX10";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"-1000 ~1000";
	EepromData++;
	num++;

	
    EepromData->Number =num;
	EepromData->Name =L"EY1";
	EepromData->Unit = L"deg" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100.0 ~100.0";
	EepromData++;
	num++;

	    EepromData->Number =num;
	EepromData->Name =L"EY2";
	EepromData->Unit = L"deg" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100.0 ~100.0";
	EepromData++;
	num++;

	    EepromData->Number =num;
	EepromData->Name =L"EY3";
	EepromData->Unit = L"deg" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100.0 ~100.0";
	EepromData++;
	num++;

	    EepromData->Number =num;
	EepromData->Name =L"EY4";
	EepromData->Unit = L"deg" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100.0 ~100.0";
	EepromData++;
	num++;

	    EepromData->Number =num;
	EepromData->Name =L"EY5";
	EepromData->Unit = L"deg" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100.0 ~100.0";
	EepromData++;
	num++;

	    EepromData->Number =num;
	EepromData->Name =L"EY6";
	EepromData->Unit = L"deg" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100.0 ~100.0";
	EepromData++;
	num++;

	    EepromData->Number =num;
	EepromData->Name =L"EY7";
	EepromData->Unit = L"deg" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100.0 ~100.0";
	EepromData++;
	num++;

	    EepromData->Number =num;
	EepromData->Name =L"EY8";
	EepromData->Unit = L"deg" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100.0 ~100.0";
	EepromData++;
	num++;
	    EepromData->Number =num;
	EepromData->Name =L"EY9";
	EepromData->Unit = L"deg" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100.0 ~100.0";
	EepromData++;
	num++;
	    
	EepromData->Number =num;
	EepromData->Name =L"EY10";
	EepromData->Unit = L"deg" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100.0 ~100.0";
	EepromData++;
	num++;


		EepromData->Number =num;
	EepromData->Name =L"ATtime_x1";
	EepromData->Unit = L"S" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0 ~1000";
	EepromData++;
	num++;

			EepromData->Number =num;
	EepromData->Name =L"ATtime_x2";
	EepromData->Unit = L"S" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0 ~1000";
	EepromData++;
	num++;


			EepromData->Number =num;
	EepromData->Name =L"ATtime_x3";
	EepromData->Unit = L"S" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0 ~1000";
	EepromData++;
	num++;

			EepromData->Number =num;
	EepromData->Name =L"ATtime_x4";
	EepromData->Unit = L"S" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0 ~1000";
	EepromData++;
	num++;

			EepromData->Number =num;
	EepromData->Name =L"ATtime_x5";
	EepromData->Unit = L"S" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0 ~1000";
	EepromData++;
	num++;

			EepromData->Number =num;
	EepromData->Name =L"ATtime_x6";
	EepromData->Unit = L"S" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0 ~1000";
	EepromData++;
	num++;

			EepromData->Number =num;
	EepromData->Name =L"ATtime_y1";
	EepromData->Unit = L"deg" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100.0 ~100.0";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"ATtime_y2";
	EepromData->Unit = L"deg" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100.0 ~100.0";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"ATtime_y3";
	EepromData->Unit = L"deg" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100.0 ~100.0";
	EepromData++;
	num++;

		EepromData->Number =num;
	EepromData->Name =L"ATtime_y4";
	EepromData->Unit = L"deg" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100.0 ~100.0";
	EepromData++;
	num++;

		EepromData->Number =num;
	EepromData->Name =L"ATtime_y5";
	EepromData->Unit = L"deg" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100.0 ~100.0";
	EepromData++;
	num++;

		EepromData->Number =num;
	EepromData->Name =L"ATtime_y6";
	EepromData->Unit = L"deg" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100.0 ~100.0";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"Tincar_set";
	EepromData->Unit = L"S" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0 ~1000";
	EepromData++;
	num++;

EepromData->Number =num;
	EepromData->Name =L"Sun_1X";
	EepromData->Unit = L"AD" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0-255";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"Sun_2X";
	EepromData->Unit = L"AD" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0-255";
	EepromData++;
	num++;


	EepromData->Number =num;
	EepromData->Name =L"Sun_3X";
	EepromData->Unit = L"AD" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0-255";
	EepromData++;
	num++;


	EepromData->Number =num;
	EepromData->Name =L"Sun_4X";
	EepromData->Unit = L"AD" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0-255";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"Sun_5X";
	EepromData->Unit = L"AD" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0-255";
	EepromData++;
	num++;


	
	EepromData->Number =num;
	EepromData->Name =L"Sun_6X";
	EepromData->Unit = L"AD" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0-255";
	EepromData++;
	num++;


	EepromData->Number =num;
	EepromData->Name =L"Sun_1Y";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"-120 ~ 0";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"Sun_2Y";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"-120 ~ 0";
	EepromData++;
	num++;



	EepromData->Number =num;
	EepromData->Name =L"Sun_3Y";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"-120 ~ 0";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"Sun_4Y";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"-120 ~ 0";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"Sun_5Y";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"-120 ~ 0";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"Sun_6Y";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"-120 ~ 0";
	EepromData++;
	num++;

		EepromData->Number =num;
	EepromData->Name =L"HOTSEQ_1";
	EepromData->Unit = L"sec" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"0 ~ 25.5";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"COOLSEQ_1";
	EepromData->Unit = L"sec" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"0 ~ 25.5";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"COOLSEQ_2";
	EepromData->Unit = L"sec" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"0 ~ 25.5";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"UNUSE";
	EepromData->Unit = L"sec" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"0 ~ 255";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"UNUSE1";
	EepromData->Unit = L"sec" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0 ~ 255";
	EepromData++;
	num++;

		EepromData->Number =num;
	EepromData->Name =L"BV_FRESH";
	EepromData->Unit = L"%" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"00 ~ 255";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"BLR_VSMAX";
	EepromData->Unit = L"V" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0 ~ 25.5";
	EepromData++;
	num++;

	
	EepromData->Number =num;
	EepromData->Name =L"Core_Eff";
	EepromData->Unit = L"con" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"100 ~ 255";
	EepromData++;
	num++;


		EepromData->Number =num;
	EepromData->Name =L"G_Td";
	EepromData->Unit = L"con" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"0 ~ 25.5";
	EepromData++;
	num++;



	EepromData->Number =num;
	EepromData->Name =L"吹面";
	EepromData->Unit = L"ad" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0~255";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"吹面吹脚";
	EepromData->Unit = L"ad" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0~255";
	EepromData++;
	num++;


	EepromData->Number =num;
	EepromData->Name =L"吹脚";
	EepromData->Unit = L"AD" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0~255";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"吹脚除霜";
	EepromData->Unit = L"AD" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0~255";
	EepromData++;
	num++;
	    
	EepromData->Number =num;
	EepromData->Name =L"除霜";
	EepromData->Unit = L"AD" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0~255";
	EepromData++;
	num++;



	EepromData->Number =num;
	EepromData->Name =L"AM_FF0_10";
	EepromData->Unit = L"%" ;
	EepromData ->Accuracy = L"0.1";	
	EepromData ->Area = L"0~100.0";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"AM_FF0_20";
	EepromData->Unit = L"%" ;
	EepromData ->Accuracy = L"0.1";	
	EepromData ->Area = L"0~100.0";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"AM_FF0_30";
	EepromData->Unit = L"%" ;
	EepromData ->Accuracy = L"0.1";	
	EepromData ->Area = L"0~100.0";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"AM_FF0_40";
	EepromData->Unit = L"%" ;
	EepromData ->Accuracy = L"0.1";	
	EepromData ->Area = L"0~100.0";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"AM_FF0_50";
	EepromData->Unit = L"%" ;
	EepromData ->Accuracy = L"0.1";	
	EepromData ->Area = L"0~100.0";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"AM_FF0_60";
	EepromData->Unit = L"%" ;
	EepromData ->Accuracy = L"0.1";	
	EepromData ->Area = L"0~100.0";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"AM_FF0_70";
	EepromData->Unit = L"%" ;
	EepromData ->Accuracy = L"0.1";	
	EepromData ->Area = L"0~100.0";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"AM_FF0_80";
	EepromData->Unit = L"%" ;
	EepromData ->Accuracy = L"0.1";	
	EepromData ->Area = L"0~100.0";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"AM_FF0_90";
	EepromData->Unit = L"%" ;
	EepromData ->Accuracy = L"0.1";	
	EepromData ->Area = L"0~100.0";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"T1(B/L)";
	EepromData->Unit = L"%" ;
	EepromData ->Accuracy = L"1";	
	EepromData ->Area = L"0~100";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"T2";
	EepromData->Unit = L"%" ;
	EepromData ->Accuracy = L"1";	
	EepromData ->Area = L"0~100";
	EepromData++;
	num++;


	EepromData->Number =num;
	EepromData->Name =L"T3";
	EepromData->Unit = L"%" ;
	EepromData ->Accuracy = L"1";	
	EepromData ->Area = L"0~100";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"AMB_STHOT";
	EepromData->Unit = L"deg" ;
	EepromData ->Accuracy = L"0.1";	
	EepromData ->Area = L"-100~100.0";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"AMB_STCOOL";
	EepromData->Unit = L"deg" ;
	EepromData ->Accuracy = L"0.1";	
	EepromData ->Area = L"-100~100.0";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"TD_STHOT";
	EepromData->Unit = L"td" ;
	EepromData ->Accuracy = L"0.1";	
	EepromData ->Area = L"-100~100.0";
	EepromData++;
	num++;

		EepromData->Number =num;
	EepromData->Name =L"TD_STCOOL";
	EepromData->Unit = L"td" ;
	EepromData ->Accuracy = L"0.1";	
	EepromData ->Area = L"-100~100.0";
	EepromData++;
	num++;


		EepromData->Number =num;
	EepromData->Name =L"Tadt_Lo1";
	EepromData->Unit = L"deg" ;
	EepromData ->Accuracy = L"0.1";	
	EepromData ->Area = L"-100~100.0";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"Tadt_Lo2";
	EepromData->Unit = L"deg" ;
	EepromData ->Accuracy = L"0.1";	
	EepromData ->Area = L"-100~100.0";
	EepromData++;
	num++;


	
	EepromData->Number =num;
	EepromData->Name =L"Amb_Auto_off";
	EepromData->Unit = L"deg" ;
	EepromData ->Accuracy = L"0.1";	
	EepromData ->Area = L"-100~100.0";
	EepromData++;
	num++;

	
	EepromData->Number =num;
	EepromData->Name =L"Amb_Auto_on";
	EepromData->Unit = L"deg" ;
	EepromData ->Accuracy = L"0.1";	
	EepromData ->Area = L"-100~100.0";
	EepromData++;
	num++;


	EepromData->Number =num;
	EepromData->Name =L"Evap_Auto_off";
	EepromData->Unit = L"deg" ;
	EepromData ->Accuracy = L"0.1";	
	EepromData ->Area = L"-100~100.0";
	EepromData++;
	num++;

	
	EepromData->Number =num;
	EepromData->Name =L"Evap_Auto_on";
	EepromData->Unit = L"deg" ;
	EepromData ->Accuracy = L"0.1";	
	EepromData ->Area = L"-100~100.0";
	EepromData++;
	num++;


	EepromData->Number =num;
	EepromData->Name =L"TD_FOOT";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100~ 100";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"TD_BLF";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100~ 100";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"TD_BLU";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100~ 100";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"TD_VENT";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100~ 100";
	EepromData++;
	num++;



	
	EepromData->Number =num;
	EepromData->Name =L"TD_FR";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100~ 100";
	EepromData++;
	num++;

		EepromData->Number =num;
	EepromData->Name =L"TD_REC";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100~ 100";
	EepromData++;
	num++;

		EepromData->Number =num;
	EepromData->Name =L"BLRTDM_8";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100~ 100";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"BLRTDM_7";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100~ 100";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"BLRTDM_6";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100~ 100";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"BLRTDM_5";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100~ 100";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"BLRTDM_4";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100~ 100";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"BLRTDM_3";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100~ 100";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"BLRTDM_2";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100~ 100";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"BLRTDM_1";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100~ 100";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"BLRTDP_1";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100~ 100";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"BLRTDP_2";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100~ 100";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"BLRTDP_3";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100~ 100";
	EepromData++;
	num++;

	
	EepromData->Number =num;
	EepromData->Name =L"BLRTDP_4";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100~ 100";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"BLRTDP_5";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100~ 100";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"BLRTDP_6";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100~ 100";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"BLRTDP_7";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100~ 100";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"BLRTDP_8";
	EepromData->Unit = L"TD" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"-100~ 100";
	EepromData++;
	num++;


	EepromData->Number =num;
	EepromData->Name =L"风量1当";
	EepromData->Unit = L"V" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0-25.5";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"风量2当";
	EepromData->Unit = L"V" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0-25.5";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"风量3当";
	EepromData->Unit = L"V" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0-25.5";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"风量4当";
	EepromData->Unit = L"V" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0-25.5";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"风量5当";
	EepromData->Unit = L"V" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0-25.5";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"风量6当";
	EepromData->Unit = L"V" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0-25.5";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"风量7当";
	EepromData->Unit = L"V" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0-25.5";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"风量8当";
	EepromData->Unit = L"V" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0-25.5";
	EepromData++;
	num++;




	EepromData->Number =num;
	EepromData->Name =L"dTdt_I_Min";
	EepromData->Unit = L"dec" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"0~100";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"dTdt_I_Max";
	EepromData->Unit = L"dec" ;
	EepromData ->Accuracy = L"0.1";
	EepromData ->Area = L"0~100";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"AM_Imax";
	EepromData->Unit = L"HEX" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0~2000";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"AM_Imin";
	EepromData->Unit = L"HEX" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"-2000~0";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"AM_Pmax";
	EepromData->Unit = L"HEX" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0~2000";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"AM_Pmin";
	EepromData->Unit = L"HEX" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"-2000~0";
	EepromData++;
	num++;

		EepromData->Number =num;
	EepromData->Name =L"UNUSE";
	EepromData->Unit = L"HEX" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0~0";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"AM_I_TIME";
	EepromData->Unit = L"0.1sec" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"1~255";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"全热";
	EepromData->Unit = L"AD" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0~255";
	EepromData++;
	num++;

	EepromData->Number =num;
	EepromData->Name =L"全冷";
	EepromData->Unit = L"AD" ;
	EepromData ->Accuracy = L"1";
	EepromData ->Area = L"0~255";
	EepromData++;
	num++;

//MAX_EEPROMNUM_LINE 增加数据时，需要改变此参数


	ASSERT(num <= MAX_EEPROMNUM_LINE+5);
	return  --num;
}


void	DispDataToEeprom(int *Eeprom,float *DisPlayData);

enum
{
	DTYPE_UINT_FLOAT = 0, // 占1个字节  无符号显示 比如0xf5  显示： 245
	DTYPE_INT_FLOAT,      // 占1个字节  有符号显示 比如0xf5  显示： -11
	DTYPE_UINT10_FLOAT,   // 占1个字节  无符号显示除以10   比如0xf5  显示： 24.5
	DTYPE_INT10_FLOAT,    // 占1个字节  有符号显示除以10   比如0xf5  显示： -1.1
	DTYPE_TEMP_FLOAT,     // 1个字节对
	DTYPE_UINTOFFSET1_FLOAT,   //OFFSET -165
	DTYPE_INT2_FLOAT,
	DTYPE_UINT16_FLOAT,
	DTYPE_UINT10_FLOAT16,
	DTYPE_INT16_FLOAT_D10,
	DTYPE_INT16_FLOAT,
	DTYPE_UINT100255_FLOAT,
	DTYPE_UINT01_FLOAT,

};
#define  DTYPE_OFFSET1    -165

int EEDataType[PARAMETER_ACCNUM] =
{
DTYPE_UINT10_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,//5
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,//25  
DTYPE_UINT16_FLOAT,
DTYPE_UINT16_FLOAT,
DTYPE_UINT16_FLOAT,    //ATTIM
DTYPE_UINT16_FLOAT,
DTYPE_UINT16_FLOAT,
DTYPE_UINT16_FLOAT,//31 
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,//37
DTYPE_UINT16_FLOAT,
DTYPE_UINT_FLOAT,
DTYPE_UINT_FLOAT,
DTYPE_UINT_FLOAT,   //SUN_X
DTYPE_UINT_FLOAT,
DTYPE_UINT_FLOAT,
DTYPE_UINT_FLOAT,//44

DTYPE_INT10_FLOAT,
DTYPE_INT10_FLOAT,
DTYPE_INT10_FLOAT,     //SUN_Y
DTYPE_INT10_FLOAT,
DTYPE_INT10_FLOAT,
DTYPE_INT10_FLOAT,  //50

DTYPE_UINT10_FLOAT,
DTYPE_UINT10_FLOAT,
DTYPE_UINT10_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT, //预留1 ，2

DTYPE_UINT_FLOAT,
DTYPE_UINT10_FLOAT,
DTYPE_UINT_FLOAT,
DTYPE_UINT10_FLOAT,  //59， u8G_TD

DTYPE_UINT_FLOAT,
DTYPE_UINT_FLOAT,
DTYPE_UINT_FLOAT,
DTYPE_UINT_FLOAT,
DTYPE_UINT_FLOAT, //64  VENT

DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT, //73 TBL_TEMPMOTOR_TD

DTYPE_UINT_FLOAT,
DTYPE_UINT_FLOAT,
DTYPE_UINT_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT, //AMB_STCOOL 78
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT, // 82 Tadt_Lo2

DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT, // 86 Evap_Auto_on

DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT, //92  NCF_2

DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT, //108 BLRTDP_8
DTYPE_UINT10_FLOAT,
DTYPE_UINT10_FLOAT,
DTYPE_UINT10_FLOAT,
DTYPE_UINT10_FLOAT,
DTYPE_UINT10_FLOAT,
DTYPE_UINT10_FLOAT,
DTYPE_UINT10_FLOAT,
DTYPE_UINT10_FLOAT,  //116 AF_8
DTYPE_TEMP_FLOAT,
DTYPE_TEMP_FLOAT,
DTYPE_INT16_FLOAT,
DTYPE_INT16_FLOAT,
DTYPE_INT16_FLOAT,
DTYPE_INT16_FLOAT,
DTYPE_INT16_FLOAT, //123 UNUSE3
DTYPE_UINT_FLOAT,
DTYPE_UINT16_FLOAT,
DTYPE_UINT16_FLOAT,  //126
};

void	EepromToDispData(int *Eeprom,float *DisPlayData)
{
	int i, j;
	
/*	DisPlayData[0] = (float)Eeprom[0];
	
	for ( i=1; i<5; i++)
	{
		DisPlayData[i] = (float)Eeprom[i] / 10;	
	}

	for ( i=5; i < 11; i++)
	{
		DisPlayData[i] = (float)((signed char)Eeprom[i]);	
	}

	for ( i=11; i < 19; i++)
	{
		DisPlayData[i] = (float)Eeprom[i] / 2 -30;	
	}

    DisPlayData[19] = (float)((signed char)Eeprom[19]);

	DisPlayData[20] = (float)((signed char)Eeprom[20]);

	DisPlayData[21] = (float)Eeprom[21];

	DisPlayData[22] = (float)Eeprom[22];

	DisPlayData[23] = (float)Eeprom[23];

	for ( i=24; i < 30; i++)
	{
		DisPlayData[i] = (float)Eeprom[i] / 2 -30;	
	}


	DisPlayData[30] = (float)((signed char)Eeprom[30]);

	DisPlayData[31] = (float)((signed char)Eeprom[31]);

	DisPlayData[32] = (float)Eeprom[32] / 10;	

	DisPlayData[33] = (float)Eeprom[33] ;	

	DisPlayData[34] = (float)Eeprom[34] / 10;	

	DisPlayData[35] = (float)Eeprom[35] ;	
	
	DisPlayData[36] = (float)Eeprom[36] / 10;	

	for ( i=37; i < 46; i++)
	{
		DisPlayData[i] = (float)((signed char)Eeprom[i]);	
	}


	for ( i=46; i < 52; i++)
	{
		DisPlayData[i] = (float)Eeprom[i];	
	}

	for ( i=52; i < 66; i++)
	{
		DisPlayData[i] = (float)((signed char)Eeprom[i]);	
	}

	for ( i=66; i < 71; i++)
	{
		DisPlayData[i] = (float)Eeprom[i];	
	}
	DisPlayData[71] = (float)((signed char)Eeprom[71]);	
	DisPlayData[72] = (float)((signed char)Eeprom[72]);	
*/
	for(i = 0, j = 0; i< PARAMETER_ACCNUM; i++,j++)
	{
		switch(EEDataType[i])
		{
			default:
				i++;
				i--;
				break;
			case DTYPE_UINT_FLOAT:
				DisPlayData[i] = (float)Eeprom[j];
				break;
			case DTYPE_INT_FLOAT:
				DisPlayData[i] = (float)((signed char)Eeprom[j]);
				break;
			case DTYPE_UINT10_FLOAT:
				DisPlayData[i] = (float)Eeprom[j] / 10;	
				break;
			case DTYPE_UINT01_FLOAT:
				DisPlayData[i] = (float)Eeprom[j] * 10;	
				break;
			case DTYPE_INT10_FLOAT:
				DisPlayData[i] = (float)(signed char)Eeprom[j] / 10;	
				break;
			case DTYPE_INT16_FLOAT_D10:
			case DTYPE_TEMP_FLOAT:
				DisPlayData[i] = (float)((signed short)(Eeprom[j] +  Eeprom[j+1]*256))/ 10;
				j++;
				break;
			case DTYPE_UINT10_FLOAT16:
				DisPlayData[i] = (float)((Eeprom[j] +  Eeprom[j+1]*256))/ 10;
				j++;
				break;
			case DTYPE_INT16_FLOAT:
				DisPlayData[i] = (float)(signed short(Eeprom[j] +  Eeprom[j+1]*256));
				j++;
				break;

		    case DTYPE_UINTOFFSET1_FLOAT:
				DisPlayData[i] = (float)Eeprom[j] +DTYPE_OFFSET1;
				break;
			case DTYPE_INT2_FLOAT:
				DisPlayData[i] = (float)(signed char)Eeprom[j] / 2;
				break;
			case DTYPE_UINT100255_FLOAT:
				DisPlayData[i] = ChangeINT255100(Eeprom[j]);
				break;
			case DTYPE_UINT16_FLOAT:
				DisPlayData[i] = Eeprom[j++];
				DisPlayData[i] += Eeprom[j] *256;
	
				break;
		}
	}

}

void	DispDataToEeprom(int *Eeprom,float *DisPlayData)
{
	int i , row=0;
	for(row = 0, i= 0; row< PARAMETER_ACCNUM; row++)
	{
		if (Eeprom_DisplayFlg[row])
		{//此功能不对
			switch(EEDataType[row])
			{
				default:
					i++;
					i--;
					break;
				case DTYPE_UINT_FLOAT:
					Eeprom[i] = (int)DisPlayData[row];
					break;
				case DTYPE_INT_FLOAT:
					Eeprom[i] = (unsigned char)(int)DisPlayData[row];
					break;
				case DTYPE_UINT10_FLOAT:
					Eeprom[i] = (int)ChangeFloat10(DisPlayData[row]);	
					break;
		    	case DTYPE_UINT01_FLOAT:
					Eeprom[i] = (int)(DisPlayData[row]/10);	
					break;
				case DTYPE_INT10_FLOAT:
					Eeprom[i] = (unsigned char)(int)ChangeFloat10(DisPlayData[row]);	
					break;
				

				case DTYPE_UINTOFFSET1_FLOAT:
					Eeprom[i] = (int)DisPlayData[row] -  DTYPE_OFFSET1;
					break;
				case DTYPE_INT2_FLOAT:
					Eeprom[i] = (unsigned char)(int)(DisPlayData[row]*2);	
					break;
				case DTYPE_UINT100255_FLOAT:
					Eeprom[i] = ChangeFloat100255(DisPlayData[row]);
					break;
				case DTYPE_UINT16_FLOAT:
					Eeprom[i++] = (unsigned char)( (unsigned short)DisPlayData[row]);
					Eeprom[i] = (unsigned char)((unsigned short)DisPlayData[row] >> 8);
					break;
				case DTYPE_UINT10_FLOAT16:
					Eeprom[i++] = (unsigned char)( (int)ChangeFloat10(DisPlayData[row]) );
					Eeprom[i] = (unsigned char)( (unsigned short)ChangeFloat10(DisPlayData[row]) >> 8);
					break;
					case DTYPE_TEMP_FLOAT:
				case DTYPE_INT16_FLOAT_D10:
		    		Eeprom[i++] = (unsigned char)( (unsigned short)ChangeFloat10(DisPlayData[row]) );
					Eeprom[i] = (unsigned char)( (unsigned short)ChangeFloat10(DisPlayData[row]) >> 8);
					break;	

				case DTYPE_INT16_FLOAT:
					Eeprom[i++] = (unsigned char)( (unsigned short)DisPlayData[row]);
					Eeprom[i] = (unsigned char)((unsigned short)DisPlayData[row] >> 8);
					break;

    //DTYPE_UINT16_FLOAT,
	//DTYPE_UINT10_FLOAT16,
	//DTYPE_INT16_FLOAT_D10,
	//DTYPE_INT16_FLOAT,
			}
			i++;
		}
	}

}

void EepromInit(int *eeprom)
{
	int * rtx;
	for(int i=0; i<MAX_DATA_NUM;i++)
	{
		eeprom[i] =245;
	}
  /*  eeprom[ 0]= 243;eeprom[ 1]=186;eeprom[ 2]= 129;eeprom[ 3]=72;eeprom[ 4]= 12;eeprom[ 5]= 24;eeprom[ 6]=233;eeprom[ 7]=233;eeprom[ 8]= 2;eeprom[ 9]=102; 
      //10-20  const unsigned  static unsigned char tbl_fan[]={0,95,120,145,170,190,220,255,255};
      eeprom[10]= 0;eeprom[11]=  95;eeprom[12]=  116;eeprom[13]= 137;eeprom[14]= 158;eeprom[15]= 179;eeprom[16]=201;eeprom[17]=220;eeprom[18]= 240;eeprom[19]= 5;
      eeprom[20]= 0;eeprom[21]= 48;eeprom[22]=13;eeprom[23]= 18;eeprom[24]=9;eeprom[25]= 196;eeprom[26]=10; eeprom[27]= 76;eeprom[28]= 66;eeprom[29]= 0; 
      eeprom[30]=  0;eeprom[31]= 48;eeprom[32]= 13;eeprom[33]= 18;eeprom[34]=9;eeprom[35]=196;eeprom[36]=10;eeprom[37]=205;eeprom[38]=255;eeprom[39]= 98; 
      eeprom[40]= 67; eeprom[41]= 70;  eeprom[42] =0; eeprom[43]=0;
	  
	  */
	
	
	eeprom[95] =20; eeprom[96] =21;  eeprom[97] =50; eeprom[103] =170; eeprom[103] =190;
	eeprom[131] =0; eeprom[132] =5; eeprom[133] =10;eeprom[138]=25;
	eeprom[139]=19;
	eeprom[140] =255 ; eeprom[141] =220;eeprom[142]=170;eeprom[147] =170;eeprom[148] =220;
	eeprom[150] =1;eeprom[159] =1;eeprom[150] =2;eeprom[159] =3;
	eeprom[160] =255;eeprom[160] =255;
	eeprom[170] =1;eeprom[179] =1;
	eeprom[180] =255;eeprom[189] =255;
	eeprom[109] =20; eeprom[110] =44;  eeprom[111] =60;

}