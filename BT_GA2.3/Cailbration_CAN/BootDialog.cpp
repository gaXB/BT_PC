// BootDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Cailbration_CAN.h"
#include "BootDialog.h"
#include "GWMMN/ApplayLay.h"


static uint8 Mode = SESSION_MODE_DEFAULT;
extern uint8 SessionMode, SecurityState;

extern void  L_SendDiagFram(uint8* NetData);
uint8  F_N_USDATA_REQ(uint8* ReqData,uint16 Length,uint8 mId_Target);
// CBootDialog �Ի���

IMPLEMENT_DYNAMIC(CBootDialog, CDialog)

CBootDialog::CBootDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CBootDialog::IDD, pParent)
	, m_FileName(_T(""))
	, m_22Message(_T(""))
	, m_DisplayEn(false)
	, m_FastLoadState(0)
{

}

CBootDialog::~CBootDialog()
{
}

void CBootDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_FileName);
	DDX_Control(pDX, IDC_LIST_INFO, m_ListInfo);
	DDX_Control(pDX, IDC_EDIT2, m_DIDHexCode);
	DDX_Control(pDX, IDC_EDIT_22MESSAGE, m_Edite22Message);
	DDX_Text(pDX, IDC_EDIT_22MESSAGE, m_22Message);
	DDX_Control(pDX, IDC_EDIT3, m_DragEdit);
	DDX_Control(pDX, IDC_EDIT1, m_File);
}


BEGIN_MESSAGE_MAP(CBootDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CBootDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_SESSIONMODE, &CBootDialog::OnBnClickedButtonSessionmode)
	ON_BN_CLICKED(IDC_BUTTON_SCURITY, &CBootDialog::OnBnClickedButtonScurity)
	ON_BN_CLICKED(IDC_BUTTON_READDTC, &CBootDialog::OnBnClickedButtonReaddtc)
	ON_BN_CLICKED(IDC_BUTTON_COMCONTROL, &CBootDialog::OnBnClickedButtonComcontrol)
	ON_BN_CLICKED(IDC_BUTTON_EARSE, &CBootDialog::OnBnClickedButtonEarse)
	ON_BN_CLICKED(IDC_BUTTON_DownLoad, &CBootDialog::OnBnClickedButtonDownload)
	ON_BN_CLICKED(IDC_BUTTON_Integrity, &CBootDialog::OnBnClickedButtonIntegrity)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CBootDialog::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_RDID, &CBootDialog::OnBnClickedButtonRdid)
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY_EN, &CBootDialog::OnBnClickedButtonDisplayEn)
	ON_BN_CLICKED(IDC_BUTTON_FASTLOAD, &CBootDialog::OnBnClickedButtonFastload)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_WDID, &CBootDialog::OnBnClickedButtonWdid)
	ON_BN_CLICKED(IDC_BUTTON_IOCONTROL, &CBootDialog::OnBnClickedButtonIocontrol)
	ON_BN_CLICKED(IDC_BUTTON2, &CBootDialog::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CBootDialog::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CBootDialog::OnBnClickedButton4)
	ON_EN_CHANGE(IDC_EDIT3, &CBootDialog::OnEnChangeEdit3)
	ON_BN_CLICKED(IDOK, &CBootDialog::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT1, &CBootDialog::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON_READADDR, &CBootDialog::OnBnClickedButtonReadaddr)
	ON_BN_CLICKED(IDCANCEL, &CBootDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON6, &CBootDialog::OnBnClickedButton6)
	ON_EN_CHANGE(IDC_EDIT2, &CBootDialog::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON_READDTC2, &CBootDialog::OnBnClickedButtonReaddtc2)
END_MESSAGE_MAP()


// CBootDialog ��Ϣ�������
int JudgeS19(CString filename);
int ReadS19(unsigned char (*p)[100], CString filename);
int ReadS19TOData(HEXDATA_ALL *sHexData , CString filename);
void CBootDialog::OnBnClickedButton1()
{
	// TODO: load S19
	CString strName;
	CString str, m_Result;
	//CFileDialog dlg(FALSE,_T("s19; hex"),_T("*.s19; *.hex"),OFN_EXPLORER,_T("�����ļ�(.s19)| *.s19; *.hex|�����ļ���*.*)|*.*||)"));
	CFileDialog dlg(FALSE,_T("s19; hex"),_T("*.s19; *.hex"),OFN_EXPLORER,_T("�����ļ�(.s19)| *.s19; *.hex|�����ļ���*.*)|*.*||)"));
	if(dlg.DoModal()==IDOK)
	{
		strName=dlg.GetPathName();
		m_File.SetWindowTextW(strName);
	}		
	else
	{
		ShowInfo(L"�ļ�����ʧ��", 2);
	}
	UpdateData(FALSE);
}


//��Ϣ��ʾ��
void CBootDialog::ShowInfo(CString str, int code)
{
	if (m_DisplayEn)
	{
		m_ListInfo.InsertString(m_ListInfo.GetCount(),str);
		m_ListInfo.SetCurSel(m_ListInfo.GetCount()-1);
	}
}


// �õ�flsh ����
int CBootDialog::SortRegion(void)
{
	int Length = 0;
	int CheckFF;
	int regionNum = 0;
	m_Data1Num = 0;
    m_Data1Addr = m_HexData[Length][2]*256 + m_HexData[Length][3];
	do
	{
		memcpy(&m_HexData1[m_Data1Num], &m_HexData[Length][4], m_HexData[Length][1]-3);
		m_Data1Num +=	m_HexData[Length][1]-3;
		//	m_HexData[Length] = 
	//	m_HexData1[] = 	
		Length++; 
	}while (Length < m_HexDataLen && (m_Data1Num + m_Data1Addr) == (m_HexData[Length][2]*256 + m_HexData[Length][3]) );
	m_FlashChkSum = 0;
	for (int i =0; i < m_Data1Num; i++)
	{//cal chksum
		m_FlashChkSum += m_HexData1[i];
	}//Ϊ16λchecksum
	/*
	unsigned int i = 4, data;
	do
	{
		data = m_HexData1[i-4];
		data += m_HexData1[i-3]*0x100;
		data += m_HexData1[i-2]*0x10000;
		data += m_HexData1[i-1]*0x1000000;
		m_FlashChkSum += data;
		i += 4;
	}while(i <= m_Data1Num);

	i -=4;
*/
    unsigned int data;
	data = (CODE_END_ADDR - CODE_START_ADDR - 4 - m_Data1Num) * 0xff;

	m_FlashChkSum +=  data;

	//if (m_Data1Num != i || (CODE_END_ADDR - CODE_START_ADDR - i) % 4)
	//{
	//	m_FlashChkSum = 0xffff;
	//}
m_FlashChkSum = ~m_FlashChkSum;
m_FlashChkSum++;
	if (m_HexData[Length][0] == 0xff) return 1;  //ֻ��1������

    m_Data2Addr = m_HexData[Length][2]*256 + m_HexData[Length][3];
	m_Data2Num = 0;
    do
	{
		memcpy(&m_HexData2[m_Data2Num], &m_HexData[Length][4], m_HexData[Length][1]-3);
		m_Data2Num +=	m_HexData[Length][1]-3;
		//	m_HexData[Length] = 
	//	m_HexData1[] = 	
		Length++; 
	}while (Length < m_HexDataLen && (m_Data2Num + m_Data2Addr) == (m_HexData[Length][2]*256 + m_HexData[Length][3]) && m_HexData[Length][0] == 0xfa );

	for (int i =0; i < m_Data2Num; i++)
	{//cal chksum
		m_FlashChkSum += m_HexData2[i];
	}

	CheckFF = (CODE_END_ADDR - CODE_START_ADDR - m_Data2Num - m_Data1Num -4 ) * 0xff;
	m_FlashChkSum += CheckFF;
	m_FlashChkSum = ~m_FlashChkSum;
    m_FlashChkSum++;//???���ﲻ��
	
		if (m_HexData[Length][0] == 0xff) return 2;  //ֻ��1������
		else
			return 0; 
	/*m_Data3Num = m_HexData[Length][1];
	m_Data3Addr =  m_HexData[Length][2]*256 + m_HexData[Length][3];
	
	m_HexData3[0] =  m_HexData[Length][4];
	m_HexData3[1] =  m_HexData[Length][5];

	if (m_Data3Num == 2 && m_Data3Addr == (BOOT_LOAD_ADDR - 2) && m_FlashChkSum == (m_HexData3[0]*256 + m_HexData3[0]))
	{
		Length++;
		if (m_HexData[Length][0] == 0xff) return 3;  //ֻ��1������
		else
			return 0; 
	}
	else
	{
		return 0;
	}�˴���Ҫ���ǿյ�ַΪ0xff�����Դ˷�����ʱ������*/

}
void CBootDialog::OnBnClickedButtonSessionmode()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	switch(Mode)
	{
		default:
		case SESSION_MODE_DEFAULT:
			SessionMode = SESSION_MODE_EXTERN;
			Mode = SESSION_MODE_EXTERN;

			
			GetDlgItem(IDC_BUTTON_SESSIONMODE)->SetWindowText(L"���ģʽ");

		
			break;
		case SESSION_MODE_PROGRAM:
			SessionMode = SESSION_MODE_DEFAULT;
			Mode = SESSION_MODE_DEFAULT;

			GetDlgItem(IDC_BUTTON_SESSIONMODE)->SetWindowText(L"��չģʽ");
			break;
		case SESSION_MODE_EXTERN:
			SessionMode = SESSION_MODE_PROGRAM;
			Mode = SESSION_MODE_PROGRAM;

		GetDlgItem(IDC_BUTTON_SESSIONMODE)->SetWindowText(L"Ĭ��ģʽ");
			break;
	}

}

void CBootDialog::OnBnClickedButtonScurity()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (Mode == SESSION_MODE_PROGRAM)
		SecurityState = SECURITY_STATE_SSEED2;
	else SecurityState = SECURITY_STATE_SSEED1;
}

void CBootDialog::OnBnClickedButtonReaddtc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	uint8 Data[8];
	Data[0] = 0x02;
	Data[1] = 0x19;
	Data[2] = 0x0a;
/*
	Data[0] = 0x06;
	Data[1] = 0x19;
	Data[2] = 0x04;

	Data[3] = 0xC0;
	Data[4] = 0x73;
	Data[5] = 0x88;
	Data[6] = 0x01;
*/
	L_SendDiagFram(Data);
}

void CBootDialog::OnBnClickedButtonComcontrol()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: ͨѶ���ƣ�
	static uint8 u8ControlState;// 0 Ϊ ʹ��  1�� ��ֹ
	uint8 SendByte[3];

	SendByte[0] = 0x28;
	if (u8ControlState)
	{
		u8ControlState = 0;
		GetDlgItem(IDC_BUTTON_COMCONTROL)->SetWindowText(L"ʹ��");
		SendByte[1] = 0x00;
	}
	else
	{
		u8ControlState = 1;
		GetDlgItem(IDC_BUTTON_COMCONTROL)->SetWindowText(L"��ֹ");
		SendByte[1] = 0x03;
	}
	SendByte[2] = 0x01;
	
	UpdateData(false);

	F_N_USDATA_REQ(SendByte, 3,ID_DEFINE_TARGET);
}

void CBootDialog::OnBnClickedButtonEarse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		uint8 SendByte[13];
	SendByte[0] = 0x31;
	SendByte[1] = 0x1;
	SendByte[2] = (unsigned char) (0XFF00 >>8);
	SendByte[3] = (unsigned char) (0xFF00 & 0x00ff);
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

void CBootDialog::OnBnClickedButtonDownload()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_LoadTask = LOAD_TASK_REQDOWNL;
	nRegion = 0;
	nFLashRegion = 0;
	Deal_TransSever();
}


#define  UDSNETLAY_MAXSIZE  252
void CBootDialog::Deal_TransSever(void)
{
	CString str;
	CString Addr;
	unsigned char SendData[UDSNETLAY_MAXSIZE+5];

/*	if (m_LoadTask == LOAD_TASK_TRANSDATA)
	{
		str = L"\r\n���ֳɹ�";

		int nSendLen;
		Addr.Format(L"�� %d ��,  �����ˣ� %d���� ",m_BLSendLen, m_BLSendNum);
		str += Addr;

		SendData[0] = 0x36;
		SendData[1] = m_BLSendLen % 256;//���

		//loop send 
			if (m_BLSendNum >= sHexData[nRegion].HexDataLen) 
			{
				m_LoadTask = LOAD_TASK_EXITTRANS;
				return;
			}
			if ( (m_BLSendNum + UDSNETLAY_MAXSIZE) < sHexData[nRegion].HexDataLen)
			{
				nSendLen = UDSNETLAY_MAXSIZE;
			}
			else
			{//�������
				nSendLen = sHexData[nRegion].HexDataLen - m_BLSendNum;
				m_LoadTask = LOAD_TASK_EXITTRANS;
			}
			memcpy(&SendData[2], &sHexData[nRegion].HexData[m_BLSendNum], nSendLen);

		

		F_N_USDATA_REQ(SendData, nSendLen+2,ID_DEFINE_TARGET);

		m_BLSendLen++;
		m_BLSendNum += nSendLen; 
	}
	else if (m_LoadTask == LOAD_TASK_EXITTRANS)
	{
		SendData[0] = 0x37;
		SendData[1] = 0x01;
		F_N_USDATA_REQ(SendData, 2,ID_DEFINE_TARGET);
		m_LoadTask = LOAD_TASK_REQDOWNL;
		nRegion++;
		if (nRegion >=m_nRegion)
		{
			m_LoadTask = LOAD_TASK_NULL;
		} 
		str.Format(L"��%d�������ؽ���", nRegion);
	}
	else if (m_LoadTask == LOAD_TASK_REQDOWNL)
	{
		SendData[0] = 0x34;
		SendData[1] = 0x00;
		SendData[2] = 0x44;


		SendData[3] = (uint8)(sHexData[nRegion].BeginAddr >> 24);
		SendData[4] = (uint8)((sHexData[nRegion].BeginAddr & 0x00ff0000) >> 16);
		SendData[5] = (uint8)((sHexData[nRegion].BeginAddr & 0x0000ff00) >> 8);
		SendData[6] = (uint8) (sHexData[nRegion].BeginAddr & 0x000000ff);

	
		SendData[7] = (uint8)(sHexData[nRegion].HexDataLen >> 24);
		SendData[8] = (uint8)((sHexData[nRegion].HexDataLen & 0x00ff0000) >> 16);
		SendData[9] = (uint8)((sHexData[nRegion].HexDataLen & 0x0000ff00) >> 8);
		SendData[10] = (uint8) (sHexData[nRegion].HexDataLen & 0x000000ff);	



		//UpdateData(TRUE);


		str.Format(L"��ʼ����� %d ������ �ܹ��� %d ������", sHexData[nRegion].BeginAddr, sHexData[nRegion].HexDataLen);


	//	UpdateData(TRUE);
		F_N_USDATA_REQ(SendData, 11,ID_DEFINE_TARGET);
		m_BLSendLen = 1;
		m_BLSendNum = 0;
		m_LoadTask = LOAD_TASK_TRANSDATA;
	}
	
	ShowInfo(str,2);*/
}

void CBootDialog::OnBnClickedButtonIntegrity()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	uint8 SendByte[4];
	SendByte[0] = 0x31;
	SendByte[1] = 0x01;
	SendByte[2] = (unsigned char) (0xDFFF >>8);
	SendByte[3] = (unsigned char) (0xDFFF & 0x00ff);

	F_N_USDATA_REQ(SendByte, 4,ID_DEFINE_TARGET);
}

void CBootDialog::OnBnClickedButtonReset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		// TODO: �ڴ���ӿؼ�֪ͨ����������
	uint8 SendByte[2];

	SendByte[0] = 0x11;
	SendByte[1] = 0x01;
	UpdateData(false);

	F_N_USDATA_REQ(SendByte, 2,ID_DEFINE_TARGET);
}

void CBootDialog::OnBnClickedButtonRdid()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nDidCode;
	UpdateData(TRUE);
	nDidCode = m_DIDHexCode.GetInt();
		
	uint8 SendByte[3];

	SendByte[0] = 0x22;
	SendByte[1] = nDidCode>>8;
	SendByte[2] = nDidCode&0x00ff;
	F_N_USDATA_REQ(SendByte, 3,ID_DEFINE_TARGET);
}


void CBootDialog::DisplayDID(uint8 *Data, uint8 Length)
{	
	m_22Message = "";

	CString str;
	for (int i = 2; i< (Length-1) ; i++)
	{
		
		str.Format(L"%02X", Data[i]);
		m_22Message += str;
	}

	m_Edite22Message.SetWindowText(m_22Message);
//	UpdateData(false);
//	UpdateData(TRUE);
} 

CString TBL_DTCName[14] = {_T("���ڴ�����"), _T("���⴫����"), _T("����������"), _T("����������"), _T("ģʽ�������"), _T("ģʽ�����ת"),
							_T("��ů�������"), _T("��ů�����ת"),_T("¶�㴫�����¶�"),_T("¶�㴫����ʪ��"),
							_T("�ķ������"),_T("��ѹ����"),_T("��ѹ����"),_T("���⴫����"),};
void CBootDialog::ReadDTC(uint8 *Data, uint16 Length)
{	
	m_22Message = "";

	CString str, str1;
	for (int i = 0; (2+i*4 )< Length; i++)
	{
		
		str.Format(_T("%d: "), i+1);
		
		str1.Format(_T("0x%02x%02x%02x:    0x%02x \r\n"),Data[2+i*4],Data[3+i*4],Data[4+i*4],Data[5+i*4]);

		str += str1;
		m_22Message += str;

	}

	m_Edite22Message.SetWindowText(m_22Message);
//	UpdateData(false);
//	UpdateData(TRUE);
} 
//���ܵ�CAN��Ӧ�ò�����

BOOL CBootDialog::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	int len=pCopyDataStruct->cbData;
	unsigned  char *ch=(unsigned char *)pCopyDataStruct->lpData;
	
	int  u16DID = 256*ch[0] + ch[1];

	return 0;
}

void CBootDialog::OnBnClickedButtonDisplayEn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_DisplayEn)
	{
		m_DisplayEn = FALSE;
	    GetDlgItem(IDC_BUTTON_DISPLAY_EN)->SetWindowText(L"��ʾ��");

	}
	else
	{
		m_DisplayEn = TRUE;
		GetDlgItem(IDC_BUTTON_DISPLAY_EN)->SetWindowText(L"��ʾ��");
	}
}


#define  FASTLOAD_STATE_OVER 0
#define  FASTLOAD_STATE_SECURITY  1
#define  FASTLOAD_STATE_EARSE    2
#define   FASTLOAD_STATE_LOAD   3
#define   FASTLOAD_STATE_INTEGRITY  4
#define   FASTLOAD_STATE_RESET   5

void CBootDialog::OnBnClickedButtonFastload()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_FastLoadState = FASTLOAD_STATE_SECURITY;
	SessionMode = SESSION_MODE_PROGRAM;
	Mode = SESSION_MODE_PROGRAM;
	SetTimer(1,500,NULL);
}

void CBootDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nIDEvent==1)
	{
		switch(m_FastLoadState)
		{
			default:
			case FASTLOAD_STATE_OVER:
				ShowInfo(_T("һ�����ؽ���"),2);
				KillTimer(1);
				break;
			case FASTLOAD_STATE_SECURITY:
				OnBnClickedButtonScurity();
				SetTimer(1,500,NULL);
				m_FastLoadState = FASTLOAD_STATE_EARSE;
				break;
			case FASTLOAD_STATE_EARSE:
				OnBnClickedButtonEarse();
				SetTimer(1,5000,NULL);
				m_FastLoadState = FASTLOAD_STATE_LOAD;
				break;
			case FASTLOAD_STATE_LOAD:
				OnBnClickedButtonDownload();
				SetTimer(1,2000,NULL);
				m_FastLoadState = FASTLOAD_STATE_INTEGRITY;
				break;
			case FASTLOAD_STATE_INTEGRITY:
				if (m_LoadTask == LOAD_TASK_NULL)
				{
					OnBnClickedButtonIntegrity();
					m_FastLoadState = FASTLOAD_STATE_RESET;
				}
				SetTimer(1,500,NULL);
				break;
			case FASTLOAD_STATE_RESET:
				OnBnClickedButtonReset();
				m_FastLoadState = FASTLOAD_STATE_OVER;
				break;
		}
	}

	CDialog::OnTimer(nIDEvent);
}


void	HexToUchar(CString m_StrR,  char *pchar)
{
	unsigned char HexA, HexB;
	unsigned char Len = 0;
	int StrLen = m_StrR.GetLength();

	LPTSTR pch;
	pch = (LPTSTR)(LPCTSTR)m_StrR;

	while(StrLen)
	{
		HexA =(unsigned char) *pch++;
		StrLen--;
		if (HexA >= 65) HexA -= 55;
		else	HexA -= 48;
		HexB =(unsigned char) *pch++;
		if (HexB >= 65) HexB -= 55;
		else	HexB -= 48;
		pchar[Len++] = HexA*16 + HexB;
		StrLen--;
	}
}
void CBootDialog::OnBnClickedButtonWdid()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		int nDidCode;
	UpdateData(TRUE);
	nDidCode = m_DIDHexCode.GetInt();
	
	CString strWrite;
	GetDlgItem(IDC_EDIT_22MESSAGE)->GetWindowTextW(strWrite);
	int length =GetDlgItem(IDC_EDIT_22MESSAGE)->GetWindowTextLengthW();
	//Cwnd =  GetWindowTextW(&strWrite);	
	char SendByte[100];
	//SendByte = new char(3+length);

	SendByte[0] = 0x2e;
	SendByte[1] = nDidCode>>8;
	SendByte[2] = nDidCode&0x00ff;

	HexToUchar(strWrite, &SendByte[3]);

	//strcpy(SendByte, (const char *)(LPCTSTR)strWrite);
	//Change_CString_Char()
	F_N_USDATA_REQ((unsigned char*)SendByte, 3+length/2,ID_DEFINE_TARGET);
}



void CBootDialog::OnBnClickedButtonIocontrol()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nDidCode;
	UpdateData(TRUE);
	nDidCode = m_DIDHexCode.GetInt();
	
	CString strWrite;
	GetDlgItem(IDC_EDIT_22MESSAGE)->GetWindowTextW(strWrite);
	int length =GetDlgItem(IDC_EDIT_22MESSAGE)->GetWindowTextLengthW();
	//Cwnd =  GetWindowTextW(&strWrite);	
	char SendByte[100];

	
	SendByte[0] = 0x2F;
	SendByte[1] = nDidCode>>8;
	SendByte[2] = nDidCode&0x00ff;
	SendByte[3] = 3;

	HexToUchar(strWrite, &SendByte[4]);

	SendByte[5] = 0xff;
	SendByte[6] = 0;
	SendByte[7] = 0;
	//strcpy(SendByte, (const char *)(LPCTSTR)strWrite);
	//Change_CString_Char()
	if (nDidCode == 0xD000)
	{
		F_N_USDATA_REQ((unsigned char*)SendByte, 8,ID_DEFINE_TARGET);
	}
	else
	{
		F_N_USDATA_REQ((unsigned char*)SendByte, 5,ID_DEFINE_TARGET);
	}
	
}

void CBootDialog::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	uint8 SendByte[4];
	SendByte[0] = 0x85;
	SendByte[1] = 0x01;


	F_N_USDATA_REQ(SendByte, 2,ID_DEFINE_TARGET);
}

void CBootDialog::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		// TODO: �ڴ���ӿؼ�֪ͨ����������
	uint8 SendByte[4];
	SendByte[0] = 0x85;
	SendByte[1] = 0x02;


	F_N_USDATA_REQ(SendByte, 2,ID_DEFINE_TARGET);
}

void CBootDialog::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		// TODO: �ڴ���ӿؼ�֪ͨ����������
	uint8 SendByte[4];
	SendByte[0] = 0x14;
	SendByte[1] = 0xff;
	SendByte[2] = 0xff;
		SendByte[3] = 0xff;

	F_N_USDATA_REQ(SendByte, 4,ID_DEFINE_TARGET);
	m_22Message = _T("");
	m_Edite22Message.SetWindowText(m_22Message);
}

void CBootDialog::OnEnChangeEdit3()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strFile = L"";  
    m_DragEdit.GetWindowTextW(strFile);
	if(JudgeS19(strFile))
	{
		ShowInfo(L"�ļ�����ɹ�", 2);
	}
	else
	{
		ShowInfo(L"�ļ�����ʧ��", 2);
	}
}

void CBootDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CBootDialog::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
/*	CString strFile = L"";  
	CString  m_Result, str;
    m_File.GetWindowTextW(strFile);

	if(JudgeS19(strFile))
	{
		ShowInfo(L"�ļ�����ɹ�", 2);
		m_FileName = strFile;
		//m_HexDataLen = ReadS19(m_HexData, m_FileName);
		m_nRegion = ReadS19TOData(sHexData, m_FileName);
		
		if (m_nRegion <= HEX_MAX_REGION && m_nRegion != 0)
		{
			m_Result.Format(L"�� %d ������ \r\n", m_nRegion);
			ShowInfo(m_Result, 2);

			for (int i = 0; i< m_nRegion; i++)
			{
				str.Format(L"��%d���� ��ʼ��ַ��0X%x�� ���� %d \r\n",i+1, sHexData[i].BeginAddr, sHexData[i].HexDataLen);
				ShowInfo(str, 2);
				//m_Result += str;
			}
			m_Result = _T("");
			//����checksum
			int iStartAddr, iEndAddr;

			//iStartAddr = m_StartAddr.GetInt();
			//iEndAddr  = m_EndAddr.GetInt();
			iStartAddr = CODE_START_ADDR;
			iEndAddr = CODE_END_ADDR;

			int HexStartAddr, HexEndAddr;
			HexStartAddr = sHexData[0].BeginAddr;
			HexEndAddr = sHexData[m_nRegion-1].BeginAddr + sHexData[m_nRegion-1].HexDataLen -1;
			
			if (iStartAddr >= iEndAddr     //��ַ����
				|| iStartAddr != HexStartAddr || HexEndAddr > iEndAddr) // ��ʼ��ַһ��Ҫ���,��β��ַҪ������Ŀ�
			//if (0)
			{
				m_Result += (L"��ַ��Ч������HEX������checksumΪ��\r\n");
				m_FlashChkSum = 0;
				for (int i = 0; i< m_nRegion; i++)
				{
					for (int j = 0; j < sHexData[i].HexDataLen; j++)
					{
						m_FlashChkSum += sHexData[i].HexData[j];
					}						
				}
			}
			else
			{
				//�����ж��ٸ�����δ��д����������δ0xff
				m_Result += (L"��ַ��Ч���յ�ַΪ0xff������checksumΪ��\r\n");
				int HexTotalLen = 0;
				for (int i = 0; i< m_nRegion; i++)
				{
					HexTotalLen += sHexData[i].HexDataLen;
				} 
				m_FlashChkSum = 0;
				for (int i = 0; i< m_nRegion; i++)
				{
					for (int j = 0; j < sHexData[i].HexDataLen; j++)
					{
						m_FlashChkSum += sHexData[i].HexData[j];
					}						
				}
				m_FlashChkSum += (iEndAddr - iStartAddr - HexTotalLen)*0xff;

			}
			m_FlashChkSum = 0xffffffff - m_FlashChkSum;
			m_FlashChkSum++;
			str.Format(L"0x%x", m_FlashChkSum);
			m_Result += str;
		}
		else
		{
			if (m_nRegion ==0)
			{
				m_Result.Format(L"���ݸ�ʽ����");
			}
			else
			{
				m_Result.Format(L"��Χ����");
			}
		
		}
		ShowInfo(m_Result, 2);
	}*/
}

void CBootDialog::OnBnClickedButtonReadaddr()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nDidCode;
	UpdateData(TRUE);
	nDidCode = m_DIDHexCode.GetInt();
		
	uint8 SendByte[8];

	SendByte[0] = 0x23;
	SendByte[1] = 0x24;
	SendByte[2] = nDidCode>>24;
	SendByte[3] = nDidCode>>16;
	SendByte[4] = nDidCode>>8;
	SendByte[5] = nDidCode&0x00ff;
	SendByte[6] = 0;
	SendByte[7] = 16;

	F_N_USDATA_REQ(SendByte, 8,ID_DEFINE_TARGET);
}

void CBootDialog::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_DisplayEn = FALSE;
	OnCancel();
}

void  InterLay_SetDiagID(uint8 bFun);
void CBootDialog::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	static uint8 mFunCtion;

	mFunCtion = !mFunCtion;

	if (mFunCtion)
	{
		GetDlgItem(IDC_BUTTON6)->SetWindowText(L"����");
	}
	else
	{
		GetDlgItem(IDC_BUTTON6)->SetWindowText(L"����");
	}

	InterLay_SetDiagID(mFunCtion);
}


void CBootDialog::OnEnChangeEdit2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������


}


void CBootDialog::OnBnClickedButtonReaddtc2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nDidCode  = 0xFDE2;
	UpdateData(TRUE);
	nDidCode = m_DIDHexCode.GetInt();
		
	uint8 SendByte[3];

	SendByte[0] = 0x22;
	SendByte[1] = 0xFD;
	SendByte[2] = 0xE2;
	F_N_USDATA_REQ(SendByte, 3,ID_DEFINE_TARGET);
}
