#pragma once
#include "afxwin.h"
#include "Resource.h"

// CCheckDialog 对话框

class CCheckDialog : public CDialog
{
	DECLARE_DYNAMIC(CCheckDialog)

public:
	CCheckDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCheckDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
};
