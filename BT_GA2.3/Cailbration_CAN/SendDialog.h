#pragma once

#include "afxwin.h"
#include "Resource.h"
// SendDialog �Ի���

class SendDialog : public CDialog
{
	DECLARE_DYNAMIC(SendDialog)

public:
	SendDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SendDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_SENDMESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_UDSData;
	CEdit m_CANData;
	afx_msg void OnBnClickedButtonSend1();
};
