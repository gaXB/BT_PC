// CheckDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SystemConfig.h"
#include "CheckDialog.h"
extern int TBL_RamDataValid[];
extern void RamRowSet(CStringArray  *col1);

// CCheckDialog 对话框

IMPLEMENT_DYNAMIC(CCheckDialog, CDialog)

CCheckDialog::CCheckDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckDialog::IDD, pParent)
{

}

CCheckDialog::~CCheckDialog()
{
}

void CCheckDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCheckDialog, CDialog)
	ON_BN_CLICKED(IDC_CHECK2, &CCheckDialog::OnBnClickedCheck2)
	ON_BN_CLICKED(IDOK, &CCheckDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO1, &CCheckDialog::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CCheckDialog::OnBnClickedRadio2)
END_MESSAGE_MAP()


// CCheckDialog 消息处理程序
BOOL CCheckDialog::OnInitDialog()
{
	//if (TBL_RamDataValid)
	CStringArray RawString;
	 
	RamRowSet(&RawString);

	for (int i = 0; i< CAILBRATION_RAM_NUM; i++)
	{	
		if (TBL_RamDataValid[i])
		{
			((CButton*)GetDlgItem(IDC_CHECK1+i))->SetCheck(TRUE);
		}
		else
		{
			((CButton*)GetDlgItem(IDC_CHECK1+i))->SetCheck(FALSE);
		}
		((CButton*)GetDlgItem(IDC_CHECK1+i))->SetWindowTextW(RawString.GetAt(i));
	}
	return 0;
}
void CCheckDialog::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CCheckDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i =1; i< CAILBRATION_RAM_NUM-1; i++)
	{
		if(BST_CHECKED==((CButton*)GetDlgItem(IDC_CHECK1+i))->GetCheck())
		{
			TBL_RamDataValid[i] = 1;
		}
		else
		{
			TBL_RamDataValid[i] = 0;
		}
	}
	OnOK();
}

void CCheckDialog::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i< CAILBRATION_RAM_NUM; i++)
	{	
		((CButton*)GetDlgItem(IDC_CHECK1+i))->SetCheck(TRUE);
	}
}

void CCheckDialog::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i< CAILBRATION_RAM_NUM; i++)
	{	
		((CButton*)GetDlgItem(IDC_CHECK1+i))->SetCheck(FALSE);
	}
}
