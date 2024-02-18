#pragma once
#include "afxwin.h"
#include "Resource.h"
#include "edithexinput.h"
#include "dragedit.h"

// CDomodalDlg 对话框
typedef struct 
{
	int Band;
	int FunID;
	int PhyID;
	int ECUID;
	int FramSize;
	unsigned int mask1;
	unsigned int mask2;
	unsigned int RCID_INTERGRITY;
	unsigned int RCID_DEPENDENCE;
	unsigned int RCID_ERASE;
	unsigned int RCID_PRECONDITIONS;
	CString dllPath;
    unsigned char u8Security2;
    unsigned char PrintType;
}SETCONFIG;

class CDomodalDlg : public CDialog
{
	DECLARE_DYNAMIC(CDomodalDlg)

public:
	CDomodalDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDomodalDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DOMODAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCbnSelchangeCombo1();
	// 21
	CComboBox m_combox_com;
	virtual BOOL OnInitDialog();
	int COMID;
	// 波特率
	CComboBox m_Baudrate;
	int BOTID;


	afx_msg void OnBnClickedButtonOpenCom();
	// com  open flag
	int m_Enable;
	afx_msg void OnBnClickedButtonCloseCom();
	afx_msg void OnCbnSelchangeCombo2();
	int m_ScurityType;
	unsigned char m_ScurityLevel2;
	CComboBox m_ProjectID;
	int ProjectID;
	CEditHexInput m_PhysicID;
	CEditHexInput m_FunCtionID;
	CEditHexInput m_ECUID;
	SETCONFIG SetConfig;
	afx_msg void OnCbnSelchangeCombo3();
	// 帧格式
	CComboBox m_FramSize;
	CEditHexInput m_Mask1;
	CEditHexInput m_Mask2;
	afx_msg void OnBnClickedButtonOpendll();
	CDragEdit m_ScurityPath;
	afx_msg void OnEnChangeEdit4();
	CEditHexInput m_IntergrityID;
	CEditHexInput m_dependenceID;
	CEditHexInput m_RCID_ERASE;
	CEditHexInput m_Precondition;
	afx_msg void OnEnChangeEdit2();
	// 指纹类型
	int m_PrintType;
};
