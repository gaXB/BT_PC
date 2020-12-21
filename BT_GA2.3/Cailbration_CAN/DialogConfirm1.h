#pragma once
#include "Resource.h"

// CDialogConfirm1 对话框

class CDialogConfirm1 : public CDialog
{
	DECLARE_DYNAMIC(CDialogConfirm1)

public:
	CDialogConfirm1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogConfirm1();

// 对话框数据
	enum { IDD = IDD_DIALOG_FIRM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
