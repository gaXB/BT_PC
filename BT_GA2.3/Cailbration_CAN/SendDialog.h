#pragma once

#include "afxwin.h"
#include "Resource.h"
// SendDialog 对话框

class SendDialog : public CDialog
{
	DECLARE_DYNAMIC(SendDialog)

public:
	SendDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SendDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_SENDMESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_UDSData;
	CEdit m_CANData;
	afx_msg void OnBnClickedButtonSend1();
};
