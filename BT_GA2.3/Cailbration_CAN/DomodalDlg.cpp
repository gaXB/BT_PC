// DomodalDlg.cpp : 实现文件
//

#include "stdafx.h"
//#include "ToolBar.h"
#include "DomodalDlg.h"
#include "ADO.h"

// CDomodalDlg 对话框

IMPLEMENT_DYNAMIC(CDomodalDlg, CDialog)

CDomodalDlg::CDomodalDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDomodalDlg::IDD, pParent)
	, COMID(0)
	, BOTID(1)
	, m_Enable(0)
	, m_ScurityType(5)
	, m_ScurityLevel2(5)
{
	SetConfig.ECUID = 0X7a1;
	SetConfig.PhyID = 0x721;
	SetConfig.FunID = 0x7df;
	SetConfig.Band = 500;
	SetConfig.FramSize = 0;
	SetConfig.mask1 = 0x51c5635c;
	SetConfig.mask2 = 0x51f5635f;
	SetConfig.dllPath = L"SECURITY_SAIC_DLL.dll";
	SetConfig.RCID_DEPENDENCE = 0xFF01;
	SetConfig.RCID_INTERGRITY = 0xDFFF;
	SetConfig.RCID_ERASE = 0xFF00;
	SetConfig.RCID_PRECONDITIONS = 0xFF02;
	SetConfig.u8Security2 = 5;
}

CDomodalDlg::~CDomodalDlg()
{
}

void CDomodalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combox_com);
	DDX_Control(pDX, IDC_COMBO2, m_Baudrate);
	DDX_Text(pDX, IDC_EDIT2, m_ScurityType);
	DDX_Control(pDX, IDC_COMBO3, m_ProjectID);
	DDX_Control(pDX, IDC_EDIT3, m_PhysicID);
	DDX_Control(pDX, IDC_EDIT5, m_FunCtionID);
	DDX_Control(pDX, IDC_EDIT6, m_ECUID);
	DDX_Control(pDX, IDC_COMBO4, m_FramSize);
	DDX_Control(pDX, IDC_EDIT7, m_Mask1);
	DDX_Control(pDX, IDC_EDIT8, m_Mask2);
	DDX_Control(pDX, IDC_EDIT4, m_ScurityPath);
	DDX_Control(pDX, IDC_EDIT9, m_IntergrityID);
	DDX_Control(pDX, IDC_EDIT10, m_dependenceID);
	DDX_Control(pDX, IDC_EDIT11, m_RCID_ERASE);
	DDX_Control(pDX, IDC_EDIT12, m_Precondition);
}


BEGIN_MESSAGE_MAP(CDomodalDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CDomodalDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDomodalDlg::OnBnClickedOk)
//	ON_BN_CLICKED(IDC_BUTTON1, &CDomodalDlg::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDomodalDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_COM, &CDomodalDlg::OnBnClickedButtonOpenCom)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_COM, &CDomodalDlg::OnBnClickedButtonCloseCom)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDomodalDlg::OnCbnSelchangeCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDomodalDlg::OnCbnSelchangeCombo3)
	ON_BN_CLICKED(IDC_BUTTON_OPENDLL, &CDomodalDlg::OnBnClickedButtonOpendll)
	ON_EN_CHANGE(IDC_EDIT4, &CDomodalDlg::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT2, &CDomodalDlg::OnEnChangeEdit2)
END_MESSAGE_MAP()


// CDomodalDlg 消息处理程序

void CDomodalDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CDomodalDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
	int i=m_combox_com.GetCurSel();
	COMID=i;
	BOTID=m_Baudrate.GetCurSel();
	
}

void CDomodalDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDomodalDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}

BOOL CDomodalDlg::OnInitDialog(void)
{
	CString strIDName, strIDName1;
	int  u16ccd;



    CDialog::OnInitDialog();
	m_combox_com.AddString(L"CAN1");
	m_combox_com.AddString(L"CAN2");
	m_combox_com.AddString(L"CAN3");
	LPCTSTR lpszmyString=L"com";
	m_combox_com.SetCurSel(COMID);

	m_Baudrate.AddString(L"100");
	m_Baudrate.AddString(L"250");
	m_Baudrate.AddString(L"500");
	
	strIDName.Format(L"%d",SetConfig.Band);
	m_Baudrate.SetWindowTextW(strIDName);
	
    //m_Baudrate.SetCurSel(BOTID);

	m_FramSize.AddString(L"标准帧");
	m_FramSize.AddString(L"扩展帧");

	if (SetConfig.FramSize)
	{
		//m_FramSize.SetCurSel(BOTID);
		m_FramSize.SetWindowTextW(L"扩展帧");
	}
	else
	{
		m_FramSize.SetWindowTextW(L"标准帧");
	}
    GetDlgItem(IDC_BUTTON_OPEN_COM)->EnableWindow(!m_Enable);
	GetDlgItem(IDC_BUTTON_CLOSE_COM)->EnableWindow(m_Enable);

	CString str;
	str.Format(L"%x", SetConfig.ECUID);
	m_ECUID.SetWindowTextW(str);

	str.Format(L"%x", SetConfig.PhyID);
	m_PhysicID.SetWindowTextW(str);

	str.Format(L"%x", SetConfig.FunID);
	m_FunCtionID.SetWindowTextW(str);

	str.Format(L"%x", SetConfig.mask1);
	m_Mask1.SetWindowTextW(str);

	str.Format(L"%x", SetConfig.mask2);
	m_Mask2.SetWindowTextW(str);

	str.Format(L"%x", SetConfig.RCID_DEPENDENCE);
	m_dependenceID.SetWindowTextW(str);

	str.Format(L"%x", SetConfig.RCID_INTERGRITY);
	m_IntergrityID.SetWindowTextW(str);

	str.Format(L"%x", SetConfig.RCID_ERASE);
	m_RCID_ERASE.SetWindowTextW(str);

	str.Format(L"%x", SetConfig.RCID_PRECONDITIONS);
	m_Precondition.SetWindowTextW(str);

	m_ScurityPath.SetWindowTextW(SetConfig.dllPath);

	_variant_t vUsername,vBirthday,vID,vOld;
		_variant_t RecordsAffected;
	CADO ado;
	_RecordsetPtr m_pRecordset;

	//if (ado.OnInitADOConn(L"id.mdb") )
	//{
	//	return;
	//}
	//vID = ado.OnInitADOConn(L"id.mdb");//L"id.dbc"
	if (ado.OnInitADOConn(L"id.mdb") == FALSE)
	{
		return FALSE;
	}	
	m_pRecordset   = ado.GetRecordset();
	m_pRecordset = ado.OpenRecordset(L"select *  from CANCONFIG");




	while(!m_pRecordset->adoEOF)
	{
		vID = m_pRecordset->GetCollect(_variant_t((long)0));
		vUsername = m_pRecordset->GetCollect("Project");
		m_ProjectID.AddString((CString)vUsername);

		//vUsername = m_pRecordset->GetCollect("Project");

		m_pRecordset->MoveNext();///移到下一条记录
	}


    m_ProjectID.SetCurSel(ProjectID);

/*	strIDName = L"";
	CString str;

	CString strCmd=L"SELECT  * FROM user2"; //WHERE username=
	strCmd += strIDName;

	_ConnectionPtr m_pConnection;
	m_pConnection = ado.GetConnec();
	m_pRecordset = m_pConnection->Execute(strCmd.AllocSysString(),&RecordsAffected,adCmdText);
*/
	return TRUE;
}

void CDomodalDlg::OnBnClickedButtonOpenCom()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	COMID=m_combox_com.GetCurSel();;
	BOTID=m_Baudrate.GetCurSel();
	ProjectID=m_ProjectID.GetCurSel();

	SetConfig.PhyID = m_PhysicID.GetInt();
	SetConfig.FunID = m_FunCtionID.GetInt();
	SetConfig.ECUID = m_ECUID.GetInt();
	SetConfig.mask1 = m_Mask1.GetInt();
	SetConfig.mask2 = m_Mask2.GetInt();
	SetConfig.RCID_INTERGRITY = m_IntergrityID.GetInt();
	SetConfig.RCID_DEPENDENCE = m_dependenceID.GetInt();
	SetConfig.RCID_ERASE = m_RCID_ERASE.GetInt();
	SetConfig.RCID_PRECONDITIONS = m_Precondition.GetInt();


	m_Baudrate.GetWindowTextW(str) ;
	if (str== L"500")
	{
		SetConfig.Band = 500;
	}
	else if (str == L"250")
	{
		SetConfig.Band = 250;
	}
	else
	{
		SetConfig.Band = 100;
	}

	m_FramSize.GetWindowTextW(str) ;
    if (str == L"标准帧")
	{
		SetConfig.FramSize = 0;
	}
	else
	{
		SetConfig.FramSize = 1;
	}
	UpdateData(FALSE);
	SetConfig.u8Security2 = m_ScurityType;
	m_Enable=1;
	OnOK();
}

void CDomodalDlg::OnBnClickedButtonCloseCom()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Enable=0;
	OnOK();
}

void CDomodalDlg::OnCbnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDomodalDlg::OnCbnSelchangeCombo3()
{
	CString str;
	_variant_t vUsername,vBirthday,vID,vOld;
	_variant_t RecordsAffected;
	CADO ado;
	_RecordsetPtr m_pRecordset;


	m_ProjectID.GetLBText(m_ProjectID.GetCurSel(),str);
	

	ado.OnInitADOConn(L"id.mdb");//L"id.dbc"
		
	m_pRecordset   = ado.GetRecordset();
	//m_pRecordset = ado.OpenRecordset(L"select *  from user2");
	



	CString strCmd=L"SELECT  * FROM CANCONFIG WHERE Project= ";
	strCmd = strCmd + L"'"+str+L"'";

	_ConnectionPtr m_pConnection;
	m_pConnection = ado.GetConnec();
	m_pRecordset = ado.m_pConnection->Execute(strCmd.AllocSysString(),&RecordsAffected,adCmdText);


	if (!m_pRecordset->adoEOF)
	{
		vID = m_pRecordset->GetCollect(_variant_t((long)0));
		vUsername = m_pRecordset->GetCollect("Bandrate");
		str = (CString)vUsername;
		m_Baudrate.SetWindowTextW(str);

		vUsername = m_pRecordset->GetCollect("扩展帧");
		str = (CString)vUsername;
		if (str ==L"0")
		{
			m_FramSize.SetWindowTextW(L"标准帧") ;
		}
		else
		{
			m_FramSize.SetWindowTextW(L"扩展帧") ;
		}

		vUsername = m_pRecordset->GetCollect("PhysicID");
		str = (CString)vUsername;
		m_PhysicID.SetWindowTextW(str);

		vUsername = m_pRecordset->GetCollect("FunctionID");
		str = (CString)vUsername;
		m_FunCtionID.SetWindowTextW(str);

		vUsername = m_pRecordset->GetCollect("ECUID");
		str = (CString)vUsername;
		m_ECUID.SetWindowTextW(str);

		vUsername = m_pRecordset->GetCollect("Mask1");
		str = (CString)vUsername;
		m_Mask1.SetWindowTextW(str);

		vUsername = m_pRecordset->GetCollect("Mask2");
		str = (CString)vUsername;
		m_Mask2.SetWindowTextW(str);

		vUsername = m_pRecordset->GetCollect("RCID_INTERGRITY");
		str = (CString)vUsername;
		m_IntergrityID.SetWindowTextW(str);

		vUsername = m_pRecordset->GetCollect("RCID_DEPENDENCE");
		str = (CString)vUsername;
		m_dependenceID.SetWindowTextW(str);

		vUsername = m_pRecordset->GetCollect("RCID_ERASE");
		str = (CString)vUsername;
		m_RCID_ERASE.SetWindowTextW(str);

		vUsername = m_pRecordset->GetCollect("RCID_PRECONDITIONS");
		str = (CString)vUsername;
		m_Precondition.SetWindowTextW(str);


		/*vUsername = m_pRecordset->GetCollect("解锁等级2");
		
		if (vUsername.vt == VT_NULL)
		{
		}
		else
		{
			str = (CString)vUsername;
			if (str ==L"")
			{
				m_ScurityLevel2 = 0x5 ;
			}
			else if (str ==L"9")
			{
				m_ScurityLevel2 = 0x9 ;
			}
			else if (str ==L"11")
			{
				m_ScurityLevel2 = 0x11 ;
			}
			else if (str ==L"3")
			{
				m_ScurityLevel2 = 0x3;
			}
			else
			{
				m_ScurityLevel2 = 0x5 ;
			}
		}
		*/
		//vUsername = m_pRecordset->GetCollect("Project");

		//m_pRecordset->MoveNext();///移到下一条记录
	}
/*		static unsigned int productnum=0;
	_variant_t RecordsAffected;
	CADO ado;
	CString strIDName, strData;
	_RecordsetPtr m_pRecordset;

	ado.OnInitADOConn(L"id.mdb");//L"id.dbc"
	
	//m_pRecordset   = ado.GetRecordset();
	m_pRecordset   = ado.GetRecordset();
	m_pRecordset = ado.OpenRecordset(L"select *  from user2");

	CString strCmd=L"SELECT  * FROM user2 WHERE username= '12345678901'";

	_ConnectionPtr m_pConnection;
	m_pConnection = ado.GetConnec();
	m_pRecordset = ado.m_pConnection->Execute(strCmd.AllocSysString(),&RecordsAffected,adCmdText);
	*/
	// TODO: 在此添加控件通知处理程序代码
}


void CDomodalDlg::OnBnClickedButtonOpendll()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strName;
	CString str, m_Result;
	//CFileDialog dlg(FALSE,_T("s19; hex"),_T("*.s19; *.hex"),OFN_EXPLORER,_T("程序文件(.s19)| *.s19; *.hex|所有文件（*.*)|*.*||)"));
	CFileDialog dlg(FALSE,_T("DLL"),_T("*.dll"),OFN_EXPLORER,_T("程序文件(.dll)| *.dll|所有文件（*.*)|*.*||)"));
	if(dlg.DoModal()==IDOK)
	{
		strName=dlg.GetPathName();
		m_ScurityPath.SetWindowTextW(strName);
	}		
	else
	{
		//ShowInfo(L"文件载入失败", 2);
	}
	UpdateData(FALSE);

}


void CDomodalDlg::OnEnChangeEdit4()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	CString strFile = L"";  
	CString  m_Result, str;
    m_ScurityPath.GetWindowTextW(strFile);

	SetConfig.dllPath =  strFile;
	// TODO:  在此添加控件通知处理程序代码
}


void CDomodalDlg::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
