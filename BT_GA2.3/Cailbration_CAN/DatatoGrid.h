#pragma once
/***********************************************
此类，是将接送到int数据转换为 Cstring 并显示


*****************************************************************/
typedef struct
{
	float Ktset;
	float Ktamb;
	float Ktin;
	float Ktduct;
	float Offset;
}TDPARAMTER;
class CDatatoGrid
{
public:
	CDatatoGrid(void);
	~CDatatoGrid(void);
	int* rtx;
	CStringArray GridString;
	int GridLong;
	// 将rxdata[] 送入 此类，并进行转换
	int TransData(unsigned char* rxt);
	TDPARAMTER m_Tdpa ;
	void  SetTdParameter(TDPARAMTER tdp);
};
