	
#include "stdafx.h"
#include "SystemConfig.h"
//#include "ToolBar.h"
//#include "EepromDialog.h"
//#include"StdioFileEx.h"

int TBL_RamDataValid[CAILBRATION_RAM_NUM] =
{
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};
void RamRowSet(CStringArray  *col1)
{


    col1->Add(L"时间");                 //我加入字段的值
	col1->Add(L"设置温度");
	col1->Add(L"模式");
	col1->Add(L"内外气");
	col1->Add(L"风挡");
	col1->Add(L"A/C");
	col1->Add(L"副设置温度");
		col1->Add(L"车内温度");
		col1->Add(L"车内温度当前");
	col1->Add(L"车外温度");
		col1->Add(L"车外温度当前");
	col1->Add(L"蒸发温度");
	col1->Add(L"水温");
	col1->Add(L"车速");


	col1->Add(L"adtin");
	col1->Add(L"adtamb");
	col1->Add(L"adtevp");
	col1->Add(L"adfan");
	col1->Add(L"adsun");//10
	col1->Add(L"计算后阳光AD");
	col1->Add(L"res");
	col1->Add(L"res");

	col1->Add(L"TDBase");

	col1->Add(L"外温补偿");		
	col1->Add(L"阳光补偿");//20
//	col1->Add(L"错误代码");

	col1->Add(L"当前风量电压");
	col1->Add(L"风挡");
	col1->Add(L"压缩机");
	col1->Add(L"Inlet");
	col1->Add(L"Mode");
	col1->Add(L"左混合当前AD");
	col1->Add(L"左混合设定AD");
    col1->Add(L"混合%");

	col1->Add(L"模式当前AD");
	col1->Add(L"模式设定AD");//30

	col1->Add(L"风量保护");


	//col1->Add(L"TD_TEMP_R");

	col1->Add(L"吹面出风口");
	col1->Add(L"吹脚出风口");
		col1->Add(L"车内simul");


	col1->Add(L"出风口温度实际值");
	col1->Add(L"目标值");
	col1->Add(L"AM_FF0");
	col1->Add(L"AM_P");
	col1->Add(L"AM_I");
	col1->Add(L"AM_Hfs");
	col1->Add(L"Hmix");
	col1->Add(L"");
	col1->Add(L"");
	col1->Add(L"");
	col1->Add(L"");
	col1->Add(L"");


	col1->Add(L"");
	col1->Add(L"");
	col1->Add(L"");
	col1->Add(L"");
	col1->Add(L"");
	col1->Add(L"");
	col1->Add(L"");
	col1->Add(L"");
	col1->Add(L"");

	col1->Add(L"");
	col1->Add(L"");
	col1->Add(L"");
	col1->Add(L"");
	col1->Add(L"");
	col1->Add(L"");
	col1->Add(L"");
	col1->Add(L"");

	
	ASSERT(col1->GetSize()>=CAILBRATION_RAM_NUM);
}