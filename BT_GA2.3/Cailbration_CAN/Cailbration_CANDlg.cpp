// Cailbration_CANDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Cailbration_CAN.h"
#include "Cailbration_CANDlg.h"
#include "ControlCAN.h"
#include "mmsystem.h"
#include "afxwin.h"
#include "SystemConfig.h"
#include "StdioFileEx.h"

#include "GWMMN/timerout.h"
#include "GWMMN/ApplayLay.h"
#include "GWMMN/NetLaye.h"
#include "ADD1.h"

uint8  F_N_USDATA_REQ(uint8* ReqData,uint16 Length,uint8 mId_Target);
CString str_pathDll;

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib,"controlcan.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEVICE_TYPE     4
#define DEVICE_ID       0
#define DEVICE_NUM      0


extern int TBL_RamDataValid[];
extern uint8  SessionMode;    //�Ựģʽ
extern uint8  SecurityState ;   //��ȫ����ģʽ

CCailbration_CANDlg *pWnd;
unsigned int  TIMEID = 0;
static int a_OPENBEGIN = 0;
extern uint8  scurityType;
void  NetLay_ReciveDiag(uint8 nId,uint8* L_data);
void RamRowSet(CStringArray  *col1);

static UINT indicators[] =
{
ID_SEPARATOR, // status line indicator
ID_INDICATOR_CLOCK, //CAP lock indicator.
ID_INDICATOR_NUM, //NUM lock indicator.
IDS_PANEL1, //SCRL lock indicator.
};

//  �ؼ���Ϣ����
static DLGCTLINFO  dcMenuGroup[] = 
{
    {IDOK, MOVEX, 100},

	{IDCANCEL, MOVEX, 100},	 
	{IDC_STATIC_MESSAGE, MOVEX, 100},	 
	{IDC_LIST_INFO, MOVEY, 100},	
	{IDC_LIST1, ELASTICXY, 100},
	//{IDC_PROGRESS1, MOVEXY, 100},

	{IDC_STATIC_MESSAGE, MOVEY, 100},	
	
	{IDC_STATIC_ICON, MOVEX, 100},
	{IDC_STATIC_COMSTATE, MOVEX, 100},
	{IDC_EDIT1, MOVEX, 100},
	{IDC_EDIT4, MOVEX, 100},
	{IDC_SPIN1, MOVEX, 100},
	{IDC_BUTTON_SAVE, MOVEXY, 100},
	{IDC_BUTTON_BEGIN, MOVEX, 100},
	{IDC_STATICGROUP_FUN, MOVEX, 100},
	
//	{IDC_EDIT_MESSAGE, MOVEX, -100},

	//{IDC_STATIC, MOVEX, 100},
	//{IDC_STATIC_BITMAP, MOVEX, 100},

	//{IDC_STATIC_ICON, MOVEX, 100},
	//{IDC_EDIT1, MOVEXY, 100},
	//{IDC_EDIT2, MOVEY, 100},
	//{IDC_BUTTON1, MOVEY, 100},
	//{IDC_STATIC_RECEIVE, MOVEXY, 100},
//ID FUN
	//{IDC_STATICGROUP_FUN, MOVEXY, 100},
	//{IDC_EDIT4, MOVEXY, 100},
	//{IDC_SPIN1, MOVEXY, 100},
	//{IDC_BUTTON_SAVE, MOVEXY, 100},
	//{IDC_BUTTON_BEGIN, MOVEXY, 100},




//    {IDC_GRID, ELASTICXY, 100},



	//ID RECEIVE TIMES
//	{IDC_STATIC_PICT, MOVEXY, 100},
	//{IDC_EDIT_TIMES, MOVEXY, 100},

 //   {ID_BUTTON_TOOL1, MOVEX, 50},

};
void  AppLay_DiagControl(void);
static void CALLBACK TimerCallBack(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	static unsigned int abc;
	uint8 data[8];
	abc++;
	MsCounterUpadat();
//	Deal_UDSNetLay();
	AppLay_DiagControl();
	if (abc ==10)
	{	
		abc = 0;
		//ReceiveFram();
	//	L_SendDiagFram(data);
	}
    return;
}

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMenueAbout();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//ON_COMMAND(ID_MENUE_ABOUT, &CAboutDlg::OnMenueAbout)
END_MESSAGE_MAP()


// CCailbration_CANDlg �Ի���




CCailbration_CANDlg::CCailbration_CANDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCailbration_CANDlg::IDD, pParent)
	, m_EditeString(_T(""))
	, m_22Message(_T(""))
	, m_RxRow(1)
	, Eeprom_Window_Flg(WINDOUS_FLG_RAM)
	,m_MessagePoint(0)
	, m_nStep(0)
	,m_Close(0)
	,m_BootLoadState(BT_STATE_END)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_EEPROM2);
}

void CCailbration_CANDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_EditeString);
	DDX_Control(pDX, IDC_LIST_INFO, m_ListInfo);
	//	DDX_Control(pDX, IDC_STATIC_ICON, m_COM_POCK);
	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_Edite22Message);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_22Message);
	DDX_Control(pDX, IDC_BUTTON_BEGIN, m_CButtonBegin);
	DDX_Control(pDX, IDC_LIST1, m_MessageList);
	DDX_Control(pDX, IDC_EDIT2, m_S19File);
	DDX_Control(pDX, IDC_EDIT13, m_FlashDriveFile);
	DDX_Control(pDX, IDC_PROGRESS1, m_LoadProgress);
}

BEGIN_MESSAGE_MAP(CCailbration_CANDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CCailbration_CANDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT1, &CCailbration_CANDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON_BEGIN, &CCailbration_CANDlg::OnBnClickedButtonBegin)
	ON_COMMAND(ID_BUTTON_TOOL1,OnToolbar1)
	ON_COMMAND(ID_BUTTON_TOOL2,OnToolbar2)
	ON_COMMAND(ID_BUTTON_TOOL3,OnToolbar3)
	ON_COMMAND(ID_BUTTON_TOOL4,OnToolbar4)
	ON_COMMAND(ID_BUTTON_TOOL5,OnCancel)
	ON_BN_CLICKED(IDCANCEL, &CCailbration_CANDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CCailbration_CANDlg::OnBnClickedButtonSave)
	ON_COMMAND(ID_MENU_COM, &CCailbration_CANDlg::OnMenuCom)
	ON_COMMAND(ID_Menu_BOOT, &CCailbration_CANDlg::OnMenuBoot)
	ON_LBN_SELCHANGE(IDC_LIST_INFO, &CCailbration_CANDlg::OnLbnSelchangeListInfo)
	ON_COMMAND(ID_MENUE_ABOUT, &CCailbration_CANDlg::OnMenueAbout)
	ON_COMMAND(ID_MENUE_EEPROM, &CCailbration_CANDlg::OnMenueEeprom)
	ON_EN_CHANGE(IDC_EDIT2, &CCailbration_CANDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON_OPENS19, &CCailbration_CANDlg::OnBnClickedButtonOpens19)
	ON_LBN_SELCHANGE(IDC_LIST1, &CCailbration_CANDlg::OnLbnSelchangeList1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CCailbration_CANDlg::OnNMCustomdrawProgress1)
	ON_EN_CHANGE(IDC_EDIT13, &CCailbration_CANDlg::OnEnChangeEdit13)
	ON_STN_CLICKED(IDC_STATIC_MESSAGE, &CCailbration_CANDlg::OnStnClickedStaticMessage)
	ON_BN_CLICKED(IDC_BUTTON_OPENS20, &CCailbration_CANDlg::OnBnClickedButtonOpens20)
END_MESSAGE_MAP()

//���ô�С����
BOOL CCailbration_CANDlg::SetControlProperty(PDLGCTLINFO lp, int nElements)
{
    //  ���ÿؼ�������Ϣ

    if (NULL == lp)
        return FALSE;

    if (nElements <= 0)
        return FALSE;

    m_pControlArray = lp;
    m_iControlNumber = nElements;

    return TRUE;
}

void CCailbration_CANDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialog::OnSizing(fwSide, pRect);

	// TODO: �ڴ˴������Ϣ����������
	    //  �Ի�����С�ڳ�ʼ��С

    int iWidth = pRect->right - pRect->left;
    int iHeight = pRect->bottom - pRect->top;

    if (iWidth <= m_iMinWidth)
        pRect->right = pRect->left + m_iMinWidth;
  
    if(iHeight <= m_iMinHeight)
        pRect->bottom = pRect->top + m_iMinHeight;

}

void CCailbration_CANDlg::OnSize(UINT nType, int cx, int cy)
{
//	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	CDialog::OnSize(nType, cx, cy);
 
    //  �Ի����Ⱥ͸߶ȵ����� 
    int iIncrementX = cx - m_iClientWidth;
    int iIncrementY = cy - m_iClientHeight;

    //  ��С��ʱ����Ϊ0
    if (nType == SIZE_MINIMIZED)
    {
        iIncrementX = iIncrementY = 0;
    }

    for (int i = 0; i < m_iControlNumber; i++)
    {
        CWnd *pWndCtrl = NULL;

        int iId = m_pControlArray[i].iId;
        int iFlag = m_pControlArray[i].iFlag;
        int iPercent = m_pControlArray[i].iPercent;

        //  ��Чֵ
        if ((iPercent < 0) || (iPercent > 100))
            continue;

        //  �õ��ؼ�ָ��
        pWndCtrl = GetDlgItem(iId);
        if ((NULL != pWndCtrl) && IsWindow(pWndCtrl->GetSafeHwnd()))
        {
            CRect rectCtrl;
            pWndCtrl->GetWindowRect(rectCtrl);

            ScreenToClient(rectCtrl);

            int iLeft = rectCtrl.left;
            int iTop = rectCtrl.top;
            int iWidth = rectCtrl.Width();
            int iHeight = rectCtrl.Height();

            switch (iFlag)
            {
            case MOVEX:  //  X�����ƶ�
                iLeft += (iIncrementX * iPercent / 100);
                break;

            case MOVEY:  //  Y�����ƶ�
                iTop += (iIncrementY * iPercent / 100);
                break;

            case MOVEXY:  //  X�����Y����ͬʱ�ƶ�
                iLeft += (iIncrementX * iPercent / 100);
                iTop += (iIncrementY * iPercent / 100);
                break;

            case ELASTICX:  //  X����ı��С
                iWidth += (iIncrementX * iPercent / 100);
                break;

            case ELASTICY:  //  Y����ı��С
                iHeight += (iIncrementY * iPercent / 100);
                break;

            case ELASTICXY:  //  X�����Y����ͬʱ�ı��С
                iWidth += (iIncrementX * iPercent / 100);
                iHeight += (iIncrementY * iPercent / 100);
                break;

            default:
                ;
            }

            //  �ѿؼ��ƶ�����λ��
            pWndCtrl->MoveWindow(iLeft, iTop, iWidth, iHeight);
		}

	}

	  //  ��ͼ���ƶ����Ի������½�
  /*  if (IsWindow(m_wndSizeIcon.GetSafeHwnd()))
    {
        m_wndSizeIcon.MoveWindow(cx - m_bitmap.bmWidth, cy - m_bitmap.bmHeight, m_bitmap.bmWidth, m_bitmap.bmHeight);

        if (nType == SIZE_MAXIMIZED)
             m_wndSizeIcon.ShowWindow(FALSE);
        else
            m_wndSizeIcon.ShowWindow(TRUE);
    }

    Invalidate();*/

    //  ��¼�Ի���client����Ĵ�С
    if (nType != SIZE_MINIMIZED)
    {
        m_iClientWidth = cx;
        m_iClientHeight = cy;
    }
    
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);



}

// CCailbration_CANDlg ��Ϣ�������
void CCailbration_CANDlg::InitGrid(void)
{
	
}

BOOL CCailbration_CANDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�
    ModifyStyle(0, WS_SIZEBOX);
	SetControlProperty(dcMenuGroup, sizeof(dcMenuGroup)/sizeof(DLGCTLINFO));

    //  �ԶԻ���ĳ�ʼ��С��Ϊ�Ի���Ŀ�Ⱥ͸߶ȵ���Сֵ
    CRect rectDlg;
    GetWindowRect(rectDlg);
    m_iMinWidth = rectDlg.Width();
    m_iMinHeight = rectDlg.Height();

    //  �õ��Ի���client����Ĵ�С
    CRect rectClient;
    GetClientRect(rectClient);
    m_iClientWidth = rectClient.Width();
    m_iClientHeight = rectClient.Height();
	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
		CSpinButtonCtrl *pSpinCtrl=(CSpinButtonCtrl*) GetDlgItem(IDC_SPIN1);
	CEdit *pEdit=(CEdit*) GetDlgItem(IDC_EDIT4);
	pSpinCtrl->SetBuddy(pEdit);
	pSpinCtrl->SetRange(1,100);
	pSpinCtrl->SetPos(2); //spin edite set

	//���빤����
	m_ImageList.Create(32,32,ILC_COLOR24|ILC_MASK,1,1);//ͼƬ��С����һ��
	CBitmap bmp;

     //��ͼ���б������ͼ��
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_COM));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_EEPROM2));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_FX2));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_OFF));
   //  m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICONUSER));
	UINT nArray[6];
	for(int i=0;i<6;i++)
	{
		nArray[i]=ID_BUTTON_TOOL1+i;
	}
	m_Toolbar.CreateEx(this);//�����������ؼ�,CreateEx���������Ĺ�������Ĭ���и�����ť����
	m_Toolbar.SetButtons(nArray,6);//�����������߰�ť������������ID�ţ���ӦnArray�����Ԫ��
	m_Toolbar.SetSizes(CSize(56,60),CSize(32,32));//������һ���ǰ�ť��С���ڶ�����ͼ���С����ť�����ͼ��Ҫ�󣬾����ǰ�ť��Ҫ

	//��ͼ��Ŀ��7�������ϣ���6
	m_Toolbar.GetToolBarCtrl().SetImageList(&m_ImageList);//����ͼ��


	m_Toolbar.SetButtonText(0,L"ͨ������ ");
	m_Toolbar.SetButtonText(1,L"EEPROM");
	m_Toolbar.SetButtonText(2,L"RAMSET");
	m_Toolbar.SetButtonText(3,L"��ֵ��");
	m_Toolbar.SetButtonText(4,L"�˳�ϵͳ");
	//m_Toolbar.SetButtonText(7,L"����Ա����");
//	m_Toolbar.SetSizes(CSize(56,60),CSize(32,32));//������һ���ǰ�ť��С���ڶ�����ͼ���С����ť�����ͼ��Ҫ�󣬾����ǰ�ť��Ҫ
	
	
	m_Rebar.Create(this);//�������ڣ��ؼ���
	m_Rebar.AddBar(&m_Toolbar);//���m_ToolbarΪ�Ӵ���
	m_Rebar.RedrawWindow();//�ػ�����
	REBARBANDINFO info;
	info.cbSize=sizeof(info);
	info.fMask=RBBIM_BACKGROUND;
	m_Toolbar.ModifyStyle(0,TBSTYLE_TRANSPARENT);//���ù���������ɫ͸��
	info.hbmBack=LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_TOOLBACK));//����λͼ
	m_Rebar.GetReBarCtrl().SetBandInfo(0,&info);
	
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);
	//CreateMenu();
	menu.LoadMenu(IDR_MENU2);
	//SetMenu(NULL);
	SetMenu(&menu);
	if(!m_wndStatusBar.Create(this)||  !m_wndStatusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT))) 
	{  TRACE0("Can't create status bar\n"); return false; } 
//	m_wndStatusBar.MoveWindow(0,rect.bottom-20,rect.right,20);
	UINT nID=0; //����״̬������ķ���
	
	m_wndStatusBar.SetPaneInfo(0,nID,SBPS_STRETCH|SBPS_NOBORDERS,100); //����ֵ��nID��
	
	m_wndStatusBar.SetPaneText(0,L"����");
	//m_wndStatusBar.SetPaneInfo(1,nID,SBPS_NORMAL,100);
	//m_wndStatusBar.SetPaneText(1,L"��д1");
	//m_wndStatusBar.SetPaneInfo(2,nID,SBPS_POPOUT,100);
	//m_wndStatusBar.SetPaneText(2,L"����1");

	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);
   	SetTimer(1,1000,NULL);//��װ��ʱ����������ʱ������Ϊ1000����
	
	InitGrid();

	pWnd = this;
	GetDlgItem(IDC_BUTTON_BEGIN)->EnableWindow(false);

	m_MessagePoint = 1;
	GetDlgItem(IDC_BUTTON_SAVE)->SetWindowText(L"��ʾ��");


	AfxBeginThread(ReceiveThread,this);
	AfxBeginThread(SendThread,this);
	TIMEID = timeSetEvent(1,1,&TimerCallBack,(DWORD)this,1);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CCailbration_CANDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCailbration_CANDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CCailbration_CANDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCailbration_CANDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CCailbration_CANDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}



void CCailbration_CANDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nIDEvent==1)
	{
		CTime time;
		time=CTime::GetCurrentTime();//�õ���ǰʱ��
		CString s=time.Format(L"%H:%M:%S");//ת��ʱ���ʽ
		//SetPaneText(1,L"��д1");
		m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_CLOCK),s); //��ʾʱ��'
		if (sCANSet.bSend==1)
		{
			VCI_CAN_OBJ vcoData;
			int  Speed;
			Speed = sCANSet.iVeicleSpeed * 160/9;

			vcoData.ID = sCANSet.id;
			vcoData.Data[1] = Speed %128;
			vcoData.Data[0] = Speed /128;

			vcoData.Data[6] = (sCANSet.iTWater + 48) * 4 / 3;
			SendCanMessage(&vcoData);

		}
	}
	else	if(nIDEvent == ID_TIMER_COM)
	{
		//Send �����շ�����
		BootLoad_ONTIMER(nIDEvent);
	}
	CDialog::OnTimer(nIDEvent);
}

//��ʾ����
void CCailbration_CANDlg::ShowInfo(CString str, int code)
{
	if (m_MessagePoint)
	{
		m_ListInfo.InsertString(m_ListInfo.GetCount(),str);
		m_ListInfo.SetCurSel(m_ListInfo.GetCount()-1);
	}
}

void CCailbration_CANDlg::ShowMainInfo(CString str, int code)
{
	m_MessageList.InsertString(m_MessageList.GetCount(),str);
	m_MessageList.SetCurSel(m_MessageList.GetCount()-1);
}

int panny_call_back(int a, int b)
{
	int c;
	c = a + b;
	return 11;
}

void CCailbration_CANDlg::OnBnClickedButtonBegin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_BootLoadState == BT_STATE_END || m_BootLoadState == BT_STATE_START)
	{
		m_BootLoadState = BT_STATE_START;
		SetTimer(ID_TIMER_COM,500,NULL);
		ShowMainInfo(L"��ʼ����",2);
		m_LogFile.CreatFile();
		uint8 SendByte[3];
		SendByte[0] = 0x22;
		SendByte[1] = (uint8)(ROUTINE_ID_CHECKSUM >>8);   
		SendByte[2] = (uint8)(ROUTINE_ID_CHECKSUM & 0xff);
		F_N_USDATA_REQ(SendByte, 3,ID_DEFINE_TARGET);
		u32OldCheckSum = 0;

		m_LoadProgress.SetRange(0,100);
			m_LoadProgress.SetPos(0);
		UpdateData(FALSE);
		m_Close = 0;
	}
	else
	{
		m_BootLoadState = BT_STATE_END;
		SetTimer(ID_TIMER_COM,500,NULL);
		ShowMainInfo(L"�������أ�����ȡ��",2);
		uint8 SendByte[3];
		SendByte[0] = 0x22;
		SendByte[1] = (uint8)(ROUTINE_ID_CHECKSUM >>8);   
		SendByte[2] = (uint8)(ROUTINE_ID_CHECKSUM & 0xff);
		F_N_USDATA_REQ(SendByte, 3,ID_DEFINE_TARGET);
		UpdateData(FALSE);
		m_Close = 1;
		m_LoadTask = LOAD_TASK_FAILED;
	}
	
	//SessionMode = SESSION_MODE_PROGRAM;
/*	int p = 10;
	int panny1;
	//panny1 = panny(p, panny_call_back);
	//_RecordsetPtr m_pRecordset;


	//ado.OnInitADOConn(L"id.mdb");//L"id.dbc"
		
//	m_pRecordset   = ado.GetRecordset();
//	m_pRecordset = ado.OpenRecordset(L"select *  from user2");


	if(a_OPENBEGIN==0)
	{
		
		a_OPENBEGIN=1;
		m_CButtonBegin.SetWindowText(L"�ر�");
		UpdateData(TRUE);
		CSpinButtonCtrl *pSpinCtrl=(CSpinButtonCtrl*) GetDlgItem(IDC_SPIN1);

		m_nStep=pSpinCtrl->GetPos();
		m_nStep=m_nStep*1000;
		SetTimer(ID_TIMER_COM,m_nStep,NULL);
	}
	else 
	{
		a_OPENBEGIN=0;
		KillTimer(ID_TIMER_COM);
		m_CButtonBegin.SetWindowText(L"��ʼ");
	}*/

}



//toobar  button
void SetSecurityType(uint8 id);
void CCailbration_CANDlg::OnToolbar1(void)
{
 //INT_PTR nRet=m_ComDlg.DoModal();

	INT_PTR nRet=m_ComDlg.DoModal();

	sCanCofig = m_ComDlg.SetConfig;
	ID_DIAG_FUNCT = sCanCofig.FunID; 
	ID_DIAG_PHYSIC = sCanCofig.PhyID; 
	ID_DIAG_CLM = sCanCofig.ECUID;
	ID_FRAMESIZE = sCanCofig.FramSize;
	SECURITY_MASK_LEVEL1 = sCanCofig.mask1;
	SECURITY_MASK_LEVEL2 = sCanCofig.mask2;

	VCI_INIT_CONFIG init_config;
	init_config.AccCode = 0x00000000;
	init_config.AccMask = 0xffffffff;
	init_config.Filter = 0;
	init_config.Mode = 0;
	init_config.Timing0 = 0x04;
	init_config.Timing1 = 0x1c;
	
	scurityType = m_ComDlg.m_ScurityType;

	str_pathDll = sCanCofig.dllPath;

	
	SECURITY_REQSD_LEV2 = sCanCofig.u8Security2;
	SECURITY_SENDK_LEV2 = sCanCofig.u8Security2+1;

	switch (sCanCofig.Band)
	{
	default:
		break;
	case 100: //100k
		init_config.Timing0 = 0x04;
		init_config.Timing1 = 0x1c;
		break;
	case 500:
		init_config.Timing0 = 0;
		init_config.Timing1 = 0x1c;
		break;
	case 250:
		init_config.Timing0 = 0x01;
		init_config.Timing1 = 0x1c;
		break;
	}


	SetSecurityType(m_ComDlg.ProjectID);

	switch ( nRet )
	{
		case -1: 
			AfxMessageBox(L"Dialog box could not be created!");
		break;
		case IDABORT:
		  // Do something
			break;
		case IDOK:
		//OK BUTTON
			if(m_ComDlg.m_Enable)
			{
				if(VCI_OpenDevice(DEVICE_TYPE,DEVICE_ID,0)!=STATUS_OK)
				{
					MessageBox(L"���豸ʧ��!",L"����",MB_OK|MB_ICONQUESTION);
					m_ComDlg.m_Enable =0 ;
					return;
				}
				if(VCI_InitCAN(DEVICE_TYPE,DEVICE_ID,DEVICE_NUM,&init_config)!=STATUS_OK)
				{
					MessageBox(L"��ʼ��CANʧ��!",L"����",MB_OK|MB_ICONQUESTION);
					m_ComDlg.m_Enable = 0;
					VCI_CloseDevice(DEVICE_TYPE,DEVICE_ID);
					return;
				}
			}
			else
			{
			//�ر�CAN
				m_ComDlg.m_Enable = 0;
				VCI_CloseDevice(DEVICE_TYPE,DEVICE_ID);
			}

		// Do something
			break;
		case IDCANCEL:
			return ;
      // Do something
			break;
		default:
      // Do something
			AfxMessageBox(L"Dialog box DEFAL!");
			break;
   }


   
	CStatic* m_COM_POCK =(CStatic* ) this->GetDlgItem (IDC_STATIC_ICON);
//	CBitmap bmpLoad;
	
	

	if(m_ComDlg.m_Enable)
	{
		m_COM_POCK->SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_POCK3));
		GetDlgItem(IDC_BUTTON_BEGIN)->EnableWindow(true);
	//	bmpLoad.LoadBitmap(IDB_BITMAP5);
	}
	else
	{
		m_COM_POCK->SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_POCK6));
		GetDlgItem(IDC_BUTTON_BEGIN)->EnableWindow(false);
	//	bmpLoad.LoadBitmap(IDB_BITMAP4);
	}
	//m_p->SetBitmap((HBITMAP)bmpLoad.Detach());
	ShowInfo(L"���ӳɹ�: ", 1);

	if(VCI_StartCAN(DEVICE_TYPE,DEVICE_ID,DEVICE_NUM)==1)
	{
		ShowInfo(L"�����ɹ�",0);	

	  //  SessionMode = SESSION_MODE_EXTERN;    //�Ựģʽ
       // SecurityState = SECURITY_STATE_SSEED1;   //��ȫ����ģʽ
	}
	else
	{
		CString str;
		str=L"����ʧ��";
		ShowInfo(str,2);
	}
	//
	//UpdateData(FALSE);
	return ;

}

//on toolbar2
void CCailbration_CANDlg::OnToolbar2(void)
{
	if(a_OPENBEGIN)  OnBnClickedButtonBegin();  //�رշ���
	Eeprom_Window_Flg=WINDOUS_FLG_ROM;
	//EeDlg.SetCom(&ComDlg);
	int *p = new int[m_iClientWidth];
	
	EeDlg.DoModal();
	Eeprom_Window_Flg=WINDOUS_FLG_RAM;
}
void CCailbration_CANDlg::OnToolbar3(void)
{
/*	CCheckDialog dlgCheck;
	//dlgAbout.DoModal();

	if (dlgCheck.DoModal() == IDOK)
	{
		m_Grid.DeleteAllItems();
		InitGrid();
		m_Grid.Refresh();
		 m_RxRow = 1;

//sCANSet
	}
	else
	{
		sCANSet.bSend = 0;
	}
	*/

	SendDialog *m_pDlg;

	//if (m_pDlg!=NULL)
	{
		// ������ģ̬�Ի���ʵ�� 

		
	}

	m_pDlg=new SendDialog;
	//(IDD_DIALOG1, this); 
	m_pDlg->Create(134, this);      //��һ����Ϊ�½��Ի����ID��
	m_pDlg->ShowWindow(SW_SHOW);    // // ��ʾ��ģ̬�Ի���  

}

void CCailbration_CANDlg::OnToolbar4(void)
{
	CCANSetDialog dlgAbout;
	//dlgAbout.DoModal();

	if (dlgAbout.DoModal() == IDOK)
	{
		sCANSet.bSend = 1;
		sCANSet.iVeicleSpeed = dlgAbout.m_VecleSpeed;
		sCANSet.iTWater = dlgAbout.m_Twater;
		sCANSet.id = dlgAbout.m_ID;
//sCANSet
	}
	else
	{
		sCANSet.bSend = 0;
	}

}

UINT CCailbration_CANDlg::SendThread(void *param)
{
	while(1)
	{
		Deal_UDSNetLay();
	}
}
//���ܺ����ڴ�ʵ��
UINT CCailbration_CANDlg::ReceiveThread(void *param)
{
	CCailbration_CANDlg *dlg=(CCailbration_CANDlg*)param;
	CListBox *box=(CListBox *)dlg->GetDlgItem(IDC_LIST_INFO);
	VCI_CAN_OBJ frameinfo[50];
	VCI_ERR_INFO errinfo;
	int len=1;
	int i=0;
	CString str,tmpstr;
	
	while(1)
	{
	//	ShowTime();
	//	TIME++;
	//	Deal_UDSNetLay();
		if (pWnd->m_ComDlg.m_Enable == 0)  //һֱ���ջᵼ�¹ر�CAN �ĳ���ͽ������ݳ����ͻ
		{}
		else{
			len=VCI_Receive(DEVICE_TYPE , DEVICE_ID, DEVICE_NUM ,frameinfo,50,200);
		}
		if (pWnd->m_ComDlg.m_Enable == 0)
		{
		}
		else if(len<=0)
		{
			//ע�⣺���û�ж��������������ô˺�������ȡ����ǰ�Ĵ����룬
			//ǧ����ʡ����һ������ʹ����ܲ���֪����������ʲô��
			VCI_ReadErrInfo(DEVICE_TYPE, DEVICE_ID, DEVICE_NUM, &errinfo);
		}                                                                              
		else
		{
#if 1
			dlg->m_recvcount += len;
		//	str.Format("         ���ν��� %d ֡  �ܽ��� %d ֡",len,dlg->m_recvcount);
		//	box->InsertString(box->GetCount(),str);
			for(i=0;i<len;i++)
			{
				/*str="���յ�CAN֡:  ";
				if(frameinfo[i].TimeFlag==0)
					tmpstr="ʱ���ʶ:��  ";
				else
					tmpstr.Format("ʱ���ʶ:%08x ",frameinfo[i].TimeStamp);
				str+=tmpstr;
				tmpstr.Format("֡ID:%08x ",frameinfo[i].ID);
				str+=tmpstr;
				str+="֡��ʽ:";
				if(frameinfo[i].RemoteFlag==0)
					tmpstr="����֡ ";
				else
					tmpstr="Զ��֡ ";
				str+=tmpstr;
				str+="֡����:";
				if(frameinfo[i].ExternFlag==0)
					tmpstr="��׼֡ ";
				else
					tmpstr="��չ֡ ";
				str+=tmpstr;
 				box->InsertString(box->GetCount(),str);*/
#if 1
				if(frameinfo[i].RemoteFlag==0 && (frameinfo[i].ID == ID_DIAG_PHYSIC|| frameinfo[i].ID == ID_DIAG_FUNCT|| frameinfo[i].ID == ID_DIAG_CLM))
				{
					if (dlg->Eeprom_Window_Flg == WINDOUS_FLG_BOOT)
					{
						str="RX:";
						tmpstr.Format(L"ID %02x  Data:", frameinfo[i].ID );
						str += tmpstr;
						if(frameinfo[i].DataLen>8)
							frameinfo[i].DataLen=8;
						for(int j=0;j<frameinfo[i].DataLen;j++)
						{
							tmpstr.Format(L"%02x ",frameinfo[i].Data[j]);
							str+=tmpstr;
						}

						SYSTEMTIME st;
						CString strTime;
						GetLocalTime(&st);
						//	strDate.Format("%4d-%2d-%2d",st.wYear,st.wMonth,st.wDay);
						strTime.Format(L"time��%2d:%2d:%2d",st.wMinute,st.wSecond,st.wMilliseconds) ;
						str += strTime;
						dlg->BDialog_ShowInfo(str);
					}
					else
					{
						str="RX:";
						tmpstr.Format(L"ID %02x  Data:", frameinfo[i].ID );
						str += tmpstr;
						if (pWnd->m_MessagePoint == 1)
							box->InsertString(box->GetCount(),str);
						pWnd->m_LogFile.WriteFile(str);
						str="";
						if(frameinfo[i].DataLen>8)
							frameinfo[i].DataLen=8;
						for(int j=0;j<frameinfo[i].DataLen;j++)
						{
							tmpstr.Format(L"%02x ",frameinfo[i].Data[j]);
							str+=tmpstr;
						}

						
						SYSTEMTIME st;
						CString strTime;
						GetLocalTime(&st);
						//	strDate.Format("%4d-%2d-%2d",st.wYear,st.wMonth,st.wDay);
						strTime.Format(L"time��%2d:%2d:%2d",st.wMinute,st.wSecond,st.wMilliseconds) ;
						str += strTime;


						if (pWnd->m_MessagePoint == 1)
							box->InsertString(box->GetCount(),str);	
						pWnd->m_LogFile.WriteFile(str);
					}
					//ShowTime();
				}
#endif
				if (frameinfo[i].DataLen == 8 && frameinfo[i].RemoteFlag ==0)
				{
					if (frameinfo[i].ID == ID_DIAG_CLM)
					{
						NetLay_ReciveDiag((unsigned char)(ID_DIAG_CLM&0x00ff), frameinfo[i].Data);    
					}
					else if (frameinfo[i].ID == ID_DIAG_PHYSIC)
					{
						NetLay_ReciveDiag((unsigned char)(ID_DIAG_PHYSIC&0x00ff), frameinfo[i].Data); 
					}
				}
// 				if (box->GetCount()>50)
// 				{
// 					box->ResetContent();
// 				}
			}
		/*	if (dlg->m_btest)
			{
			//	frameinfo[0].Data[0]=0xff;
			//	frameinfo[0].Data[1]=0xff;
				ULONG ret=VCI_Transmit(dlg->m_devtype,dlg->m_devind,dlg->m_cannum,&frameinfo[0],len);
				dlg->m_testcount += ret;
				CTime tm=CTime::GetCurrentTime();
				if ((int)ret!=len)
				{
					str.Format("���λظ� %d ֡  ʵ�ʻظ� %d ֡ �ܻظ� %d ֡ ----------�ظ�������",len,ret,dlg->m_testcount);

				}else
				{
					str.Format("���λظ� %d ֡  ʵ�ʻظ� %d ֡ �ܻظ� %d ֡",len,ret,dlg->m_testcount);
				}
				box->InsertString(box->GetCount(),str);
				str=tm.Format("%Y_%m_%d_%H_%M_%S") +" --- " + str;
				dlg->m_f.Write(str,str.GetLength());
			}*/
	//		if (pWnd->m_MessagePoint == 1)
	//			box->SetCurSel(box->GetCount()-1);
#endif


		}
	}
	return 0;
}


void ShowData(VCI_CAN_OBJ sVData)
{
	CString str;

	//if (pWnd->m_MessagePoint == 0)return;
   //CTestDlg *dlg=(CTestDlg*)param;	
	CListBox *box=(CListBox *)pWnd->GetDlgItem(IDC_LIST_INFO);
	
	str.Format(L"TX : ID - %X, DATA - %x, %x, %x, %x, %x, %x, %x, %x", sVData.ID ,sVData.Data[0], sVData.Data[1], sVData.Data[2],
		sVData.Data[3], sVData.Data[4], sVData.Data[5], sVData.Data[6], sVData.Data[7]);

	SYSTEMTIME st;
	CString strDate,strTime;
	GetLocalTime(&st);
	//	strDate.Format("%4d-%2d-%2d",st.wYear,st.wMonth,st.wDay);
	strTime.Format(L"time��%2d:%2d:%2d",st.wMinute,st.wSecond,st.wMilliseconds) ;
	str += strTime;
    if (pWnd->m_MessagePoint == 1)
	{
		box->InsertString(box->GetCount(),str);
	}
	if (pWnd->m_LogFile.FileIsOK() == FALSE)
	{
		str = L"ABC";
	}
	else
		pWnd->m_LogFile.WriteFile(str);

}

//UDS CAN �ӿں���
//�ӿں�����
uint8 DiagID;
uint16 sendtimes;
uint16 debugtime;
void  L_SendDiagFram(uint8* NetData)
{
	//CWnd *wnd  = AfxGetMainWnd();
	//CTestDlg *pWnd = (CTestDlg*)wnd;
	VCI_CAN_OBJ vco[2];	                                      //����C***Dialog�еĺ���xxx();
	
	if (DiagID)
	{
		vco[0].ID = ID_DIAG_FUNCT; // ��д��һ֡��ID 
	}
	else
	{
		vco[0].ID = ID_DIAG_PHYSIC; // ��д��һ֡��ID 
	}
	vco[0].SendType = 0; // �������� 
	vco[0].RemoteFlag = 0; // ����֡ 
	vco[0].ExternFlag = (uint8)ID_FRAMESIZE; // ��׼֡ 
	vco[0].DataLen = 8; // ���ݳ���1���ֽ� 

	for (int i =0 ; i< 8; i++)
		vco[0].Data[i] = NetData[i]; // ����0Ϊ0x66
//	if (TIMEID) return ;
	if( pWnd->m_ComDlg.m_Enable ==0)
		return;
	if(VCI_Transmit(DEVICE_TYPE, DEVICE_ID, DEVICE_NUM, vco,1)==1)
	{
	//	m_sendcnt += 1;

		if (pWnd->Eeprom_Window_Flg == WINDOUS_FLG_BOOT)
		{
			CString str;

			str.Format(L"TX : ID - %X, DATA - %x, %x, %x, %x, %x, %x, %x, %x", vco[0].ID ,vco[0].Data[0], vco[0].Data[1], vco[0].Data[2],
				vco[0].Data[3], vco[0].Data[4], vco[0].Data[5], vco[0].Data[6], vco[0].Data[7]);
			
			pWnd->BDialog_ShowInfo(str);
		}
		else
			ShowData(vco[0]);	
	}
	else
	{
		pWnd->ShowInfo(L"д��ʧ��",2);		
	}
}

void  InterLay_SetDiagID(uint8 bFun)
{
	DiagID = bFun;
}

void ApplayTranslate(uint8 *data)
{
	if (data[0] != 0x7f)
		pWnd->Deal_TransSever();
	else
	{
		CString str;
		str.Format(L"����ʧ��:0x%x, 0x%x", data[1], data[2]);
		//data[3]++;
		pWnd->ShowInfo(str,2);
	}
}

void Receive_AppData(A_PDU *pAPdu)
{
	pWnd->ReceiveAPPData(pAPdu);
}
void CCailbration_CANDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CCailbration_CANDlg::OnCancel()
{
	VCI_ERR_INFO errinfo;

	VCI_INIT_CONFIG init_config;
	init_config.AccCode = 0x00000000;
	init_config.AccMask = 0xffffffff;
	init_config.Filter = 0;
	init_config.Mode = 0;
	init_config.Timing0 = 0;
	init_config.Timing1 = 0x1c;

	m_ComDlg.m_Enable = 0;
//	VCI_CloseDevice(DEVICE_TYPE,DEVICE_ID);




	//Sleep(500);
	
	VCI_ReadErrInfo(DEVICE_TYPE, DEVICE_ID, DEVICE_NUM, &errinfo);
	
	Sleep(500);
		//VCI_CloseDevice(DEVICE_TYPE ,DEVICE_ID);
		//VCI_StartCAN(DEVICE_TYPE,DEVICE_ID,DEVICE_NUM);
		//m_ComDlg.m_Enable = 0;
		//Sleep(500);		

	//DeleteCriticalSection(&m_Section);
	CDialog::OnCancel();
}
void CCailbration_CANDlg::OnBnClickedButtonSave()
{
	if (m_MessagePoint)
	{
		m_MessagePoint = 0;
		GetDlgItem(IDC_BUTTON_SAVE)->SetWindowText(L"��ʾ��");
	}
	else
	{
		m_MessagePoint = 1;
		GetDlgItem(IDC_BUTTON_SAVE)->SetWindowText(L"��ʾ��");
	}

}


//����
void CCailbration_CANDlg::OnMenuCom()
{
	// TODO: �ڴ���������������
	OnToolbar1();
}
//�˵�����bootload
void CCailbration_CANDlg::OnMenuBoot()
{
	// TODO: �ڴ���������������
	if(a_OPENBEGIN)  OnBnClickedButtonBegin();  //�رշ���
	Eeprom_Window_Flg=WINDOUS_FLG_BOOT;
	//EeDlg.SetCom(&ComDlg);
	BootDlg.DoModal();
	Eeprom_Window_Flg=WINDOUS_FLG_RAM;
}

void CCailbration_CANDlg::OnLbnSelchangeListInfo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

//�� bootdialog ����ʾ����
void CCailbration_CANDlg::BDialog_ShowInfo(CString str)
{
	//�жϵ�ǰ�����ǲ�����bootload ����

		CWnd* ahWnd = FindWindow( NULL,BOOT_DIALOG1_NAME ) ;	
		if(ahWnd)
		{
			BootDlg.ShowInfo(str, 1);
		}
}



void CCailbration_CANDlg::OnMenueAbout()
{
	// TODO: �ڴ���������������
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();

}

void CCailbration_CANDlg::OnMenueEeprom()
{
	// TODO: �ڴ���������������
	OnToolbar2();
}


void  CCailbration_CANDlg::SendCanMessage(VCI_CAN_OBJ* vco)
{
	//CWnd *wnd  = AfxGetMainWnd();
	//CTestDlg *pWnd = (CTestDlg*)wnd;
	                                      //����C***Dialog�еĺ���xxx();
	
	//vco.ID = ID_DIAG_PHYSIC; // ��д��һ֡��ID 
	vco->SendType = 0; // �������� 
	vco->RemoteFlag = 0; // ����֡ 
	vco->ExternFlag = sCanCofig.FramSize; // ��׼֡ 
	vco->DataLen = 8; // ���ݳ���1���ֽ� 

//	if (TIMEID) return ;
	if( m_ComDlg.m_Enable ==0)
		return;
	if(VCI_Transmit(DEVICE_TYPE, DEVICE_ID, DEVICE_NUM, vco,1)==1)
	{
	//	m_sendcnt += 1;
	//	if (Eeprom_Window_Flg == WINDOUS_FLG_BOOT)
	//	{
			CString str;

			str.Format(L"TX : ID - %X, DATA - %x, %x, %x, %x, %x, %x, %x, %x", vco[0].ID ,vco[0].Data[0], vco[0].Data[1], vco[0].Data[2],
				vco[0].Data[3], vco[0].Data[4], vco[0].Data[5], vco[0].Data[6], vco[0].Data[7]);
			
			BDialog_ShowInfo(str);
	//	}
	//	else
		//	ShowData(vco[0]);		
	}
	else
	{
		ShowInfo(L"д��ʧ��",2);		
	}
}




void CCailbration_CANDlg::OnBnClickedButtonOpens19()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		CString strName;
	CString str, m_Result;
	//CFileDialog dlg(FALSE,_T("s19; hex"),_T("*.s19; *.hex"),OFN_EXPLORER,_T("�����ļ�(.s19)| *.s19; *.hex|�����ļ���*.*)|*.*||)"));
	CFileDialog dlg(FALSE,_T("s19; hex"),_T("*.s19; *.hex"),OFN_EXPLORER,_T("�����ļ�(.s19)| *.s19; *.hex|�����ļ���*.*)|*.*||)"));
	if(dlg.DoModal()==IDOK)
	{
		strName=dlg.GetPathName();
		m_S19File.SetWindowTextW(strName);
	}		
	else
	{
		ShowMainInfo(L"�ļ�����ʧ��", 2);
	}
	UpdateData(FALSE);
}


void CCailbration_CANDlg::OnLbnSelchangeList1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CCailbration_CANDlg::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}





void CCailbration_CANDlg::OnStnClickedStaticMessage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CCailbration_CANDlg::OnBnClickedButtonOpens20()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strName;
	CString str, m_Result;

	//CFileDialog dlg(FALSE,_T("s19; hex"),_T("*.s19; *.hex"),OFN_EXPLORER,_T("�����ļ�(.s19)| *.s19; *.hex|�����ļ���*.*)|*.*||)"));
	CFileDialog dlg(FALSE,_T("s19; hex"),_T("*.s19; *.hex"),OFN_EXPLORER,_T("�����ļ�(.s19)| *.s19; *.hex|�����ļ���*.*)|*.*||)"));
	if(dlg.DoModal()==IDOK)
	{
		strName=dlg.GetPathName();
		m_FlashDriveFile.SetWindowTextW(strName);
	}		
	else
	{
		m_FlashDriveFile.SetWindowTextW(L"");
		ShowMainInfo(L"�ļ�����ʧ��", 2);
	}
	UpdateData(FALSE);
}
