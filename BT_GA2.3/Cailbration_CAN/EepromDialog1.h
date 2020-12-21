#pragma once

//#include "SerialPort.h"
///#include "DomodalDlg.h"
#include "afxwin.h"
#include "Resource.h"
#include "DomodalDlg.h"
#include "GridCtrl_src/GridCtrl.h"
#define		ID_TIMER_TIME		1
#define		ID_TIMER_COMX		2
#define		MAX_DATA_NUM	  192 
#define		WM_RECIVE_TOEEPROM		WM_USER+102
#define     EEPROM_DIALOG1_NAME    L"参数设定"
#define     BOOT_DIALOG1_NAME    L"Bootload"

#define     PARAMETER_ACCNUM    126
#define		TIMER_ID_TIME		1
#define		MAX_EEPROMNUM	   240    //EEPROM 的数据个数
#define      MAX_EEPROMNUM_LINE  PARAMETER_ACCNUM  //显示的个数
#define     PARAMETER_LENGTH   160

#define   CAILBRATION_IODID_RAM       0xFDDE
// CEepromDialog1 对话框
typedef struct
{
	int   Number;
	CString Name;
	CString Unit;
	CString	Accuracy;
	CString Area;
}EEPROMDATA; 

#define   WORK_STATE_READ     1 
#define   WORK_STATE_READOK   2
#define   WORK_STATE_WRITE   3
#define   WORK_STATE_WRITEOK   4

class CEepromDialog1 : public CDialog
{
	DECLARE_DYNAMIC(CEepromDialog1)
public:
		CGridCtrl  m_Grid;
		CDomodalDlg* ComP;
		int     Eeprom[MAX_EEPROMNUM];
		int		MaxItem;
		
		//串口数据处理
		unsigned char CheckSum;
		int rxdata_int[MAX_DATA_NUM];
		int     config;
		int     nstat;
	    BYTE send[MAX_EEPROMNUM];
public:
	CEepromDialog1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEepromDialog1();
	virtual BOOL OnInitDialog();
// 对话框数据
	enum { IDD = IDD_DIALOG_EEPROM1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedOk();
	int m_ComID;
	CStatic m_COM_POCK;
	void SetCom(CDomodalDlg* ComDlg);
	afx_msg	LRESULT  OnComMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonRead();
	void RefereshDisplayData(void);
	void Treat_RecRam(unsigned char bt);
	// 错误信息
	CString error_message;

	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();

	BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
};



