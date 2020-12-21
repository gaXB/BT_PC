// DialogConfirm1.cpp : 实现文件
//

#include "stdafx.h"
#include "DialogConfirm1.h"


// CDialogConfirm1 对话框

IMPLEMENT_DYNAMIC(CDialogConfirm1, CDialog)

CDialogConfirm1::CDialogConfirm1(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogConfirm1::IDD, pParent)
{

}

CDialogConfirm1::~CDialogConfirm1()
{
}

void CDialogConfirm1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogConfirm1, CDialog)
	ON_BN_CLICKED(IDOK, &CDialogConfirm1::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogConfirm1::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDialogConfirm1 消息处理程序

void CDialogConfirm1::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CDialogConfirm1::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
