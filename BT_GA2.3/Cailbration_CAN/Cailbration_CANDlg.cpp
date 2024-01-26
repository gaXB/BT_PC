// Cailbration_CANDlg.cpp : 实现文件
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
extern uint8  SessionMode;    //会话模式
extern uint8  SecurityState ;   //安全访问模式

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

//  控件信息数组
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

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CCailbration_CANDlg 对话框




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

//设置大小函数
BOOL CCailbration_CANDlg::SetControlProperty(PDLGCTLINFO lp, int nElements)
{
    //  设置控件数组信息

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

	// TODO: 在此处添加消息处理程序代码
	    //  对话框不能小于初始大小

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

	// TODO: 在此处添加消息处理程序代码
	CDialog::OnSize(nType, cx, cy);
 
    //  对话框宽度和高度的增量 
    int iIncrementX = cx - m_iClientWidth;
    int iIncrementY = cy - m_iClientHeight;

    //  最小化时增量为0
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

        //  无效值
        if ((iPercent < 0) || (iPercent > 100))
            continue;

        //  得到控件指针
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
            case MOVEX:  //  X方向移动
                iLeft += (iIncrementX * iPercent / 100);
                break;

            case MOVEY:  //  Y方向移动
                iTop += (iIncrementY * iPercent / 100);
                break;

            case MOVEXY:  //  X方向和Y方向同时移动
                iLeft += (iIncrementX * iPercent / 100);
                iTop += (iIncrementY * iPercent / 100);
                break;

            case ELASTICX:  //  X方向改变大小
                iWidth += (iIncrementX * iPercent / 100);
                break;

            case ELASTICY:  //  Y方向改变大小
                iHeight += (iIncrementY * iPercent / 100);
                break;

            case ELASTICXY:  //  X方向和Y方向同时改变大小
                iWidth += (iIncrementX * iPercent / 100);
                iHeight += (iIncrementY * iPercent / 100);
                break;

            default:
                ;
            }

            //  把控件移动到新位置
            pWndCtrl->MoveWindow(iLeft, iTop, iWidth, iHeight);
		}

	}

	  //  把图标移动到对话框右下角
  /*  if (IsWindow(m_wndSizeIcon.GetSafeHwnd()))
    {
        m_wndSizeIcon.MoveWindow(cx - m_bitmap.bmWidth, cy - m_bitmap.bmHeight, m_bitmap.bmWidth, m_bitmap.bmHeight);

        if (nType == SIZE_MAXIMIZED)
             m_wndSizeIcon.ShowWindow(FALSE);
        else
            m_wndSizeIcon.ShowWindow(TRUE);
    }

    Invalidate();*/

    //  记录对话框client区域的大小
    if (nType != SIZE_MINIMIZED)
    {
        m_iClientWidth = cx;
        m_iClientHeight = cy;
    }
    
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);



}

// CCailbration_CANDlg 消息处理程序
void CCailbration_CANDlg::InitGrid(void)
{
	
}

BOOL CCailbration_CANDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。
    ModifyStyle(0, WS_SIZEBOX);
	SetControlProperty(dcMenuGroup, sizeof(dcMenuGroup)/sizeof(DLGCTLINFO));

    //  以对话框的初始大小作为对话框的宽度和高度的最小值
    CRect rectDlg;
    GetWindowRect(rectDlg);
    m_iMinWidth = rectDlg.Width();
    m_iMinHeight = rectDlg.Height();

    //  得到对话框client区域的大小
    CRect rectClient;
    GetClientRect(rectClient);
    m_iClientWidth = rectClient.Width();
    m_iClientHeight = rectClient.Height();
	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
		CSpinButtonCtrl *pSpinCtrl=(CSpinButtonCtrl*) GetDlgItem(IDC_SPIN1);
	CEdit *pEdit=(CEdit*) GetDlgItem(IDC_EDIT4);
	pSpinCtrl->SetBuddy(pEdit);
	pSpinCtrl->SetRange(1,100);
	pSpinCtrl->SetPos(2); //spin edite set

	//插入工具栏
	m_ImageList.Create(32,32,ILC_COLOR24|ILC_MASK,1,1);//图片大小必须一致
	CBitmap bmp;

     //向图像列表中添加图标
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
	m_Toolbar.CreateEx(this);//创建工具栏控件,CreateEx函数创建的工具栏，默认有浮动按钮属性
	m_Toolbar.SetButtons(nArray,6);//创建六个工具按钮，并依次设置ID号，对应nArray数组的元素
	m_Toolbar.SetSizes(CSize(56,60),CSize(32,32));//函数第一个是按钮大小，第二个是图像大小，按钮必须比图像要大，具体是按钮的要

	//比图像的宽大7或者以上，高6
	m_Toolbar.GetToolBarCtrl().SetImageList(&m_ImageList);//设置图像


	m_Toolbar.SetButtonText(0,L"通信设置 ");
	m_Toolbar.SetButtonText(1,L"EEPROM");
	m_Toolbar.SetButtonText(2,L"RAMSET");
	m_Toolbar.SetButtonText(3,L"阻值表");
	m_Toolbar.SetButtonText(4,L"退出系统");
	//m_Toolbar.SetButtonText(7,L"操作员管理");
//	m_Toolbar.SetSizes(CSize(56,60),CSize(32,32));//函数第一个是按钮大小，第二个是图像大小，按钮必须比图像要大，具体是按钮的要
	
	
	m_Rebar.Create(this);//创建窗口（控件）
	m_Rebar.AddBar(&m_Toolbar);//添加m_Toolbar为子窗口
	m_Rebar.RedrawWindow();//重画窗口
	REBARBANDINFO info;
	info.cbSize=sizeof(info);
	info.fMask=RBBIM_BACKGROUND;
	m_Toolbar.ModifyStyle(0,TBSTYLE_TRANSPARENT);//设置工具栏背景色透明
	info.hbmBack=LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_TOOLBACK));//加载位图
	m_Rebar.GetReBarCtrl().SetBandInfo(0,&info);
	
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);
	//CreateMenu();
	menu.LoadMenu(IDR_MENU2);
	//SetMenu(NULL);
	SetMenu(&menu);
	if(!m_wndStatusBar.Create(this)||  !m_wndStatusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT))) 
	{  TRACE0("Can't create status bar\n"); return false; } 
//	m_wndStatusBar.MoveWindow(0,rect.bottom-20,rect.right,20);
	UINT nID=0; //控制状态栏里面的分栏
	
	m_wndStatusBar.SetPaneInfo(0,nID,SBPS_STRETCH|SBPS_NOBORDERS,100); //返回值存nID中
	
	m_wndStatusBar.SetPaneText(0,L"就绪");
	//m_wndStatusBar.SetPaneInfo(1,nID,SBPS_NORMAL,100);
	//m_wndStatusBar.SetPaneText(1,L"大写1");
	//m_wndStatusBar.SetPaneInfo(2,nID,SBPS_POPOUT,100);
	//m_wndStatusBar.SetPaneText(2,L"数字1");

	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);
   	SetTimer(1,1000,NULL);//安装定时器，并将其时间间隔设为1000毫秒
	
	InitGrid();

	pWnd = this;
	GetDlgItem(IDC_BUTTON_BEGIN)->EnableWindow(false);

	m_MessagePoint = 1;
	GetDlgItem(IDC_BUTTON_SAVE)->SetWindowText(L"提示开");


	AfxBeginThread(ReceiveThread,this);
	AfxBeginThread(SendThread,this);
	TIMEID = timeSetEvent(1,1,&TimerCallBack,(DWORD)this,1);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCailbration_CANDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CCailbration_CANDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCailbration_CANDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CCailbration_CANDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}



void CCailbration_CANDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent==1)
	{
		CTime time;
		time=CTime::GetCurrentTime();//得到当前时间
		CString s=time.Format(L"%H:%M:%S");//转换时间格式
		//SetPaneText(1,L"大写1");
		m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_CLOCK),s); //显示时钟'
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
		//Send 启动收发程序
		BootLoad_ONTIMER(nIDEvent);
	}
	CDialog::OnTimer(nIDEvent);
}

//显示函数
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
	// TODO: 在此添加控件通知处理程序代码
	if (m_BootLoadState == BT_STATE_END || m_BootLoadState == BT_STATE_START)
	{
		m_BootLoadState = BT_STATE_START;
		SetTimer(ID_TIMER_COM,500,NULL);
		ShowMainInfo(L"开始下载",2);
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
		ShowMainInfo(L"正在下载，尝试取消",2);
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
		m_CButtonBegin.SetWindowText(L"关闭");
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
		m_CButtonBegin.SetWindowText(L"开始");
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
					MessageBox(L"打开设备失败!",L"警告",MB_OK|MB_ICONQUESTION);
					m_ComDlg.m_Enable =0 ;
					return;
				}
				if(VCI_InitCAN(DEVICE_TYPE,DEVICE_ID,DEVICE_NUM,&init_config)!=STATUS_OK)
				{
					MessageBox(L"初始化CAN失败!",L"警告",MB_OK|MB_ICONQUESTION);
					m_ComDlg.m_Enable = 0;
					VCI_CloseDevice(DEVICE_TYPE,DEVICE_ID);
					return;
				}
			}
			else
			{
			//关闭CAN
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
	ShowInfo(L"连接成功: ", 1);

	if(VCI_StartCAN(DEVICE_TYPE,DEVICE_ID,DEVICE_NUM)==1)
	{
		ShowInfo(L"启动成功",0);	

	  //  SessionMode = SESSION_MODE_EXTERN;    //会话模式
       // SecurityState = SECURITY_STATE_SSEED1;   //安全访问模式
	}
	else
	{
		CString str;
		str=L"启动失败";
		ShowInfo(str,2);
	}
	//
	//UpdateData(FALSE);
	return ;

}

//on toolbar2
void CCailbration_CANDlg::OnToolbar2(void)
{
	if(a_OPENBEGIN)  OnBnClickedButtonBegin();  //关闭发送
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
		// 创建非模态对话框实例 

		
	}

	m_pDlg=new SendDialog;
	//(IDD_DIALOG1, this); 
	m_pDlg->Create(134, this);      //第一参数为新建对话框的ID。
	m_pDlg->ShowWindow(SW_SHOW);    // // 显示非模态对话框  

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
//接受函数在此实现
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
		if (pWnd->m_ComDlg.m_Enable == 0)  //一直接收会导致关闭CAN 的程序和接收数据程序冲突
		{}
		else{
			len=VCI_Receive(DEVICE_TYPE , DEVICE_ID, DEVICE_NUM ,frameinfo,50,200);
		}
		if (pWnd->m_ComDlg.m_Enable == 0)
		{
		}
		else if(len<=0)
		{
			//注意：如果没有读到数据则必须调用此函数来读取出当前的错误码，
			//千万不能省略这一步（即使你可能不想知道错误码是什么）
			VCI_ReadErrInfo(DEVICE_TYPE, DEVICE_ID, DEVICE_NUM, &errinfo);
		}                                                                              
		else
		{
#if 1
			dlg->m_recvcount += len;
		//	str.Format("         本次接收 %d 帧  总接收 %d 帧",len,dlg->m_recvcount);
		//	box->InsertString(box->GetCount(),str);
			for(i=0;i<len;i++)
			{
				/*str="接收到CAN帧:  ";
				if(frameinfo[i].TimeFlag==0)
					tmpstr="时间标识:无  ";
				else
					tmpstr.Format("时间标识:%08x ",frameinfo[i].TimeStamp);
				str+=tmpstr;
				tmpstr.Format("帧ID:%08x ",frameinfo[i].ID);
				str+=tmpstr;
				str+="帧格式:";
				if(frameinfo[i].RemoteFlag==0)
					tmpstr="数据帧 ";
				else
					tmpstr="远程帧 ";
				str+=tmpstr;
				str+="帧类型:";
				if(frameinfo[i].ExternFlag==0)
					tmpstr="标准帧 ";
				else
					tmpstr="扩展帧 ";
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
						strTime.Format(L"time：%2d:%2d:%2d",st.wMinute,st.wSecond,st.wMilliseconds) ;
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
						strTime.Format(L"time：%2d:%2d:%2d",st.wMinute,st.wSecond,st.wMilliseconds) ;
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
					str.Format("本次回复 %d 帧  实际回复 %d 帧 总回复 %d 帧 ----------回复不正常",len,ret,dlg->m_testcount);

				}else
				{
					str.Format("本次回复 %d 帧  实际回复 %d 帧 总回复 %d 帧",len,ret,dlg->m_testcount);
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
	strTime.Format(L"time：%2d:%2d:%2d",st.wMinute,st.wSecond,st.wMilliseconds) ;
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

//UDS CAN 接口函数
//接口函数、
uint8 DiagID;
uint16 sendtimes;
uint16 debugtime;
void  L_SendDiagFram(uint8* NetData)
{
	//CWnd *wnd  = AfxGetMainWnd();
	//CTestDlg *pWnd = (CTestDlg*)wnd;
	VCI_CAN_OBJ vco[2];	                                      //调用C***Dialog中的函数xxx();
	
	if (DiagID)
	{
		vco[0].ID = ID_DIAG_FUNCT; // 填写第一帧的ID 
	}
	else
	{
		vco[0].ID = ID_DIAG_PHYSIC; // 填写第一帧的ID 
	}
	vco[0].SendType = 0; // 正常发送 
	vco[0].RemoteFlag = 0; // 数据帧 
	vco[0].ExternFlag = (uint8)ID_FRAMESIZE; // 标准帧 
	vco[0].DataLen = 8; // 数据长度1个字节 

	for (int i =0 ; i< 8; i++)
		vco[0].Data[i] = NetData[i]; // 数据0为0x66
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
		pWnd->ShowInfo(L"写入失败",2);		
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
		str.Format(L"下载失败:0x%x, 0x%x", data[1], data[2]);
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
	// TODO: 在此添加控件通知处理程序代码
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
		GetDlgItem(IDC_BUTTON_SAVE)->SetWindowText(L"提示关");
	}
	else
	{
		m_MessagePoint = 1;
		GetDlgItem(IDC_BUTTON_SAVE)->SetWindowText(L"提示开");
	}

}


//按键
void CCailbration_CANDlg::OnMenuCom()
{
	// TODO: 在此添加命令处理程序代码
	OnToolbar1();
}
//菜单进入bootload
void CCailbration_CANDlg::OnMenuBoot()
{
	// TODO: 在此添加命令处理程序代码
	if(a_OPENBEGIN)  OnBnClickedButtonBegin();  //关闭发送
	Eeprom_Window_Flg=WINDOUS_FLG_BOOT;
	//EeDlg.SetCom(&ComDlg);
	BootDlg.DoModal();
	Eeprom_Window_Flg=WINDOUS_FLG_RAM;
}

void CCailbration_CANDlg::OnLbnSelchangeListInfo()
{
	// TODO: 在此添加控件通知处理程序代码
}

//在 bootdialog 里显示数据
void CCailbration_CANDlg::BDialog_ShowInfo(CString str)
{
	//判断当前界面是不是在bootload 界面

		CWnd* ahWnd = FindWindow( NULL,BOOT_DIALOG1_NAME ) ;	
		if(ahWnd)
		{
			BootDlg.ShowInfo(str, 1);
		}
}



void CCailbration_CANDlg::OnMenueAbout()
{
	// TODO: 在此添加命令处理程序代码
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();

}

void CCailbration_CANDlg::OnMenueEeprom()
{
	// TODO: 在此添加命令处理程序代码
	OnToolbar2();
}


void  CCailbration_CANDlg::SendCanMessage(VCI_CAN_OBJ* vco)
{
	//CWnd *wnd  = AfxGetMainWnd();
	//CTestDlg *pWnd = (CTestDlg*)wnd;
	                                      //调用C***Dialog中的函数xxx();
	
	//vco.ID = ID_DIAG_PHYSIC; // 填写第一帧的ID 
	vco->SendType = 0; // 正常发送 
	vco->RemoteFlag = 0; // 数据帧 
	vco->ExternFlag = sCanCofig.FramSize; // 标准帧 
	vco->DataLen = 8; // 数据长度1个字节 

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
		ShowInfo(L"写入失败",2);		
	}
}




void CCailbration_CANDlg::OnBnClickedButtonOpens19()
{
	// TODO: 在此添加控件通知处理程序代码
		CString strName;
	CString str, m_Result;
	//CFileDialog dlg(FALSE,_T("s19; hex"),_T("*.s19; *.hex"),OFN_EXPLORER,_T("程序文件(.s19)| *.s19; *.hex|所有文件（*.*)|*.*||)"));
	CFileDialog dlg(FALSE,_T("s19; hex"),_T("*.s19; *.hex"),OFN_EXPLORER,_T("程序文件(.s19)| *.s19; *.hex|所有文件（*.*)|*.*||)"));
	if(dlg.DoModal()==IDOK)
	{
		strName=dlg.GetPathName();
		m_S19File.SetWindowTextW(strName);
	}		
	else
	{
		ShowMainInfo(L"文件载入失败", 2);
	}
	UpdateData(FALSE);
}


void CCailbration_CANDlg::OnLbnSelchangeList1()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CCailbration_CANDlg::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}





void CCailbration_CANDlg::OnStnClickedStaticMessage()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CCailbration_CANDlg::OnBnClickedButtonOpens20()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strName;
	CString str, m_Result;

	//CFileDialog dlg(FALSE,_T("s19; hex"),_T("*.s19; *.hex"),OFN_EXPLORER,_T("程序文件(.s19)| *.s19; *.hex|所有文件（*.*)|*.*||)"));
	CFileDialog dlg(FALSE,_T("s19; hex"),_T("*.s19; *.hex"),OFN_EXPLORER,_T("程序文件(.s19)| *.s19; *.hex|所有文件（*.*)|*.*||)"));
	if(dlg.DoModal()==IDOK)
	{
		strName=dlg.GetPathName();
		m_FlashDriveFile.SetWindowTextW(strName);
	}		
	else
	{
		m_FlashDriveFile.SetWindowTextW(L"");
		ShowMainInfo(L"文件载入失败", 2);
	}
	UpdateData(FALSE);
}
