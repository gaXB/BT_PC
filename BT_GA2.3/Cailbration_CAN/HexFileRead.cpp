#pragma once

#include "stdafx.h"
#include "StdioFileEx.h"



int ReadHexFile(unsigned char *pData, CString filename)
{
	CStdioFileEx myFile; 
	CFileException fileException; 
	int	LenHex = 0;  
	if(!myFile.Open(filename,CFile::modeCreate|CFile::modeNoTruncate|CFile::typeText|CFile::modeReadWrite),&fileException) 
	{ 
		TRACE("Can't open file %s, error = %u\n",filename,fileException.m_cause); 
		//return 0;
	} 

	//m_FileName = strName;
	CString	stf =L"";
	CString DisPlayStr = L"";

	myFile.ReadString(stf);
	
	unsigned char HexA, HexB;
	int Len = 0;
	unsigned char ChkSum=0;
	int StrLen = stf.GetLength();

	//m_StrR += L'\0';
	LPTSTR pch;
	pch = (LPTSTR)(LPCTSTR)stf;

	
	while(StrLen)
	{
		HexA =(unsigned char) *pch++;
		StrLen--;
		if (HexA >= 'a')  HexA = HexA - 'a'+10;
		else if (HexA >= 65) HexA -= 55;
		else	HexA -= 48;

		HexB =(unsigned char) *pch++;
		if (HexB >= 'a')  HexB = HexB - 'a'+10;
		else if (HexB >= 65) HexB -= 55;
		else	HexB -= 48;
		pData[Len++] = HexA*16 + HexB;
		StrLen--;
	}
	
	myFile.Close();

	return Len;
}
