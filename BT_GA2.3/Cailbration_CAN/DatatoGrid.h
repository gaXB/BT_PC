#pragma once
/***********************************************
���࣬�ǽ����͵�int����ת��Ϊ Cstring ����ʾ


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
	// ��rxdata[] ���� ���࣬������ת��
	int TransData(unsigned char* rxt);
	TDPARAMTER m_Tdpa ;
	void  SetTdParameter(TDPARAMTER tdp);
};
