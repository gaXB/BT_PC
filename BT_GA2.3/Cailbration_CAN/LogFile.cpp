#include "StdAfx.h"
#include "LogFile.h"
#include "StdioFileEx.h"

CLogFile::CLogFile(void)
	: m_FileName(_T(""))
{
	m_openok= 0;
}


CLogFile::~CLogFile(void)
{
}


// ���������ļ�
bool CLogFile::CreatFile(void)
{

	CTime time = CTime::GetCurrentTime();   ///����CTime����
	int m_nYear = time.GetYear();      ///��
	int m_nMonth = time.GetMonth();      ///��
	int m_nDay = time.GetDay();      ///��
	int m_nHour = time.GetHour();      ///Сʱ
	int m_nMinute = time.GetMinute();   ///����
	int m_nSecond = time.GetSecond();   ///��
	static int num;
	num++;
//	m_FileName.Format(_T("Log%04d%02d%02d%02d%02d%02d.txt"), m_nYear,m_nMonth,m_nDay, m_nHour,m_nMinute,m_nSecond);
	m_FileName.Format(_T("LogFile%d.txt"), num);
	if (m_openok == 1)
	{
		myFile.Close();
		m_openok = 2;
	}

	if(!myFile.Open(m_FileName,  CFile::modeCreate|CFile::typeText|CFile::modeReadWrite),&fileException) 
	{ 
		TRACE("Can't open file %s, error = %u\n",m_FileName,fileException.m_cause); 
	}

	CString str;
	str.Format(_T("��ʼ��¼%04d%02d%02d%02d%02d%02d\n"),m_nYear,m_nMonth,m_nDay, m_nHour,m_nMinute,m_nSecond);
	myFile.WriteString(str); 
	if (FileIsOK())
	{
		m_openok = 1;
	}
	else
	{
		m_openok = 0;
	}
	
	//myFile.Close();
	//myFile.SeekToEnd();
/*	myFile.WriteString(L"ʱ��,"); 

	for(int	j=0;j < i16TotalRowNum; j++)
	{
	stf = RowName.GetAt(j);
	myFile.WriteString(stf+L","); 
	}
		
	myFile.WriteString(L"\n"); 
	myFile.Close();*/
	return false;
}


// д��¼
int CLogFile::WriteFile(CString logStr)
{
	if (m_openok == 1)
	{
		myFile.WriteString(logStr); 
		myFile.WriteString(L"\n"); 
	}
	return 0;
}

bool CLogFile::FileIsOK(void)
{
	return myFile.m_pStream == NULL ? FALSE:TRUE;
}

int CLogFile::EndFile(void)
{
	if (m_openok == 1)
	{
		myFile.WriteString(L"������¼\n"); 
		myFile.Close();
		m_openok = 0;
	}
	return 0;
}
