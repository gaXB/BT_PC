#pragma once

#include "resource.h"		// 主符号
#include "edithexinput.h"
// CCANSetDialog 对话框

class CCANSetDialog : public CDialog
{
	DECLARE_DYNAMIC(CCANSetDialog)

public:
	CCANSetDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCANSetDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_CANSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_VecleSpeed;
	int m_Twater;
	int m_ID;
	// ID
	CEditHexInput m_IDHex;
	afx_msg void OnBnClickedOk();
};
