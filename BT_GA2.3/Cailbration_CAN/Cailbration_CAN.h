// Cailbration_CAN.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCailbration_CANApp:
// �йش����ʵ�֣������ Cailbration_CAN.cpp
//

class CCailbration_CANApp : public CWinApp
{
public:
	CCailbration_CANApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
//	afx_msg void OnMenueAbout();
};

extern CCailbration_CANApp theApp;