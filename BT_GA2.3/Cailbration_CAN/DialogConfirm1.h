#pragma once
#include "Resource.h"

// CDialogConfirm1 �Ի���

class CDialogConfirm1 : public CDialog
{
	DECLARE_DYNAMIC(CDialogConfirm1)

public:
	CDialogConfirm1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogConfirm1();

// �Ի�������
	enum { IDD = IDD_DIALOG_FIRM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
