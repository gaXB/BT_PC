// DialogConfirm1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DialogConfirm1.h"


// CDialogConfirm1 �Ի���

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


// CDialogConfirm1 ��Ϣ�������

void CDialogConfirm1::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CDialogConfirm1::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}
