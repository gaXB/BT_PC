#pragma once

#include "resource.h"		// ������
#include "edithexinput.h"
// CCANSetDialog �Ի���

class CCANSetDialog : public CDialog
{
	DECLARE_DYNAMIC(CCANSetDialog)

public:
	CCANSetDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCANSetDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_CANSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_VecleSpeed;
	int m_Twater;
	int m_ID;
	// ID
	CEditHexInput m_IDHex;
	afx_msg void OnBnClickedOk();
};
