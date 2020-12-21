// CANSetDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CANSetDialog.h"


// CCANSetDialog �Ի���

IMPLEMENT_DYNAMIC(CCANSetDialog, CDialog)

CCANSetDialog::CCANSetDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCANSetDialog::IDD, pParent)
	, m_VecleSpeed(0)
	, m_Twater(0)
{

}

CCANSetDialog::~CCANSetDialog()
{
}

void CCANSetDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_VecleSpeed);
	DDV_MinMaxInt(pDX, m_VecleSpeed, 0, 3000);
	DDX_Text(pDX, IDC_EDIT2, m_Twater);
	DDV_MinMaxInt(pDX, m_Twater, 0, 200);
	DDX_Control(pDX, IDC_EDIT3, m_IDHex);
}


BEGIN_MESSAGE_MAP(CCANSetDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CCANSetDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CCANSetDialog ��Ϣ�������


void CCANSetDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ID = m_IDHex.GetInt();
	CDialog::OnOK();
}
