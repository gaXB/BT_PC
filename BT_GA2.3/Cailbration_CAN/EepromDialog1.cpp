// EepromDialog1.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "EepromDialog1.h"
#include "DialogConfirm1.h"
#include "StdioFileEx.h"
#include "GWMMN/timerout.h"
#include "GWMMN/ApplayLay.h"
#include "GWMMN/NetLaye.h"

#include <iostream>
#include <cstring>


#define  MAINAPP_NAME	L"KEAPC_2" 
// CEepromDialog1 �Ի���
extern int Eeprom_DisplayFlg[200];

void	FirstRowSet(CStringArray  *FirstRow);
int		GridNameSet(EEPROMDATA *EepromData);
void	EepromToDispData(int *Eeprom,float *DisPlayData);
void	EepromInit(int *Eeprom);
void	DispDataToEeprom(int *Eeprom,float *DisPlayData);

extern uint8  F_N_USDATA_REQ(uint8* ReqData,uint16 Length,uint8 mId_Target);
IMPLEMENT_DYNAMIC(CEepromDialog1, CDialog)

CEepromDialog1::CEepromDialog1(CWnd* pParent /*=NULL*/)
	: CDialog(CEepromDialog1::IDD, pParent)
	, m_ComID(0)
	, error_message(_T(""))
{

}

CEepromDialog1::~CEepromDialog1()
{
}

void CEepromDialog1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_CUSTOM1, m_Grid);
//	DDX_Text(pDX, IDC_EDIT1, m_ComID);
	DDX_Control(pDX, IDC_STATIC_COM, m_COM_POCK);
	DDX_Text(pDX, IDC_EDIT_MESSAGE2, error_message);
}


BEGIN_MESSAGE_MAP(CEepromDialog1, CDialog)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDOK, &CEepromDialog1::OnBnClickedOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_BUTTON_READ, &CEepromDialog1::OnBnClickedButtonRead)
	ON_MESSAGE(WM_RECIVE_TOEEPROM, OnComMessage)
	ON_BN_CLICKED(ID_BUTTON_LOAD, &CEepromDialog1::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDCANCEL, &CEepromDialog1::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON2, &CEepromDialog1::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CEepromDialog1::OnBnClickedButton1)
	ON_WM_COPYDATA()
END_MESSAGE_MAP()


// CEepromDialog1 ��Ϣ�������


afx_msg	LRESULT CEepromDialog1:: OnComMessage(WPARAM wParam, LPARAM lParam)
{
	unsigned char bt ;
	bt =(unsigned char)wParam;
	
	//GetDlgItem(IDC_EDIT2) ->GetWindowText(m_ComReceive);
	CString Srting_a;
	Srting_a.Format(L"%d",bt);
	error_message += Srting_a+L"\r\n";
	Treat_RecRam(bt);
	return 0;
}

BOOL CEepromDialog1::OnInitDialog()
{
	CDialog::OnInitDialog();

	BOOL com_Enable;

	SetTimer(TIMER_ID_TIME,1000,NULL);
	EepromInit(Eeprom);
	/*if(ComP!=NULL)
	{
		int b=ComP->BOTID;
		switch(b)
		{
		default:
			b=9600;
			break;
		case	0:
			b=12800;
			break;
		case	1:
			b=19200;
			break;
		case	2:
			b=9600;
			break;
		}

	    m_ComID=ComP->COMID+1; //�õ����ں�
	
		com_Enable=ComP->m_Enable;
	}*/
	com_Enable=1;

	if(com_Enable)
	{
		m_COM_POCK.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_POCK3));
		//GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
		//GetDlgItem(IDC_BUTTON_LOAD)->EnableWindow(true);

	//	bmpLoad.LoadBitmap(IDB_BITMAP5);
	}
	else
	{
		m_COM_POCK.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_POCK6));
		//GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
		//GetDlgItem(IDC_BUTTON_LOAD)->EnableWindow(false);
	//	bmpLoad.LoadBitmap(IDB_BITMAP4);
	}


	m_Grid.DeleteAllItems();

	m_Grid.SetDefCellHeight(35);
	//SetTimer(1,1000,NULL);//��װ��ʱ����������ʱ������Ϊ1000����
	m_Grid.SetRowCount(MAX_EEPROMNUM_LINE+5);         //�Ҵ�������øñ����������
    m_Grid.SetFixedRowCount(1);     //�����ñ�Ĺ̶�����
    m_Grid.SetColumnCount(6);       //�����ñ������
    m_Grid.SetFixedColumnCount(1);  //�����ñ�Ĺ̶�����
	m_Grid.SetRowResize(true);     //���Ƿ�������еĴ�С

	CStringArray  FirstRow ,col1;           //�Ҷ���һ���ַ�������
	CStringArray* col12;
	FirstRowSet(&col1);
	

	EEPROMDATA EepromData[MAX_EEPROMNUM];
	EEPROMDATA*	pEepromData =EepromData;
    MaxItem = GridNameSet(EepromData);

	

	for(int col = 0; col <=5; col++)
	{
		    GV_ITEM Item;                //���� GV_ITEM �ṹ���е�ֵ�����õ�Ԫ�������.
			Item.mask = GVIF_TEXT|GVIF_FORMAT;    //����������Ϣ�ĻҶ�
			Item.row = 0;                       //������Ϣ����λ��
			Item.col = col;                       //������Ϣ����λ��

			Item.mask = GVIF_TEXT|GVIF_FORMAT;  
            Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX;  

			Item.strText=col1.GetAt(col);

			m_Grid.SetItem(&Item);  //��������Ϣ������
			//m_Grid.SetColumnWidth(col,_ROW_Width);
	}
		   
	GV_ITEM Item; 
	int row = 1;
	for(int i = 1; i <=MaxItem; i++)
	{
		if (Eeprom_DisplayFlg[i-1])  
		{
			Item.mask = GVIF_TEXT|GVIF_FORMAT;    //����������Ϣ�ĻҶ�
			Item.col = 0;                       //������Ϣ����λ��
			Item.row = row;

			Item.mask = GVIF_TEXT|GVIF_FORMAT;  
            Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX;  

			Item.strText.Format(L"%d", row);
			m_Grid.SetItem(&Item);  //��������Ϣ������
			
			Item.col = 1;
		    Item.strText =pEepromData->Name;
			m_Grid.SetItem(&Item);  

			Item.col = 3;
		    Item.strText =pEepromData->Unit;
			m_Grid.SetItem(&Item); 

			Item.col = 4;
			Item.strText =pEepromData->Accuracy;
			m_Grid.SetItem(&Item); 

			Item.col = 5;
			Item.strText =pEepromData->Area;
			m_Grid.SetItem(&Item); 

			
			row++;
			//m_Grid.SetColumnWidth(col,_ROW_Width);
		}
		pEepromData++;
	}

	RefereshDisplayData();
    if(com_Enable)	OnBnClickedButtonRead();
	return TRUE; 
}




int CEepromDialog1::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	return 0;
}


void CEepromDialog1::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CEepromDialog1::SetCom(CDomodalDlg* ComDlg)
{
	
	if(ComDlg!=NULL)
	{
		ComP=ComDlg;
		int b=ComDlg->BOTID;
		switch(b)
		{
		default:
			b=9600;
			break;
		case	0:
			b=12800;
			break;
		case	1:
			b=19200;
			break;
		case	2:
			b=9600;
			break;
		}

	    m_ComID=ComDlg->COMID+1; //�õ����ں�
	
	//	com_Enable=ComDlg->m_Enable;

	}
}


void CEepromDialog1::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CTime time;
	CString m_Time;
	static	int ntime1;
  //  UpdateData(TRUE);
	switch(nIDEvent)
	{
	case	TIMER_ID_TIME:
		ntime1++;
		if(ntime1>10) 
		{
			//m_ComReceive="";
			ntime1=0;
		}
		time=CTime::GetCurrentTime();//�õ���ǰʱ��
		m_Time=time.Format(L"%H:%M:%S");//ת��ʱ���ʽ
		CDialog::OnTimer(nIDEvent);
		GetDlgItem(IDC_TIME) ->SetWindowText(m_Time);

		break;
	case	2:
		ntime1=0;
		if (config == WORK_STATE_WRITE)
		{
			error_message="����ʧ��";
		}
		else if (config == WORK_STATE_WRITEOK)
		{
			error_message="����ɹ�";
		}
		else if (config == WORK_STATE_READ)
		{
			error_message="��ȡʧ��";
		}
		else if (config == WORK_STATE_READOK)
		{
			error_message="��ȡ�ɹ�";
		}
		else 
		{
			error_message="״̬����";
		}
	    UpdateData(FALSE);
		KillTimer(ID_TIMER_COMX);
		break;
	}

}



void CEepromDialog1::RefereshDisplayData(void)
{
	float DisPlayData[MAX_EEPROMNUM_LINE];
	


	EepromToDispData(Eeprom,DisPlayData);


	int row = 1;
	GV_ITEM Item;                //���� GV_ITEM �ṹ���е�ֵ�����õ�Ԫ�������.
	Item.mask = GVIF_TEXT|GVIF_FORMAT;    //����������Ϣ�ĻҶ�
	Item.mask = GVIF_TEXT|GVIF_FORMAT;  
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX;  
	
	for(int i = 1; i <=MaxItem; i++)
	{

		if (Eeprom_DisplayFlg[i-1])	
		{	
			Item.row = row++;                       //������Ϣ����λ��
			Item.col =2;                       //������Ϣ����λ��

			if(DisPlayData[i-1]-(int)DisPlayData[i-1]/1 !=0)
			{
				Item.strText.Format(L"%0.1f",DisPlayData[i-1]);
			}
			else
			{
				Item.strText.Format(L"%1.0f",DisPlayData[i-1]);
			}
			m_Grid.SetItem(&Item);  //��������Ϣ������
			//m_Grid.SetColumnWidth(col,_ROW_Width);
		}
	}
	m_Grid.Refresh();
}



// �������ݴ���
void CEepromDialog1::Treat_RecRam(unsigned char bt)
{
	
}

//���밴�� �������Ϣ
extern uint8  SecurityState, SessionMode; 
void CEepromDialog1::OnBnClickedButtonLoad()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogConfirm1 m_Dlg;
	INT_PTR Res = m_Dlg.DoModal();
	if(Res != IDOK) return;
	config=WORK_STATE_WRITE;
	nstat=0;
//	m_SendUChar=0xa5;
//	AFXSend[0]=0xa5;

    SessionMode = SESSION_MODE_EXTERN;    //�Ựģʽ
    SecurityState = SECURITY_STATE_SSEED1;   //��ȫ����ģʽ

	Sleep(200);

    float DisPlayData[MAX_EEPROMNUM_LINE];
	CString m_Text;

	for(int row = 1; row <=MaxItem; row++)
	{
		m_Text =m_Grid.GetItemText(row,2);
		DisPlayData[row-1] = (float)_tstof(m_Text);
	}
	
	DispDataToEeprom(Eeprom,DisPlayData);

	//CWnd* ahWnd = FindWindow( NULL,MAINAPP_NAME ) ;
	//if(ahWnd)
	//{
	//	CDS.dwData = 0;   
	//	CDS.cbData = MAX_EEPROMNUM; 
	//	CDS.lpData = Eeprom;
	//	ahWnd->SendMessage(WM_COPYDATA,0,(LPARAM)&CDS);
	//}

	uint8 SendData[MAX_EEPROMNUM + 3];

	SendData[0] = 0x2e;
	SendData[1] =  CAILBRATION_IODID_RAM / 256;
	SendData[2] = CAILBRATION_IODID_RAM & 0x00ff;

	for (int i= 0; i<MAX_EEPROMNUM; i++)
	{
		SendData[3+i] = (uint8)Eeprom[i];
	}

	F_N_USDATA_REQ(SendData, MAX_EEPROMNUM+3,ID_DEFINE_TARGET);

    SetTimer(ID_TIMER_COMX,1000,NULL);
	error_message="��ʼ����";
	UpdateData(FALSE);
}



//��ȡ���ݵİ�����Ӧ
void CEepromDialog1::OnBnClickedButtonRead()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	config = WORK_STATE_READ ;
	nstat=0;
//	m_SendUChar=170;
//	AFXSend[0]=170;
//	*send=170;

	unsigned char ch[2];
	ch[0]=170;

	CWnd* ahWnd = FindWindow( NULL,MAINAPP_NAME ) ;
	if(ahWnd)
	{
		//CDS.dwData = 0;   
		//CDS.cbData = 1; 
		//CDS.lpData = ch;
		//ahWnd->SendMessage(WM_COPYDATA,0,(LPARAM)&CDS);

	    uint8 SendByte[3];

		SendByte[0] = 0x22;
		SendByte[1] = (uint8)(CAILBRATION_IODID_RAM / 256);
		SendByte[2] = (uint8)(CAILBRATION_IODID_RAM % 256);
		F_N_USDATA_REQ(SendByte, 3,ID_DEFINE_TARGET);
	}



	SetTimer(ID_TIMER_COMX,1000,NULL);
	error_message=_T("��ʼ��ȡ");
	UpdateData(FALSE);
}

void CEepromDialog1::OnBnClickedCancel()
{
	CDialog::OnCancel();
}

void CEepromDialog1::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strName;
	BOOL m_Success;
	CFileDialog dlg(FALSE,_T("csv"),_T("*.csv"),OFN_EXPLORER,_T("����ļ�(.csv)|*.csv|�����ļ���*.*)|*.*||)"));
	if(dlg.DoModal()==IDOK)
	{
		strName=dlg.GetPathName();
		
	

		//����Ϊ�õ�strName ·��
		CStdioFileEx myFile; 
		CFileException fileException; 
		if(!myFile.Open(strName,CFile::modeCreate|CFile::modeNoTruncate|CFile::typeText|CFile::modeReadWrite),&fileException) 
		{ 
			TRACE("Can't open file %s, error = %u\n",strName,fileException.m_cause); 
		} 


		int	r=m_Grid.GetRowCount();
		int	c=m_Grid.GetColumnCount();
		CString	stf =L"";
		CString DisPlayStr[MAX_EEPROMNUM_LINE];
		
		myFile.ReadString(stf);
		
		
		
		int nIndex , nGrid=0;  
		do
		{
			myFile.ReadString(stf);
			nIndex = stf.Find(L",");
			if(nIndex ==-1) break;
			stf =stf.Mid(++nIndex);
			nIndex = stf.Find(L",");
			if(nIndex ==-1) break;
			stf =stf.Mid(++nIndex);
			nIndex = stf.Find(L",");
			if(nIndex ==-1) break;
			stf = stf.Left(nIndex);
			DisPlayStr[nGrid] =stf;
			nGrid++;
		}while (nIndex !=-1 && nGrid <MAX_EEPROMNUM_LINE);
//char *p = (LPSTR)(LPCTSTR)stf;

		if(nGrid == MAX_EEPROMNUM_LINE)
		{
			MessageBox(strName+_T("�ļ�������"),_T("�����ļ�"));
		}
		else
		{
			MessageBox(_T("����ʧ��"),_T("��ʾ"));
			return ;
		}

		GV_ITEM Item;                //���� GV_ITEM �ṹ���е�ֵ�����õ�Ԫ�������.
		Item.mask = GVIF_TEXT|GVIF_FORMAT;    //����������Ϣ�ĻҶ�
		Item.col =2;                       //������Ϣ����λ��
		Item.mask = GVIF_TEXT|GVIF_FORMAT;  
		Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX;  


		for(int row = 1; row <=MaxItem; row++)
		{
			Item.row = row;                       //������Ϣ����λ��
			Item.strText = DisPlayStr[row -1]; //m_Grid.SetColumnWidth(col,_ROW_Width);
			m_Grid.SetItem(&Item);  //��������Ϣ������	
		}
		m_Grid.Refresh();
		myFile.Close();
	}
}

//�����ļ�
void CEepromDialog1::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		CString strName;
	CFileDialog dlg(FALSE,_T("csv"),_T("*.csv"),OFN_EXPLORER,_T("����ļ�(.csv)|*.csv|�����ļ���*.*)|*.*||)"));
	if(dlg.DoModal()==IDOK)
	{
		strName=dlg.GetPathName();
		MessageBox(strName+_T("�ļ��ѱ���"),_T("�����ļ�"));
	

		//����Ϊ�õ�strName ·��
		CStdioFileEx myFile; 
		CFileException fileException; 
		if(!myFile.Open(strName,CFile::modeCreate|CFile::modeNoTruncate|CFile::typeText|CFile::modeReadWrite),&fileException) 
		{ 
			TRACE("Can't open file %s, error = %u\n",strName,fileException.m_cause); 
		} 

		int	r=m_Grid.GetRowCount();
		int	c=m_Grid.GetColumnCount();
		CString	stf;

		for( int i=0;i<=r;i++)
		{
			for(int	j=0;j<=c;j++)
			{
				stf=m_Grid.GetItemText(i,j);
				myFile.WriteString(stf+L","); 
			}
			
			myFile.WriteString(L"\n"); 
		}
		myFile.Close();
	}
	
}


//���ܵ�CAN��Ӧ�ò�����

BOOL CEepromDialog1::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	int len=pCopyDataStruct->cbData;
	unsigned  char *ch=(unsigned char *)pCopyDataStruct->lpData;
	
	int  u16DID = 256*ch[0] + ch[1];

	if (u16DID == CAILBRATION_IODID_RAM)
	{
		if (len == 3)
		{//��2e�ķ���
			config = WORK_STATE_WRITEOK ;
		}
		else
		{
			if (len <= (MAX_EEPROMNUM+3) )
			{
				for (int i = 2; i< (len-1); i++)
				{
					Eeprom[i-2] = ch[i];
				}
				RefereshDisplayData();
				config = WORK_STATE_READOK ;
			}
		}

	}
	// if(!ComDlg.m_Enable) return 1; //��鴮���Ƿ��
	// m_SerialPort.WriteToPort(ch,len);//��������
	return 0;
}
