#include "stdafx.h"
#include "afxwin.h"
//#include "test.h"
#include "Cailbration_CANDlg.h"
#include "ControlCAN.h"
#include "mmsystem.h"
#include "GWMMN/timerout.h"
#include "GWMMN/ApplayLay.h"
#include "SystemConfig.h"

void  L_SendDiagFram(uint8* NetData);
uint8  F_N_USDATA_REQ(uint8* ReqData,uint16 Length,uint8 mId_Target);

static uint8 Mode = SESSION_MODE_DEFAULT;
extern uint8 SessionMode, SecurityState;
extern CCailbration_CANDlg *pWnd;
extern int  TBL_RamDataValid[];
/*void CTestDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	switch(Mode)
	{
	default:
	case SESSION_MODE_DEFAULT:
		SessionMode = SESSION_MODE_EXTERN;
		Mode = SESSION_MODE_EXTERN;

		
		GetDlgItem(IDC_BUTTON1)->SetWindowText("编程模式");

	
		break;
	case SESSION_MODE_PROGRAM:
		SessionMode = SESSION_MODE_DEFAULT;
		Mode = SESSION_MODE_DEFAULT;

		GetDlgItem(IDC_BUTTON1)->SetWindowText("扩展模式");
		break;
	case SESSION_MODE_EXTERN:
		SessionMode = SESSION_MODE_PROGRAM;
		Mode = SESSION_MODE_PROGRAM;

	GetDlgItem(IDC_BUTTON1)->SetWindowText("默认模式");
		break;
	}

}

void CTestDlg::OnBnClickedButtonScurity()
{
	// TODO: 在此添加控件通知处理程序代码
	if (Mode == SESSION_MODE_PROGRAM)
		SecurityState = SECURITY_STATE_SSEED2;
	else SecurityState = SECURITY_STATE_SSEED1;
}


void CTestDlg::OnBnClickedButtonReaddtc()
{
	// TODO: 在此添加控件通知处理程序代码
	uint8 Data[8];
	Data[0] = 0x02;
	Data[1] = 0x19;
	Data[2] = 0x0a;
	L_SendDiagFram(Data);
}

void CTestDlg::OnBnClickedButtonEarse()
{
	// TODO: 在此添加控件通知处理程序代码
	uint8 SendByte[13];
	SendByte[0] = 0x31;
	SendByte[1] = 0x1;
	SendByte[2] = (unsigned char) (ROUTINE_ID_ERASE >>8);
	SendByte[3] = (unsigned char) (ROUTINE_ID_ERASE & 0x00ff);
	SendByte[4] = 0x44;
    SendByte[5] = (uint8)(CODE_START_ADDR >>24);
	SendByte[6] = (uint8)((CODE_START_ADDR >>16) & 0x00ff) ;
    SendByte[7] = (uint8)((CODE_START_ADDR >>8) & 0x0000ff);
	SendByte[8] = (uint8)(CODE_START_ADDR&0x000000ff);

	SendByte[9] = (uint8)(CODE_LENGTH >>24);
	SendByte[10] = (uint8)((CODE_LENGTH >>16) & 0x00ff) ;
    SendByte[11] = (uint8)((CODE_LENGTH >>8) & 0x0000ff);
	SendByte[12] = (uint8)(CODE_LENGTH&0x000000ff);

	F_N_USDATA_REQ(SendByte,13, ID_DEFINE_TARGET);

}

*/

//传输任务
/*
void CTestDlg::OnBnClickedButtonIntegrity()
{
	// TODO: 在此添加控件通知处理程序代码
	uint8 SendByte[4];
	SendByte[0] = 0x31;
	SendByte[1] = 0x01;
	SendByte[2] = (unsigned char) (ROUTINE_ID_INTEGRITY >>8);
	SendByte[3] = (unsigned char) (ROUTINE_ID_INTEGRITY & 0x00ff);

	F_N_USDATA_REQ(SendByte, 4,ID_DEFINE_TARGET);
}


void CTestDlg::OnBnClickedButtonReset()
{
	// TODO: 在此添加控件通知处理程序代码
	uint8 SendByte[2];

	SendByte[0] = 0x11;
	SendByte[1] = 0x01;
	UpdateData(false);

	F_N_USDATA_REQ(SendByte, 2,ID_DEFINE_TARGET);
}



void CTestDlg::OnBnClickedButtonRdid()
{
	// TODO: 在此添加控件通知处理程序代码
	int nDidCode;
	UpdateData(TRUE);
	nDidCode = m_DIDHexCode.GetInt();
		
	uint8 SendByte[3];

	SendByte[0] = 0x22;
	SendByte[1] = nDidCode>>8;
	SendByte[2] = nDidCode&0x00ff;
	F_N_USDATA_REQ(SendByte, 3,ID_DEFINE_TARGET);
    
}

void CTestDlg::OnBnClickedControl()
{
	// TODO: 通讯控制，
	static uint8 u8ControlState;// 0 为 使能  1： 禁止
	uint8 SendByte[3];

	SendByte[0] = 0x28;
	if (u8ControlState)
	{
		u8ControlState = 0;
		GetDlgItem(CONTROL)->SetWindowText("使能");
		SendByte[1] = 0x00;
	}
	else
	{
		u8ControlState = 1;
		GetDlgItem(CONTROL)->SetWindowText("禁止");
		SendByte[1] = 0x03;
	}
	SendByte[2] = 0x01;
	
	UpdateData(false);

	F_N_USDATA_REQ(SendByte, 3,ID_DEFINE_TARGET);
}*/

void CCailbration_CANDlg::DisplayDID(uint8 *Data, uint8 Length)
{	
	//收到 22信号 如果 did = IODID_SYSTEMSTATE 则显示到标定程序中
	uint16 u16DID =Data[0]*256 + Data[1];
    COPYDATASTRUCT   CDS;

	if (Eeprom_Window_Flg == WINDOUS_FLG_RAM)
	{//在 ram 的界面
		if (u16DID == IODID_SYSTEMSTATE)
		{
		}
	}
	else	if (Eeprom_Window_Flg == WINDOUS_FLG_ROM)
	{
		CWnd* ahWnd = FindWindow( NULL,EEPROM_DIALOG1_NAME ) ;
		if(ahWnd)
		{
			CDS.dwData = 0;   
			CDS.cbData = Length; 
			CDS.lpData = Data;
			ahWnd->SendMessage(WM_COPYDATA,0,(LPARAM)&CDS);
		}
	}
	else  if(Eeprom_Window_Flg == WINDOUS_FLG_BOOT)
	{
		CWnd* ahWnd = FindWindow( NULL,BOOT_DIALOG1_NAME ) ;
		

		BootDlg.DisplayDID(Data, Length);
		if(ahWnd)
		{
			CDS.dwData = 0;   
			CDS.cbData = Length; 
			CDS.lpData = Data;
			ahWnd->SendMessage(WM_COPYDATA,0,(LPARAM)&CDS);
		}
	}
//	UpdateData(false);
//	UpdateData(TRUE);
} 

//2e 响应处理
void CCailbration_CANDlg::WriteDID(uint8 *Data, uint8 Length)
{	
	//收到 22信号 如果 did = IODID_SYSTEMSTATE 则显示到标定程序中
	uint16 u16DID =Data[0]*256 + Data[1];
    COPYDATASTRUCT   CDS;

	if (Eeprom_Window_Flg == WINDOUS_FLG_RAM)
	{//在 ram 的界面

	}
	else  if (Eeprom_Window_Flg == WINDOUS_FLG_ROM)
	{
		CWnd* ahWnd = FindWindow( NULL,EEPROM_DIALOG1_NAME ) ;
		if(ahWnd)
		{
			CDS.dwData = 0;   
			CDS.cbData = Length; 
			CDS.lpData = Data;
			ahWnd->SendMessage(WM_COPYDATA,0,(LPARAM)&CDS);
		}
	}
//	UpdateData(false);
//	UpdateData(TRUE);
} 


void CCailbration_CANDlg::DiagTestPresent(void)
{
	m_DiagTestPresent = 1;
}
void CCailbration_CANDlg::ReadDTC(uint8 *Data, uint16 Length)
{	
	//收到 22信号 如果 did = IODID_SYSTEMSTATE 则显示到标定程序中
	uint16 u16DID =Data[0]*256 + Data[1];
    COPYDATASTRUCT   CDS;

	if (Eeprom_Window_Flg == WINDOUS_FLG_RAM)
	{//在 ram 的界面

	}
	else  if (Eeprom_Window_Flg == WINDOUS_FLG_ROM)
	{
		CWnd* ahWnd = FindWindow( NULL,EEPROM_DIALOG1_NAME ) ;
		if(ahWnd)
		{
			CDS.dwData = 0;   
			CDS.cbData = Length; 
			CDS.lpData = Data;
			ahWnd->SendMessage(WM_COPYDATA,0,(LPARAM)&CDS);
		}
	}	
	else  if(Eeprom_Window_Flg == WINDOUS_FLG_BOOT)
	{
		CWnd* ahWnd = FindWindow( NULL,BOOT_DIALOG1_NAME ) ;
		

		BootDlg.ReadDTC(Data, Length);
		if(ahWnd)
		{
			CDS.dwData = 0;   
			CDS.cbData = Length; 
			CDS.lpData = Data;
			ahWnd->SendMessage(WM_COPYDATA,0,(LPARAM)&CDS);
		}
	}
//	UpdateData(false);
//	UpdateData(TRUE);
} 
void Deal_ReadDID(uint8 *Data, uint8 Length)
{
	pWnd->DisplayDID(Data, Length);

}

void Deal_WriteDID(uint8 *Data, uint8 Length)
{
	pWnd->WriteDID(Data, Length);

}

void Deal_ReadDTC(uint8 *Data, uint16 Length)
{
	pWnd->ReadDTC(Data, Length);
}


void CANAPP_TestPresent(void)
{
	pWnd->DiagTestPresent();
}