#pragma once
#include "afxwin.h"
#include "GWMMN/NetLaye.h"
#include "GWMMN/timerout.h"
#include "EditHexInput.h"
#include "HexConfig.h"
#include "dragedit.h"
// CBootDialog 对话框
//hex 文件 的行  列
#define   HEXDATA_ROW_MAXNUM       3000
#define   HEXDATA_COL_MAXNUM		100

enum  LOAD_TASK_ID
{	
	LOAD_TASK_NULL = 0,
	LOAD_TASK_ERASE,
	LOAD_TASK_REQDOWNL ,
	LOAD_TASK_TRANSDATA ,
	LOAD_TASK_EXITTRANS ,
	LOAD_TASK_SUCCESS,
	LOAD_TASK_NEXTREGION,
	LOAD_TASK_INTEGRITY,
	LOAD_TASK_PRINT,
	LOAD_TASK_FAILED,
	LOAD_TASK_ID ,
};


//#define CODE_END_ADDR   0x0000c980        // 64K 0xffff  最后 4k 作为flash 模拟ee
//#define CODE_START_ADDR 0x00007c00         // 0 -0x3fff   16k 为bootloader 区域
#define   MCU_KEA64     0
#define  ID_DEFINE_TARGET  0x44
#if    MCU_KEA64
#define  CODE_END_ADDR   0x0000EFFC
#else
#define CODE_END_ADDR   0x0001BFFC         // 64K 0xffff  最后 4k 作为flash 模拟ee
#endif

#define CODE_START_ADDR 0x00004000         // 0 -0x3fff   16k 为bootloader 区域
#define CODE_LENGTH     (CODE_END_ADDR - CODE_START_ADDR)


class CBootDialog : public CDialog
{
	DECLARE_DYNAMIC(CBootDialog)

public:
	CBootDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBootDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_BOOT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// //s19 路径
	CString m_FileName;
	afx_msg void OnBnClickedButton1();
	// //信息栏
	CListBox m_ListInfo;
	void ShowInfo(CString str, int code);

	 //s19  ram
	int         m_HexDataLen;
	int         m_Data1Addr, m_Data2Addr, m_Data3Addr,
				m_Data1Num, m_Data2Num, m_Data3Num;
	uint8       m_HexData1[200000];
	uint8       m_HexData2[300];
	unsigned  int      m_FlashChkSum;
	uint8       m_HexData[HEXDATA_ROW_MAXNUM][HEXDATA_COL_MAXNUM];
	uint8       nRegion;
	uint8       nFLashRegion;
	int         m_BLSendLen, m_BLSendNum;
	uint8       m_LoadTask;
	uint8       m_nRegion;

	//
	int SortRegion(void);
	afx_msg void OnBnClickedButtonSessionmode();
	afx_msg void OnBnClickedButtonScurity();
	afx_msg void OnBnClickedButtonReaddtc();
	afx_msg void OnBnClickedButtonComcontrol();
	afx_msg void OnBnClickedButtonEarse();
	afx_msg void OnBnClickedButtonDownload();
	void  DisplayDID(uint8 *Data, uint8 Length);
	void  ReadDTC(uint8 *Data, uint16 Length);
	//
	void Deal_TransSever(void);
	afx_msg void OnBnClickedButtonIntegrity();
	afx_msg void OnBnClickedButtonReset();
	CEditHexInput m_DIDHexCode;
	CEdit m_Edite22Message;
	CString m_22Message;
	afx_msg void OnBnClickedButtonRdid();
	BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnBnClickedButtonDisplayEn();
	bool m_DisplayEn;
	afx_msg void OnBnClickedButtonFastload();
	// 一键下载的状态
	int m_FastLoadState;
	void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonWdid();
	afx_msg void OnBnClickedButtonIocontrol();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	CDragEdit m_DragEdit;
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnBnClickedOk();
	// file 内容
	CDragEdit m_File;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButtonReadaddr();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButtonReaddtc2();
};
