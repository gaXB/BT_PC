// SendDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SendDialog.h"
#include "afxdialogex.h"
#include "GWMMN/ApplayLay.h"

// SendDialog 对话框

IMPLEMENT_DYNAMIC(SendDialog, CDialogEx)

SendDialog::SendDialog(CWnd* pParent /*=NULL*/)
	: CDialog(SendDialog::IDD, pParent)
{

}

SendDialog::~SendDialog()
{
}

void SendDialog::DoDataExchange(CDataExchange* pDX)
{
	//	CDialogEx::DoDatachange(pDX);
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_UDSDATA, m_UDSData);
	DDX_Control(pDX, IDC_EDIT_CANDATA, m_CANData);
}


BEGIN_MESSAGE_MAP(SendDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SEND1, &SendDialog::OnBnClickedButtonSend1)
END_MESSAGE_MAP()


// SendDialog 消息处理程序

void	HexToUchar(CString m_StrR,  char *pchar);
uint8  F_N_USDATA_REQ(uint8* ReqData,uint16 Length,uint8 mId_Target);
void SendDialog::OnBnClickedButtonSend1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strWrite;
	GetDlgItem(IDC_EDIT_UDSDATA)->GetWindowTextW(strWrite);
	int length =GetDlgItem(IDC_EDIT_UDSDATA)->GetWindowTextLengthW();
	//Cwnd =  GetWindowTextW(&strWrite);	
	char SendByte[100];
	//SendByte = new char(3+length);

	HexToUchar(strWrite, &SendByte[0]);

	F_N_USDATA_REQ((unsigned char*)SendByte, length/2,ID_DEFINE_TARGET);
}
