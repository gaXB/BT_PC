	
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


    col1->Add(L"ʱ��");                 //�Ҽ����ֶε�ֵ
	col1->Add(L"�����¶�");
	col1->Add(L"ģʽ");
	col1->Add(L"������");
	col1->Add(L"�絲");
	col1->Add(L"A/C");
	col1->Add(L"�������¶�");
		col1->Add(L"�����¶�");
		col1->Add(L"�����¶ȵ�ǰ");
	col1->Add(L"�����¶�");
		col1->Add(L"�����¶ȵ�ǰ");
	col1->Add(L"�����¶�");
	col1->Add(L"ˮ��");
	col1->Add(L"����");


	col1->Add(L"adtin");
	col1->Add(L"adtamb");
	col1->Add(L"adtevp");
	col1->Add(L"adfan");
	col1->Add(L"adsun");//10
	col1->Add(L"���������AD");
	col1->Add(L"res");
	col1->Add(L"res");

	col1->Add(L"TDBase");

	col1->Add(L"���²���");		
	col1->Add(L"���ⲹ��");//20
//	col1->Add(L"�������");

	col1->Add(L"��ǰ������ѹ");
	col1->Add(L"�絲");
	col1->Add(L"ѹ����");
	col1->Add(L"Inlet");
	col1->Add(L"Mode");
	col1->Add(L"���ϵ�ǰAD");
	col1->Add(L"�����趨AD");
    col1->Add(L"���%");

	col1->Add(L"ģʽ��ǰAD");
	col1->Add(L"ģʽ�趨AD");//30

	col1->Add(L"��������");


	//col1->Add(L"TD_TEMP_R");

	col1->Add(L"��������");
	col1->Add(L"���ų����");
		col1->Add(L"����simul");


	col1->Add(L"������¶�ʵ��ֵ");
	col1->Add(L"Ŀ��ֵ");
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