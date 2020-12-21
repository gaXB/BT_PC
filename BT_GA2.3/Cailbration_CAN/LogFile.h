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
	// 创建并打开文件
	bool CreatFile(void);
	// 写记录
	int WriteFile(CString logStr);
	int EndFile(void);
	int m_openok;
};

