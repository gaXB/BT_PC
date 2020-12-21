#pragma once

#include "stdafx.h"
#include "StdioFileEx.h"
#include "HexConfig.h"

#define  S19MAX_DATANUM_ONEROW  100
	
unsigned char HexBuf[S19MAX_DATANUM_ONEROW];
struct HEXBUF
{
	unsigned char Data[S19MAX_DATANUM_ONEROW];
	int  Len;
	int  Addr;
}HexBufData;

int JudgeS19_FirstRow(CString FirsRowString)
{
	LPTSTR pch;
	pch = (LPTSTR)(LPCTSTR)FirsRowString;

	if(*pch==L'S')
	{
//		*pch=L'A';	
		pch++;
		
		if( *pch == L'0')
		{	
			return 1;
		}
		else return 0;
	}
	else
	{
		return 0;
	}
}

//解析 S1 开头的那一行HEX文件 

int GetS19HexData(CString m_StrR, unsigned char *Buf)
{
	unsigned char HexA, HexB;
	unsigned char Len = 0;
	unsigned char ChkSum=0;
	int StrLen = m_StrR.GetLength();

	//m_StrR += L'\0';
	LPTSTR pch;
	pch = (LPTSTR)(LPCTSTR)m_StrR;

	pch++;
	pch++;
	StrLen--;
	StrLen--;
	StrLen--;
	while(StrLen)
	{
		HexA =(unsigned char) *pch++;
		StrLen--;
		if (HexA >= 65) HexA -= 55;
		else	HexA -= 48;
		HexB =(unsigned char) *pch++;
		if (HexB >= 65) HexB -= 55;
		else	HexB -= 48;
		Buf[Len++] = HexA*16 + HexB;
		StrLen--;
	}
	if (Buf[0] == Len-1)
	{
		for(int i = 0; i < Len ; i++)
		{
			ChkSum += Buf[i];

		}
		if (ChkSum == 255)
		{
			HexBufData.Addr = Buf[1]*0x100 + Buf[2];
			HexBufData.Len = Buf[0] - 3; 
			CopyMemory(HexBufData.Data, &Buf[3], HexBufData.Len);
			return Len;
		}
		else
		{
			return 0;
		}
	}
	else
		return 0;
}

//解析 S2开头的那一行HEX文件 

int GetS192HexData(CString m_StrR, unsigned char *Buf)
{
	unsigned char HexA, HexB;
	unsigned char Len = 0;
	unsigned char ChkSum=0;
	int StrLen = m_StrR.GetLength();

	//m_StrR += L'\0';
	LPTSTR pch;
	pch = (LPTSTR)(LPCTSTR)m_StrR;

	pch++;
	pch++;

	StrLen--;
	StrLen--;
	StrLen--;
	while(StrLen)
	{
		HexA =(unsigned char) *pch++;
		StrLen--;
		if (HexA >= 65) HexA -= 55;
		else	HexA -= 48;
		HexB =(unsigned char) *pch++;
		if (HexB >= 65) HexB -= 55;
		else	HexB -= 48;
		Buf[Len++] = HexA*16 + HexB;
		StrLen--;
	}
	//buf[0] 长度   buf 1-3 为地址，后面为数据 + checksum

	if (Buf[0] == Len-1)
	{
		for(int i = 0; i < Len ; i++)
		{
			ChkSum += Buf[i];
		}
		if (ChkSum == 255)
		{
			HexBufData.Addr = Buf[1]*0x10000 + Buf[2]*0x100 + Buf[3];
			HexBufData.Len = Buf[0] - 4; 
			CopyMemory(HexBufData.Data, &Buf[4], HexBufData.Len);

			return Len;
		}
		else
		{
			return 0;
		}
	}
	else
		return 0;
}

//解析 S3 开头的那一行HEX文件 

int GetS193HexData(CString m_StrR, unsigned char *Buf)
{
	unsigned char HexA, HexB;
	unsigned char Len = 0;
	unsigned char ChkSum=0;
	int StrLen = m_StrR.GetLength();

	//m_StrR += L'\0';
	LPTSTR pch;
	pch = (LPTSTR)(LPCTSTR)m_StrR;

	pch++;
	pch++;

	StrLen--;
	StrLen--;
	StrLen--;
	while(StrLen)
	{
		HexA =(unsigned char) *pch++;
		StrLen--;
		if (HexA >= 65) HexA -= 55;
		else	HexA -= 48;
		HexB =(unsigned char) *pch++;
		if (HexB >= 65) HexB -= 55;
		else	HexB -= 48;
		Buf[Len++] = HexA*16 + HexB;
		StrLen--;
	}
	//buf[0] 长度   buf 1-3 为地址，后面为数据 + checksum

	if (Buf[0] == Len-1)
	{
		for(int i = 0; i < Len ; i++)
		{
			ChkSum += Buf[i];
		}
		if (ChkSum == 255)
		{
			HexBufData.Addr = Buf[1]*0x1000000 + Buf[2]*0x10000 + Buf[3]*0x100 +  Buf[4];
			HexBufData.Len = Buf[0] - 5; 
			CopyMemory(HexBufData.Data, &Buf[5], HexBufData.Len);

			return Len;
		}
		else
		{
			return 0;
		}
	}
	else
		return 0;
}

int JudgeS19_FollowRow(CString m_StrR)
{
	LPTSTR pch;
	pch = (LPTSTR)(LPCTSTR)m_StrR;

	if (*pch == L'S')
	{
		pch++;
		if(*pch == L'1')
		{
			if (GetS19HexData(m_StrR, HexBuf))
			{
				return 1;
			}
			else
				return 0;
		}
		else if(*pch == L'9')
		{
			return 255;
		}
		else if(*pch == L'8')
		{
			return 255;
		}
		else if(*pch == L'7')
		{
			return 255;
		}
		else if(*pch == L'2')
		{
			if (GetS192HexData(m_StrR, HexBuf))
			{
				return 1;
			}
			else
				return 0;
		}
		else if(*pch == L'3')
		{
			if (GetS193HexData(m_StrR, HexBuf))
			{
				return 1;
			}
			else
				return 0;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

//判断此路径的文件是否正确
//判断此文件的格式 1 开头S0 2 S1 的checksum  3 S9 结尾
//return 1 正确，0 错误

int JudgeS19(CString filename)
{
//以上为得到strName 路径
	CStdioFileEx myFile; 
	CFileException fileException; 
	if(!myFile.Open(filename,CFile::modeCreate|CFile::modeNoTruncate|CFile::typeText|CFile::modeReadWrite),&fileException) 
	{ 
		TRACE("Can't open file %s, error = %u\n",filename,fileException.m_cause); 
		///return 0;
	} 

	if (filename == L"")
	{
		return 0;
	}
	//m_FileName = strName;
	CString	stf =L"";
	CString DisPlayStr = L"";

	myFile.ReadString(stf);
	
	//dudge first row
	if (stf != L"")
	{
		if (JudgeS19_FirstRow(stf))
		{

		}
		else
		{
			return 0;
		}
	}
	else
	{ //file format wrong
		return 0;
	}
	
	int JudgeResult = 0;  //0 wrong 1:s1 data 255: s9 end
	do // judge follow row 
	{
		JudgeResult = 0;
		myFile.ReadString(stf);
		if(stf != L"")
		{
			JudgeResult = JudgeS19_FollowRow(stf);
		}
		else
		{
			return 0;
		}
	}while (JudgeResult == 1);
	
	if (JudgeResult == 255)
	{
//		*filename = strName;
		myFile.Close();
		return 1;
	}
	else
	{
		myFile.Close();
		return 0;
	}
}

int ReadS19(unsigned char (*p)[S19MAX_DATANUM_ONEROW], CString filename)
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
	
	int JudgeResult = 0;  //0 wrong 1:s1 data 255: s9 end
	do // judge follow row 
	{
		JudgeResult = 0;
		myFile.ReadString(stf);
		if(stf != L"")
		{
			JudgeResult = JudgeS19_FollowRow(stf);
			if(JudgeResult == 1)
			{
				//p[LenHex][HEXDATA_COL_MAXNUM] = HexBuf[]
				CopyMemory(p[LenHex]+1 ,HexBuf, S19MAX_DATANUM_ONEROW);
				p[LenHex][0] = 0xFA;
			}
			else if(JudgeResult == 255)
			{
				//CopyMemory(p[LenHex]+1 ,HexBuf, S19MAX_DATANUM_ONEROW);
				p[LenHex][0] = 0xFF;
				*(p[LenHex]+1) = 0x03;
			} 
		}
		else
		{
			return 0;
		}
		LenHex++;
		if(LenHex > 2500)
		{
			return 1;
		}
	}while (JudgeResult == 1);

	if (JudgeResult == 255)
	{
		myFile.Close();
		return LenHex;
	}
	else
	{
		myFile.Close();
		return 0;
	}
}

extern volatile  unsigned long  CRC32TABLE[];
//返回区域
int ReadS19TOData(HEXDATA_ALL *sHexData , CString filename)
{
	CStdioFileEx myFile; 
	CFileException fileException; 
	HEXREGION *pHexRegion = sHexData->sRegionMsg;

	int	LenHex = 0;  
	int nRegion = 0;

	sHexData->TotalLength = 0;
	if(!myFile.Open(filename,CFile::modeCreate|CFile::modeNoTruncate|CFile::typeText|CFile::modeReadWrite),&fileException) 
	{ 
		TRACE("Can't open file %s, error = %u\n",filename,fileException.m_cause); 
		//return 0;
	} 

	//m_FileName = strName;
	CString	stf =L"";
	CString DisPlayStr = L"";

	myFile.ReadString(stf);
	
	int JudgeResult = 0;  //0 wrong 1:s1 data 255: s9 end
	pHexRegion->BeginAddr =  0xffff0000;
	pHexRegion->HexDataLen = 0;
	do // judge follow row 
	{
		JudgeResult = 0;
		myFile.ReadString(stf);
		if(stf != L"")
		{
			JudgeResult = JudgeS19_FollowRow(stf);
			if(JudgeResult == 1)
			{
				//p[LenHex][HEXDATA_COL_MAXNUM] = HexBuf[]

				if (HexBufData.Addr ==  pHexRegion->BeginAddr + pHexRegion->HexDataLen ) //长度过长使用下一页
 				{//下一个记录长度 ==  sHexData 当前记录的点。  sHexData->BeginAddr初始化为0xffff0000不会等于所以下个空间开始 

				}
				else
				{// 下一个 hexdata
					pHexRegion = &(sHexData->sRegionMsg[nRegion]);
					nRegion++;
					if (nRegion > HEX_MAX_REGION) return HEXDATA_ERROR_REGION;
					pHexRegion->BeginAddr = HexBufData.Addr;
					pHexRegion->HexDataLen = 0;
					pHexRegion->pData = &(sHexData->HexData[sHexData->TotalLength]);
				}
				if (pHexRegion->HexDataLen + HexBufData.Len >= HEXDATA_MAXLEN)
				{
					JudgeResult = 2;
					break;
				}
				else
				{
					CopyMemory( &(pHexRegion->pData[pHexRegion->HexDataLen]) ,HexBufData.Data, HexBufData.Len);
					pHexRegion->HexDataLen += HexBufData.Len;
					sHexData->TotalLength += HexBufData.Len;
				}
			}
			else if(JudgeResult == 255)
			{
				//CopyMemory(p[LenHex]+1 ,HexBuf, S19MAX_DATANUM_ONEROW);
				//p[LenHex][0] = 0xFF;
				//*(p[LenHex]+1) = 0x03;
			} 
		}
		else
		{
			break;
		}
	}while (JudgeResult == 1);

	unsigned	long crcValue;

	if (JudgeResult == 255)
	{
		sHexData->i16RegionNum = nRegion;
		for (int i = 0; i< nRegion; i++)
		{
			crcValue = 0xFFFFFFFF;  

			for (int j = 0; j < sHexData->sRegionMsg[i].HexDataLen; j++)
			{
				crcValue = (crcValue << 8) ^ CRC32TABLE[((crcValue >>24 ) & 0xFF) ^sHexData->sRegionMsg[i].pData[j]]; 
			}
			

			crcValue =~crcValue;
			sHexData->sRegionMsg[i].regionchecksum = crcValue;
		}
		myFile.Close();
		return nRegion;
	}
	else
	{
		myFile.Close();
		if (JudgeResult == 2)
		{
			return 255;
		}
		else
		{
			return 0;
		}

		
	}
}