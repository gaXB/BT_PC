#pragma once
#include "afxwin.h"
#include "Resource.h"

// CCheckDialog �Ի���

class CCheckDialog : public CDialog
{
	DECLARE_DYNAMIC(CCheckDialog)

public:
	CCheckDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCheckDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
};
