#pragma once

#include "StdioFileEx.h"

class CLogFile
{
public:
	CLogFile(void);
	~CLogFile(void);
	CString m_FileName;
	CStdioFileEx myFile; 
	CFileException fileException; 
	// ���������ļ�
	bool CreatFile(void);
	// д��¼
	int WriteFile(CString logStr);
	int EndFile(void);
	int m_openok;
};

