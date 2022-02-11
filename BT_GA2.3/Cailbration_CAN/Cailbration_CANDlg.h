// Cailbration_CANDlg.h : 头文件
//


#if !defined(AFX_TESTDLG_H__0B453A60_AE2F_4119_AF51_CC52D47F30DF__INCLUDED_)
#define AFX_TESTDLG_H__0B453A60_AE2F_4119_AF51_CC52D47F30DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


#pragma once

#include "GridCtrl_src/GridCtrl.h"
#include "TestListBox.h"
#include "ControlCAN.h"
#include "DomodalDlg.h"
#include "GWMMN/timerout.h"
#include "DatatoGrid.h"
#include "afxwin.h"
#include "EepromDialog1.h"
#include "BootDialog.h"
#include "CheckDialog.h"
#include "CANSetDialog.h"
#include "SendDialog.h"
#include "ado.h"
#include "HexConfig.h"
#include "GWMMN/APPlayLay.h"
#include "LogFile.h"

#pragma warning(disable:4786)
#include <map>
#include "afxwin.h"
#include "afxcmn.h"
using namespace std;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib,"controlcan.lib")

//#include "comWnd.h"
//#include "DatatoGrid.h"
#define ID_BUTTON_TOOL1    501
#define ID_BUTTON_TOOL2    502
#define ID_BUTTON_TOOL3    503
#define ID_BUTTON_TOOL4    504
#define ID_BUTTON_TOOL5    505
#define ID_BUTTON_TOOL6    506

#define ID_BUTTON_TOOL10 510

#define  WINDOUS_FLG_RAM    0
#define  WINDOUS_FLG_ROM    1
#define  WINDOUS_FLG_BOOT   2

//显示计数定时器
#define   ID_TIMER_COM   2

#define ID_TIMER_SHOWCNT 1
#define ID_TIMER_SHOWCNT_TM 10 //1秒定时

#define IODID_SYSTEMSTATE  0xFDDD  

typedef struct _dlgControlTag 
{
    int iId;
    int iFlag;
    int iPercent;
} DLGCTLINFO, *PDLGCTLINFO;

enum
{
    MOVEX = 0,
    MOVEY,
    MOVEXY,
    ELASTICX,
    ELASTICY,
    ELASTICXY
};

typedef struct  
{
    int iVeicleSpeed;
    int iTWater;
    int bSend;
	int id;
} CANSET;

#define     _COL_Width  40
#define     _ROW_Width  75

//hex 文件 的行  列
//#define   HEXDATA_ROW_MAXNUM       2000
//#define   HEXDATA_COL_MAXNUM		100
#define  BT_STATE_START      0
#define  BT_STATE_PM         1
#define  BT_STATE_SCURITY    2
//#define  BT_STATE_ERASE      3
#define  BT_STATE_LOADE      4
#define  BT_STATE_INTERGRITY     5
#define  BT_STATE_DEPENDENCE      6
#define  BT_STATE_RESET         7
#define  BT_STATE_END           8
#define	 BT_STATE_EXTERNMODE    9
#define  BT_STATE_SUCCESS       10
#define  BT_STATE_READCHECKSUM         11
#define  BT_STATE_READCHECKSUMWAIT      12
#define  BT_STATE_PRECONDITIONS         13
// CCailbration_CANDlg 对话框
class CCailbration_CANDlg : public CDialog
{
// 构造
public:
	CCailbration_CANDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CAILBRATION_CAN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CMenu   menu;
	CToolBar m_Toolbar;
	CReBar m_Rebar;
	CStatusBar m_wndStatusBar;
	CImageList m_ImageList;

	CEepromDialog1 EeDlg;

	
    int m_iClientWidth;  //  对话框client区域的宽度
    int m_iClientHeight;  //  对话框client区域的高度
    int m_iMinWidth;  //  对话框的最小宽度
    int m_iMinHeight;  //  对话框的最小高度
    PDLGCTLINFO m_pControlArray;  //  控件信息数组指针
    int m_iControlNumber;  //  设置控件信息的控件个数
    BOOL m_bShowSizeIcon;  //  是否显示表示可改变大小的图标

	
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	BOOL SetControlProperty(PDLGCTLINFO lp, int nElements);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	CDomodalDlg  m_ComDlg;
	afx_msg void OnBnClickedOk();
	CString m_EditeString;
	afx_msg void OnEnChangeEdit1();
	CTestListBox	m_ListInfo;
	void ShowInfo(CString str,int code);
	void ShowMainInfo(CString str, int code);
	afx_msg void OnBnClickedButtonBegin();
	void DisplayDID(uint8 *Data, uint8 Length);
	void WriteDID(uint8 *Data, uint8 Length);
	void ReadDTC(uint8 *Data, uint8 Length);
	//void ReadDTC(uint8 *Data, uint8 Length);
	void  OnToolbar1(void);
	void  OnToolbar2(void);
	void  OnToolbar3(void);
	void  OnToolbar4(void);
	void  SendCanMessage(VCI_CAN_OBJ* vco);
	static UINT ReceiveThread(void *param);
	static UINT SendThread(void *param);
    void  Deal_TransSever(void);
	void  SetTransSize(int size);
	int i16TransSize;
	//can 数据
	int m_recvcount;
	CANSET sCANSet;

	 //s19  ram
	int         m_HexDataLen;
	int         m_Data1Addr, m_Data2Addr, m_Data3Addr,
				m_Data1Num, m_Data2Num, m_Data3Num;
	uint8       m_HexData1[200000];
	uint8       m_HexData2[300];
	int      m_FlashChkSum;
	uint32 crcValue;  
//	uint8       m_HexData[HEXDATA_ROW_MAXNUM][HEXDATA_COL_MAXNUM];
	uint8       nRegion;
	uint8       nFLashRegion;
	int         m_BLSendLen, m_BLSendNum;
	uint8       m_LoadTask;
	uint8       m_nRegion;
	CEdit m_Edite22Message;
	CString m_22Message;
	CButton m_CButtonBegin;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonSave();
	int m_RxRow;
	CBootDialog BootDlg;
    void InitGrid(void);
	//
	int Eeprom_Window_Flg;
	afx_msg void OnMenuCom();
	afx_msg void OnMenuBoot();

	afx_msg void OnLbnSelchangeListInfo();
	void  BDialog_ShowInfo(CString str);
	int m_nStep;
	afx_msg void OnMenueAbout();
	afx_msg void OnMenueEeprom();
	SETCONFIG  sCanCofig;
	// 信息栏
	CListBox m_MessageList;
	// s19地址
	CDragEdit m_S19File;
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButtonOpens19();
	HEXDATA_ALL		sCodeData;
	//FLASH DRIVE
	CDragEdit m_FlashDriveFile;
	HEXDATA_ALL     sFlashDrive;

	//
	void DiagTestPresent(void);
	uint8 m_DiagTestPresent;
	void ReceiveAPPData(A_PDU* APData);
	void DiagRecNRC(uint8* aData);
	uint8 m_BootLoadState;
	void BootLoad_ONTIMER(uint8 nTimer);
	void BootLoad_DoingNext(void);
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult);
	// 下载进程
	CProgressCtrl m_LoadProgress;
	uint8 m_MessagePoint;
	uint32 u32OldCheckSum;
	uint32 u32NewCheckSum;
	CLogFile m_LogFile;
	afx_msg void OnEnChangeEdit13();
	afx_msg void OnStnClickedStaticMessage();
	afx_msg void OnBnClickedButtonOpens20();
};

#endif 
