
#include "stdafx.h"
#include "Cailbration_CAN.h"
#include "Cailbration_CANDlg.h"

uint8  FBSessionMode;
uint8 FBScurity;
uint8 TestPresent;
static uint32 u32TotalSize, u32DownSize;
uint8 bFileOK = 0;
uint8 bFlashFileOK = 0;
extern uint8  F_N_USDATA_REQ(uint8* ReqData,uint16 Length,uint8 mId_Target);


volatile uint32 CRC32TABLE[] =  
{  
        0x00000000U,  0x04C11DB7U,  0x09823B6EU,  0x0D4326D9U,  0x130476DCU,  0x17C56B6BU,  0x1A864DB2U,  0x1E475005U,  
        0x2608EDB8U,  0x22C9F00FU,  0x2F8AD6D6U,  0x2B4BCB61U,  0x350C9B64U,  0x31CD86D3U,  0x3C8EA00AU,  0x384FBDBDU,  
        0x4C11DB70U,  0x48D0C6C7U,  0x4593E01EU,  0x4152FDA9U,  0x5F15ADACU,  0x5BD4B01BU,  0x569796C2U,  0x52568B75U,  
        0x6A1936C8U,  0x6ED82B7FU,  0x639B0DA6U,  0x675A1011U,  0x791D4014U,  0x7DDC5DA3U,  0x709F7B7AU,  0x745E66CDU,  
        0x9823B6E0U,  0x9CE2AB57U,  0x91A18D8EU,  0x95609039U,  0x8B27C03CU,  0x8FE6DD8BU,  0x82A5FB52U,  0x8664E6E5U,  
        0xBE2B5B58U,  0xBAEA46EFU,  0xB7A96036U,  0xB3687D81U,  0xAD2F2D84U,  0xA9EE3033U,  0xA4AD16EAU,  0xA06C0B5DU,  
        0xD4326D90U,  0xD0F37027U,  0xDDB056FEU,  0xD9714B49U,  0xC7361B4CU,  0xC3F706FBU,  0xCEB42022U,  0xCA753D95U,  
        0xF23A8028U,  0xF6FB9D9FU,  0xFBB8BB46U,  0xFF79A6F1U,  0xE13EF6F4U,  0xE5FFEB43U,  0xE8BCCD9AU,  0xEC7DD02DU,  
        0x34867077U,  0x30476DC0U,  0x3D044B19U,  0x39C556AEU,  0x278206ABU,  0x23431B1CU,  0x2E003DC5U,  0x2AC12072U,  
        0x128E9DCFU,  0x164F8078U,  0x1B0CA6A1U,  0x1FCDBB16U,  0x018AEB13U,  0x054BF6A4U,  0x0808D07DU,  0x0CC9CDCAU,  
        0x7897AB07U,  0x7C56B6B0U,  0x71159069U,  0x75D48DDEU,  0x6B93DDDBU,  0x6F52C06CU,  0x6211E6B5U,  0x66D0FB02U,  
        0x5E9F46BFU,  0x5A5E5B08U,  0x571D7DD1U,  0x53DC6066U,  0x4D9B3063U,  0x495A2DD4U,  0x44190B0DU,  0x40D816BAU,  
        0xACA5C697U,  0xA864DB20U,  0xA527FDF9U,  0xA1E6E04EU,  0xBFA1B04BU,  0xBB60ADFCU,  0xB6238B25U,  0xB2E29692U,  
        0x8AAD2B2FU,  0x8E6C3698U,  0x832F1041U,  0x87EE0DF6U,  0x99A95DF3U,  0x9D684044U,  0x902B669DU,  0x94EA7B2AU,  
        0xE0B41DE7U,  0xE4750050U,  0xE9362689U,  0xEDF73B3EU,  0xF3B06B3BU,  0xF771768CU,  0xFA325055U,  0xFEF34DE2U,  
        0xC6BCF05FU,  0xC27DEDE8U,  0xCF3ECB31U,  0xCBFFD686U,  0xD5B88683U,  0xD1799B34U,  0xDC3ABDEDU,  0xD8FBA05AU,  
        0x690CE0EEU,  0x6DCDFD59U,  0x608EDB80U,  0x644FC637U,  0x7A089632U,  0x7EC98B85U,  0x738AAD5CU,  0x774BB0EBU,  
        0x4F040D56U,  0x4BC510E1U,  0x46863638U,  0x42472B8FU,  0x5C007B8AU,  0x58C1663DU,  0x558240E4U,  0x51435D53U,  
        0x251D3B9EU,  0x21DC2629U,  0x2C9F00F0U,  0x285E1D47U,  0x36194D42U,  0x32D850F5U,  0x3F9B762CU,  0x3B5A6B9BU,  
        0x0315D626U,  0x07D4CB91U,  0x0A97ED48U,  0x0E56F0FFU,  0x1011A0FAU,  0x14D0BD4DU,  0x19939B94U,  0x1D528623U,  
        0xF12F560EU,  0xF5EE4BB9U,  0xF8AD6D60U,  0xFC6C70D7U,  0xE22B20D2U,  0xE6EA3D65U,  0xEBA91BBCU,  0xEF68060BU,  
        0xD727BBB6U,  0xD3E6A601U,  0xDEA580D8U,  0xDA649D6FU,  0xC423CD6AU,  0xC0E2D0DDU,  0xCDA1F604U,  0xC960EBB3U,  
        0xBD3E8D7EU,  0xB9FF90C9U,  0xB4BCB610U,  0xB07DABA7U,  0xAE3AFBA2U,  0xAAFBE615U,  0xA7B8C0CCU,  0xA379DD7BU,  
        0x9B3660C6U,  0x9FF77D71U,  0x92B45BA8U,  0x9675461FU,  0x8832161AU,  0x8CF30BADU,  0x81B02D74U,  0x857130C3U,  
        0x5D8A9099U,  0x594B8D2EU,  0x5408ABF7U,  0x50C9B640U,  0x4E8EE645U,  0x4A4FFBF2U,  0x470CDD2BU,  0x43CDC09CU,  
        0x7B827D21U,  0x7F436096U,  0x7200464FU,  0x76C15BF8U,  0x68860BFDU,  0x6C47164AU,  0x61043093U,  0x65C52D24U,  
        0x119B4BE9U,  0x155A565EU,  0x18197087U,  0x1CD86D30U,  0x029F3D35U,  0x065E2082U,  0x0B1D065BU,  0x0FDC1BECU,  
        0x3793A651U,  0x3352BBE6U,  0x3E119D3FU,  0x3AD08088U,  0x2497D08DU,  0x2056CD3AU,  0x2D15EBE3U,  0x29D4F654U,  
        0xC5A92679U,  0xC1683BCEU,  0xCC2B1D17U,  0xC8EA00A0U,  0xD6AD50A5U,  0xD26C4D12U,  0xDF2F6BCBU,  0xDBEE767CU,  
        0xE3A1CBC1U,  0xE760D676U,  0xEA23F0AFU,  0xEEE2ED18U,  0xF0A5BD1DU,  0xF464A0AAU,  0xF9278673U,  0xFDE69BC4U,  
        0x89B8FD09U,  0x8D79E0BEU,  0x803AC667U,  0x84FBDBD0U,  0x9ABC8BD5U,  0x9E7D9662U,  0x933EB0BBU,  0x97FFAD0CU,  
        0xAFB010B1U,  0xAB710D06U,  0xA6322BDFU,  0xA2F33668U,  0xBCB4666DU,  0xB8757BDAU,  0xB5365D03U,  0xB1F740B4U
};  


void CCailbration_CANDlg::ReceiveAPPData(A_PDU* APData)
{
	uint8 SubFunction = (APData->A_Data[0]&0x7f);
	uint8 Data[10];
	switch(APData->A_PCI- 0x40)  
	{
		case  SEVER_SID_SESSIONCONTROL:
			if (SubFunction <= 3 )
			{
				FBSessionMode = SubFunction;
			}
			else
			{
			}

			//session control begin
			break;
	/***************session control end********************************/
		case  SEVER_SID_RESET:
		// 复位服务
			if (m_BootLoadState == BT_STATE_RESET)
			{
				BootLoad_DoingNext();
			}
		break;
	/***************reset sever end********************************/  
		case  SEVER_SID_SECURITY:
			if (APData->A_Data[0] == SECURITY_SENDK_LEV2 || APData->A_Data[0] == SECURITY_SENDK_LEV3) 
			{
				FBScurity = 0x2;
			}
			else if (APData->A_Data[0] == SECURITY_SENDK_LEV1) 
			{
				FBScurity = 0x1;
			}
		/*	if (APData->A_Data[0] == SECURITY_REQSD_LEV1 || 
				APData->A_Data[0] == SECURITY_REQSD_LEV2)
			{
               
			nSeed =  APData->A_Data[1];
			nSeed <<= 8;
			nSeed += APData->A_Data[2];
			nSeed <<= 8;
			nSeed += APData->A_Data[3];
			nSeed <<= 8;
			nSeed += APData->A_Data[4];

			
			HMODULE hDll = LoadLibrary(L"SECURITY_SAIC_DLL.dll");
			if (hDll != NULL)
			{
				SetSecurityMask SetMask = (SetSecurityMask)GetProcAddress(hDll, "SetSecurityMask");
				if (SetMask != NULL)
				{
					SetMask(SECURITY_MASK_LEVEL1, SECURITY_MASK_LEVEL2);
				}
				
				seedToKeyFun GetKey = (seedToKeyFun)GetProcAddress(hDll, "seedToKey");
				if (GetKey != NULL)
				{
					if (SecurityState == SECURITY_STATE_LEVEL1)
						nKey=GetKey(nSeed, 1);
					else 
						nKey=GetKey(nSeed, 2);
				}
				FreeLibrary(hDll);
			}
			
			Data[0] = 0x27;
			if(SecurityState == SECURITY_STATE_LEVEL1)
				Data[1] = 0x02;
			else Data[1] = 0x06;
			Data[2] = (uint8)(nKey >> 24);
			Data[3] = (uint8)((nKey >> 16) & 0x00ff);
			Data[4] = (uint8)((nKey & 0x0000ff00) >> 8);
			Data[5] = (uint8) (nKey & 0x000000ff);
			(void)F_N_USDATA_REQ(Data, 6, 0x77); 
			SecurityState =  255;
			}*/
			break;
		/***************security sever end********************************/
			case  SEVER_SID_TP:
			TestPresent = 1;
			//tp begin   test present
			break;
			//tp end
			case  SEVER_SID_CLEARDIAGINF:
			//clear diagnostic info

			break;
			case  SEVER_SID_COMMUNICATIONCONTROL:
			//conmunication control sever

			break;
		//conmunication control sever 
			// write data by info
			case SEVER_SID_WRITEDATABYINF:
			//Deal_WriteDID(&APData.A_Data[0], APData.A_Length);
			break;
			// write data by info   end
			case SEVER_SID_IOCONTROL:
			//  Deal_IOControl();
			break;         
			case SEVER_SID_WRITEBYADDR:
			//Rspeone_NRC(APData.A_PCI,NRC_SNS,APData.A_AI);
			break;
			case SEVER_SID_READDATABYINF:
				if ( ((APData->A_Data[0] <<8) + APData->A_Data[1]) == 0xFDFF)
				{
					if (m_BootLoadState == BT_STATE_START)
					{
						u32OldCheckSum = (APData->A_Data[2] << 24) + (APData->A_Data[3] << 16) + (APData->A_Data[4] << 8) + APData->A_Data[5];
					}
					else if (m_BootLoadState == BT_STATE_READCHECKSUM || m_BootLoadState == BT_STATE_READCHECKSUMWAIT)
					{
						u32NewCheckSum = (APData->A_Data[2] << 24) + (APData->A_Data[3] << 16) + (APData->A_Data[4] << 8) + APData->A_Data[5];
						BootLoad_DoingNext();
					}else{}
				}
				TestPresent = 1;
			break; 
			//IO CONTROL  end
			case  SEVER_SID_DTCCONTROL:
			//dtc control begine

			break;
			//dtc control end
			case  SEVER_SID_READDTC :
			//read   dtc  begin
			//Deal_ReadDTC(&APData.A_Data[0], APData.A_Length);
			break;
			case SEVER_SID_RC:
			//
				if (( (APData->A_Data[1] <<8) + APData->A_Data[2]) == sCanCofig.RCID_ERASE)
				{
					if (m_BootLoadState == BT_STATE_LOADE)
					{
						Deal_TransSever();
					}
				}
				else if (( (APData->A_Data[1] <<8) + APData->A_Data[2]) == sCanCofig.RCID_INTERGRITY)
				{
					if (m_BootLoadState == BT_STATE_INTERGRITY)
					{
						BootLoad_DoingNext();
					}
				}
				else if (( (APData->A_Data[1] <<8) + APData->A_Data[2]) == sCanCofig.RCID_DEPENDENCE)
				{
					if (m_BootLoadState == BT_STATE_DEPENDENCE)
					{
						BootLoad_DoingNext();
					}
				}
				else if (( (APData->A_Data[1] <<8) + APData->A_Data[2]) == sCanCofig.RCID_PRECONDITIONS)
				{
					if (m_BootLoadState == BT_STATE_PRECONDITIONS)
					{
						BootLoad_DoingNext();
					}
				}
				break;
			//read   dtc  end 
			case  0x34:
				SetTransSize( (APData->A_Data[1] <<8) + APData->A_Data[2]);

			case  0x36:
			case  0x37:
			//Data[0] = APData.A_PCI - 0x40;
			//ApplayTranslate(Data);
			Deal_TransSever();
			//  Send_Uart_Fram(Data, 2); 
			break;
 
			case 0x3f:
			//否定响应吗
			Data[0] = 0x7f;
			Data[1] = APData->A_Data[0];
			Data[2] = APData->A_Data[1];
			TestPresent = 1;
			DiagRecNRC(Data);
			//Send_Uart_Fram(Data, 3); 
			break;
		}

}

void CCailbration_CANDlg::DiagRecNRC(uint8* aData)
{
	if (aData[2] != 0x78)
	{
		//等待
		if (aData[1] == 0x34 || aData[1] == 0x36 || aData[1] == 0x37)
		{
			if (m_BootLoadState == BT_STATE_LOADE)
			{
				m_LoadTask = LOAD_TASK_FAILED;
				//m_BootLoadState =
			}
		}
		else if (aData[1] == 0x31)
		{
			if (m_BootLoadState == BT_STATE_LOADE)
			{
				m_LoadTask = LOAD_TASK_FAILED;
			}
		}
	}
	else
	{
		/*if (aData[1] == 0x34 || aData[1] == 0x36 || aData[1] == 0x37)
		{
			if (m_BootLoadState == BT_STATE_LOADE)
			{
				m_LoadTask == LOAD_TASK_FAILED;
				//m_BootLoadState = 
			}
		}*/
	}
}


void CCailbration_CANDlg::BootLoad_ONTIMER(uint8 nTimer)
{
	uint8 SendByte[20];
	switch(m_BootLoadState)
	{
	default:
	case BT_STATE_SUCCESS:
	case BT_STATE_END:
		m_LogFile.EndFile();
		KillTimer(nTimer);
		break;
	case BT_STATE_START:
		//m_LoadProgress.SetRange(0,100);
	//	m_LoadProgress.SetStep(0);

		//检查文件
		if (bFileOK == 1)
		{
			if (TestPresent)
			{
				SendByte[0] = 0x10;
				SendByte[1] = 3;   
				SendByte[2] = 2;
				F_N_USDATA_REQ(SendByte, 2,ID_DEFINE_TARGET);
				ShowMainInfo(L"CAN 连接成功下载",2);
				//ShowMainInfo(L"尝试进入BT模式....",2);
				m_BootLoadState = BT_STATE_EXTERNMODE;
				FBSessionMode = 0;
				SetTimer(nTimer, 500, NULL);
			}
			else
			{
				ShowMainInfo(L"CAN 连接失败， 结束下载",2);
				m_BootLoadState = BT_STATE_END;
			}
		}
		else
		{
			m_BootLoadState = BT_STATE_END;
			ShowMainInfo(L"文件载入错误",2);
		}
		break;
	case BT_STATE_EXTERNMODE:
		SendByte[0] = 0x31;
		SendByte[1] = 01;   
		SendByte[2] = (unsigned char) (sCanCofig.RCID_PRECONDITIONS >>8);
		SendByte[3] = (unsigned char) (sCanCofig.RCID_PRECONDITIONS & 0x00ff);
		F_N_USDATA_REQ(SendByte, 4,ID_DEFINE_TARGET);
		ShowMainInfo(L"预编译。。。。",2);
		//ShowMainInfo(L"尝试进入BT模式....",2);
		m_BootLoadState =BT_STATE_PRECONDITIONS ;
		SetTimer(nTimer, 500, NULL);
		break;
	case BT_STATE_PRECONDITIONS:
		SendByte[0] = 0x10;
		SendByte[1] = 2;   
		SendByte[2] = 2;
		F_N_USDATA_REQ(SendByte, 2,ID_DEFINE_TARGET);
		ShowMainInfo(L"尝试进入BT模式。。。",2);
		//ShowMainInfo(L"尝试进入BT模式....",2);
		m_BootLoadState = BT_STATE_PM;
		FBSessionMode = 0;
		SetTimer(nTimer, 500, NULL);
		break;
	case BT_STATE_PM:
		if (FBSessionMode == 2)
		{
			SendByte[0] = 0x27;
			SendByte[1] = SECURITY_REQSD_LEV2;   
			SendByte[2] = 2;
			m_BootLoadState = BT_STATE_SCURITY;
			FBScurity = 0;
			SetTimer(nTimer, 500, NULL);
			F_N_USDATA_REQ(SendByte, 2,ID_DEFINE_TARGET);
			ShowMainInfo(L"进入BT模式成功",2);
			//ShowMainInfo(L"尝试进行解锁。。。",2);
		}
		else
		{
			ShowMainInfo(L"回话模式无法进入BT 模式， 结束下载",2);
			m_BootLoadState = BT_STATE_END;
		}
		break;
	case BT_STATE_SCURITY:
		if (FBScurity == 2)
		{
			m_BootLoadState = BT_STATE_LOADE;
			m_LoadTask = LOAD_TASK_NEXTREGION;
			nRegion = 0;
			nFLashRegion = 0;
			Deal_TransSever();
			SetTimer(nTimer, 1100, NULL);
			ShowMainInfo(L"解锁成功",2);
			ShowMainInfo(L"DOWNLOAD。。。。",2);
		}
		else
		{
			ShowMainInfo(L"解锁失败， 结束下载",2);
			m_BootLoadState = BT_STATE_END;
		}
		break;
	case BT_STATE_LOADE:
		if (m_LoadTask == LOAD_TASK_NEXTREGION)
		{

		}
		else if (m_LoadTask == LOAD_TASK_SUCCESS)
		{
			m_BootLoadState = BT_STATE_INTERGRITY;
			ShowMainInfo(L"传输完成，校验中。。。",2);
			SendByte[0] = 0x31;
			SendByte[1] = 0x01;
			SendByte[2] = (unsigned char) (sCanCofig.RCID_INTERGRITY >>8);
			SendByte[3] = (unsigned char) (sCanCofig.RCID_INTERGRITY & 0x00ff);
			F_N_USDATA_REQ(SendByte, 4,ID_DEFINE_TARGET);
			SetTimer(nTimer, 5000, NULL);
		}
		else if (m_LoadTask == LOAD_TASK_FAILED)
		{
			ShowMainInfo(L"传输失败， 结束下载",2);
			m_BootLoadState = BT_STATE_END;
			SetTimer(nTimer, 1000, NULL);
		}
		else
		{
			SetTimer(nTimer, 1000, NULL);
		}
		
		//UpdateData(FALSE);
		break;
	case BT_STATE_INTERGRITY:
	case BT_STATE_DEPENDENCE:
		ShowMainInfo(L"校验超时， 结束下载",2);
		m_BootLoadState = BT_STATE_END;
		SetTimer(nTimer, 1000, NULL);
		break;
	case BT_STATE_RESET:
		ShowMainInfo(L"复位超时， 结束下载",2);
		m_BootLoadState = BT_STATE_END;
		SetTimer(nTimer, 1000, NULL);
		break;
	case BT_STATE_READCHECKSUM:
		u32NewCheckSum = 0;
		SendByte[0] = 0x22;
		SendByte[1] = (uint8)(ROUTINE_ID_CHECKSUM >>8);   
		SendByte[2] = (uint8)(ROUTINE_ID_CHECKSUM & 0xff);
		F_N_USDATA_REQ(SendByte, 3,ID_DEFINE_TARGET);

		m_BootLoadState = BT_STATE_READCHECKSUMWAIT;
		SetTimer(nTimer, 500, NULL);
		break;
	case BT_STATE_READCHECKSUMWAIT:
		ShowMainInfo(L"读取校验码超时， 结束下载",2);
		m_BootLoadState = BT_STATE_SUCCESS;
		break;
	}
}


void CCailbration_CANDlg::BootLoad_DoingNext(void)
{
	uint8 SendByte[20];
	switch(m_BootLoadState)
	{
	default:
		break;
	case BT_STATE_PRECONDITIONS:
		ShowMainInfo(L"预编译成功",2);
		break;
	case BT_STATE_INTERGRITY:
		ShowMainInfo(L"校验-完整性成功，开始校验-依赖性。。。",2);
		m_BootLoadState = BT_STATE_DEPENDENCE;
		SendByte[0] = 0x31;
		SendByte[1] = 0x01;
		SendByte[2] = (unsigned char) (sCanCofig.RCID_DEPENDENCE >>8);
		SendByte[3] = (unsigned char) (sCanCofig.RCID_DEPENDENCE & 0x00ff);
		F_N_USDATA_REQ(SendByte, 4,ID_DEFINE_TARGET);
		break;
	case  BT_STATE_DEPENDENCE:
	    ShowMainInfo(L"校验-依赖性成功，开始复位。。。",2);
		m_BootLoadState = BT_STATE_RESET;
		SendByte[0] = 0x11;
		SendByte[1] = 0x01;
		F_N_USDATA_REQ(SendByte, 2, ID_DEFINE_TARGET);
		break;
	case BT_STATE_RESET:
		ShowMainInfo(L"复位成功，下载完成！！！！",2);
		m_LoadProgress.SetPos(100);
		m_BootLoadState = BT_STATE_READCHECKSUM;

		KillTimer(ID_TIMER_COM);
		SetTimer(ID_TIMER_COM,2000,NULL);

		break;
	case BT_STATE_READCHECKSUMWAIT:
		m_BootLoadState = BT_STATE_SUCCESS;
		CString str;
		str.Format(L"原校验码 = 0x%x, 新校验码 = 0x%x",u32OldCheckSum, u32NewCheckSum);
		ShowMainInfo(str,2);
		u32NewCheckSum = 0;
		break;
	}
}


#define  UDSNETLAY_MAXSIZE  252
void CCailbration_CANDlg::Deal_TransSever(void)
{
	CString str;
	CString Addr;
	int nSendLen;
	unsigned char SendData[UDSNETLAY_MAXSIZE+5];
	static int bFlashDrive;
	static HEXREGION*  pDriveHex;

	if (m_LoadTask == LOAD_TASK_NEXTREGION)
	{
		if (nFLashRegion < sFlashDrive.i16RegionNum)
		{
			bFlashDrive = 1;
			pDriveHex = &sFlashDrive.sRegionMsg[nFLashRegion];
			nFLashRegion++;
			m_LoadTask = LOAD_TASK_REQDOWNL;
			str.Format(L"下载flashDrvie的第 %d 区域", nFLashRegion);
			m_LogFile.WriteFile(str);
			ShowInfo(str,2);
			if (nFLashRegion == 0)
			{
				u32DownSize = 0;
			}
			Deal_TransSever();
			str = L"";
		}
		else if (nRegion < sCodeData.i16RegionNum)
		{
			bFlashDrive = 0;
			pDriveHex = &sCodeData.sRegionMsg[nRegion];
			if (nRegion == 0)
			{
				u32DownSize = 0;
			}
			nRegion++;
			m_LoadTask = LOAD_TASK_ERASE;
			Deal_TransSever();
		}
		else
		{
			m_LoadTask = LOAD_TASK_SUCCESS;
		}
	}
	else if (m_LoadTask == LOAD_TASK_ERASE)
	{
		SendData[0] = 0x31;
		SendData[1] = 0x1;
		SendData[2] = (unsigned char) (sCanCofig.RCID_ERASE >>8);
		SendData[3] = (unsigned char) (sCanCofig.RCID_ERASE & 0x00ff);
		SendData[4] = 0x44;
		SendData[5] = (uint8)(pDriveHex->BeginAddr >>24);
		SendData[6] = (uint8)((pDriveHex->BeginAddr >>16) & 0x00ff) ;
		SendData[7] = (uint8)((pDriveHex->BeginAddr >>8) & 0x0000ff);
		SendData[8] = (uint8)(pDriveHex->BeginAddr &0x000000ff);

		SendData[9] = (uint8)(pDriveHex->HexDataLen >>24);
		SendData[10] = (uint8)((pDriveHex->HexDataLen >>16) & 0x00ff) ;
		SendData[11] = (uint8)((pDriveHex->HexDataLen >>8) & 0x0000ff);
		SendData[12] = (uint8)(pDriveHex->HexDataLen &0x000000ff);

		F_N_USDATA_REQ(SendData,13, ID_DEFINE_TARGET);   //jac_xx

		m_LoadTask = LOAD_TASK_REQDOWNL;

		str.Format(L"擦除 起始地址： 0x%x, 长度 %d", pDriveHex->BeginAddr, pDriveHex->HexDataLen);
		ShowMainInfo(str, 2);
	}
	else if (m_LoadTask == LOAD_TASK_REQDOWNL)
	{
		SendData[0] = 0x34;
		SendData[1] = 0x00;
		SendData[2] = 0x44;

		SendData[3] = (uint8)(pDriveHex->BeginAddr >> 24);
		SendData[4] = (uint8)((pDriveHex->BeginAddr & 0x00ff0000) >> 16);
		SendData[5] = (uint8)((pDriveHex->BeginAddr & 0x0000ff00) >> 8);
		SendData[6] = (uint8) (pDriveHex->BeginAddr & 0x000000ff);

	
		SendData[7] = (uint8)(pDriveHex->HexDataLen >> 24);
		SendData[8] = (uint8)((pDriveHex->HexDataLen & 0x00ff0000) >> 16);
		SendData[9] = (uint8)((pDriveHex->HexDataLen & 0x0000ff00) >> 8);
		SendData[10] = (uint8) (pDriveHex->HexDataLen & 0x000000ff);	
		str.Format(L"\r\n开始传输 传输地址： 0x%X ， 总共有 %d 个数据",pDriveHex->BeginAddr, pDriveHex->HexDataLen);



		F_N_USDATA_REQ(SendData, 11,ID_DEFINE_TARGET);
		m_BLSendLen = 1;
		m_BLSendNum = 0;
		m_LoadTask = LOAD_TASK_TRANSDATA;
	}
	else if (m_LoadTask == LOAD_TASK_TRANSDATA)
	{
		//m_LoadProgress.SetStep(u32DownSize*100 / u32TotalSize);
		//UpdateData(FALSE

		if (i16TransSize > UDSNETLAY_MAXSIZE)
		{
			i16TransSize = UDSNETLAY_MAXSIZE;
		}
		else
		{
		//	i16TransSize = i16TransSize;
		}
	//	m_LoadProgress.SetPos(u32DownSize*100 / u32TotalSize);
		m_LoadProgress.SetPos(m_BLSendNum*100 / pDriveHex->HexDataLen);
		str = L"\r\n握手成功";

		Addr.Format(L"第 %d 次,  传输了： %d个数 ",m_BLSendLen, m_BLSendNum);
		str += Addr;

		SendData[0] = 0x36;
		SendData[1] = m_BLSendLen % 256;//块号

		//loop send 
		if (m_BLSendNum >= pDriveHex->HexDataLen) 
		{
			m_LoadTask = LOAD_TASK_EXITTRANS;
			return;
		}
		if ( (m_BLSendNum + i16TransSize) < pDriveHex->HexDataLen)
		{
			nSendLen = i16TransSize;
		}
		else
		{//发送完成
			nSendLen = pDriveHex->HexDataLen - m_BLSendNum;
			m_LoadTask = LOAD_TASK_EXITTRANS;
		}
		memcpy(&SendData[2], &pDriveHex->pData[m_BLSendNum], nSendLen);

		F_N_USDATA_REQ(SendData, nSendLen+2,ID_DEFINE_TARGET);
		m_BLSendLen++;
		m_BLSendNum += nSendLen; 
		u32DownSize += nSendLen;
	}
	else if (m_LoadTask == LOAD_TASK_EXITTRANS)
	{
		SendData[0] = 0x37;
		SendData[1] = 0x01;
		F_N_USDATA_REQ(SendData, 2,ID_DEFINE_TARGET);  ////jac_xx
		m_LoadTask = LOAD_TASK_REQDOWNL;
		//m_LoadProgress.SetPos(100);
		if (bFlashDrive)
		{
			str.Format(L"flashdrive下载结束");
		}
		else
		{
			str.Format(L"第%d区域下载结束", nRegion);
		}

		m_LoadTask = LOAD_TASK_NEXTREGION;
	}
	else
	{

	}

	m_LogFile.WriteFile(str);
	ShowInfo(str,2);
	/*
	if (m_LoadTask == LOAD_TASK_TRANSDATA)
	{
		//m_LoadProgress.SetStep(u32DownSize*100 / u32TotalSize);
		//UpdateData(FALSE
		if (bFlashFileOK == 1 && nFLashRegion == 0)
		{
			str = L"\r\n开始传输flashdrive-36";

			SendData[0] = 0x36;
			SendData[1] = m_BLSendLen % 256;//块号

			//loop send 
			if (m_BLSendNum >= sFlashDrive.TotalLength) 
			{
				m_LoadTask = LOAD_TASK_EXITTRANS;
				return;
			}
			if ( (m_BLSendNum + i16TransSize) < sFlashDrive.TotalLength)
			{
				nSendLen = i16TransSize;
			}
			else
			{//发送完成
				nSendLen = sFlashDrive.TotalLength - m_BLSendNum;
				m_LoadTask = LOAD_TASK_EXITTRANS;
			}
			memcpy(&SendData[2], &sFlashDrive.HexData[m_BLSendNum], nSendLen);

		
			F_N_USDATA_REQ(SendData, nSendLen+2,ID_DEFINE_TARGET);

			m_BLSendLen++;
			m_BLSendNum += nSendLen; 
			//u32DownSize += nSendLen;
		}
		else
		{
			if (i16TransSize > UDSNETLAY_MAXSIZE)
			{
				i16TransSize = UDSNETLAY_MAXSIZE;
			}
			else
			{
			//	i16TransSize = i16TransSize;
			}
			m_LoadProgress.SetPos(u32DownSize*100 / u32TotalSize);
	
			str = L"\r\n握手成功";

			
			Addr.Format(L"第 %d 次,  传输了： %d个数 ",m_BLSendLen, m_BLSendNum);
			str += Addr;

			SendData[0] = 0x36;
			SendData[1] = m_BLSendLen % 256;//块号

			//loop send 
			if (m_BLSendNum >= sHexData[nRegion].HexDataLen) 
			{
				m_LoadTask = LOAD_TASK_EXITTRANS;
				return;
			}
			if ( (m_BLSendNum + i16TransSize) < sHexData[nRegion].HexDataLen)
			{
				nSendLen = i16TransSize;
			}
			else
			{//发送完成
				nSendLen = sHexData[nRegion].HexDataLen - m_BLSendNum;
				m_LoadTask = LOAD_TASK_EXITTRANS;
			}
			memcpy(&SendData[2], &sHexData[nRegion].HexData[m_BLSendNum], nSendLen);

		

			F_N_USDATA_REQ(SendData, nSendLen+2,ID_DEFINE_TARGET);

			m_BLSendLen++;
			m_BLSendNum += nSendLen; 
			u32DownSize += nSendLen;
		}
	}
	else if (m_LoadTask == LOAD_TASK_EXITTRANS)
	{
		SendData[0] = 0x37;
		SendData[1] = 0x01;
		F_N_USDATA_REQ(SendData, 2,ID_DEFINE_TARGET);  ////jac_xx
		m_LoadTask = LOAD_TASK_REQDOWNL;
		
		if (bFlashFileOK == 1 && nFLashRegion == 0)
		{
			nFLashRegion = 1;
			str.Format(L"flashdrive下载结束");
			//m_LoadTask = LOAD_TASK_SUCCESS;
		}
		else
		{
			nRegion++;
			if (nRegion >=m_nRegion)
			{
				m_LoadTask = LOAD_TASK_SUCCESS;
			} 
			str.Format(L"第%d区域下载结束", nRegion);
		}
	}
	else if (m_LoadTask == LOAD_TASK_REQDOWNL)
	{
		SendData[0] = 0x34;
		SendData[1] = 0x00;
		SendData[2] = 0x44;

		if (bFlashFileOK == 1 && nFLashRegion == 0)
		{
			SendData[3] = (uint8)(sFlashDrive[0].BeginAddr >> 24);
			SendData[4] = (uint8)((sFlashDrive[0].BeginAddr & 0x00ff0000) >> 16);
			SendData[5] = (uint8)((sFlashDrive[0].BeginAddr & 0x0000ff00) >> 8);
			SendData[6] = (uint8) (sFlashDrive[0].BeginAddr & 0x000000ff);

	
			SendData[7] = (uint8)(sFlashDrive[0].HexDataLen >> 24);
			SendData[8] = (uint8)((sFlashDrive[0].HexDataLen & 0x00ff0000) >> 16);
			SendData[9] = (uint8)((sFlashDrive[0].HexDataLen & 0x0000ff00) >> 8);
			SendData[10] = (uint8) (sFlashDrive[0].HexDataLen & 0x000000ff);	
			str.Format(L"\r\n开始传输flashdrive 传输地址： 0x%X ， 总共有 %d 个数据",sFlashDrive[0].BeginAddr, sFlashDrive[0].HexDataLen);
		}
		else
		{
			SendData[3] = (uint8)(sHexData[nRegion].BeginAddr >> 24);
			SendData[4] = (uint8)((sHexData[nRegion].BeginAddr & 0x00ff0000) >> 16);
			SendData[5] = (uint8)((sHexData[nRegion].BeginAddr & 0x0000ff00) >> 8);
			SendData[6] = (uint8) (sHexData[nRegion].BeginAddr & 0x000000ff);

	
			SendData[7] = (uint8)(sHexData[nRegion].HexDataLen >> 24);
			SendData[8] = (uint8)((sHexData[nRegion].HexDataLen & 0x00ff0000) >> 16);
			SendData[9] = (uint8)((sHexData[nRegion].HexDataLen & 0x0000ff00) >> 8);
			SendData[10] = (uint8) (sHexData[nRegion].HexDataLen & 0x000000ff);	
			str.Format(L"开始传输地址： 0x%X， 总共有 %d 个数据", sHexData[nRegion].BeginAddr, sHexData[nRegion].HexDataLen);

			if (nRegion == 0)
			{
				u32DownSize = 0;
			}
		}

		F_N_USDATA_REQ(SendData, 11,ID_DEFINE_TARGET);
		m_BLSendLen = 1;
		m_BLSendNum = 0;
		m_LoadTask = LOAD_TASK_TRANSDATA;
	}
	else if (m_LoadTask == LOAD_TASK_ERASE)
	{

	}
	m_LogFile.WriteFile(str);
	ShowInfo(str,2);
	*/
}


void CCailbration_CANDlg::SetTransSize(int size)
{
	i16TransSize = size & 0xfffffc;
}



int JudgeS19(CString filename);
int ReadS19(unsigned char (*p)[100], CString filename);
int ReadS19TOData(HEXDATA_ALL *sHexData , CString filename);
void CCailbration_CANDlg::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	CString strFile = L"";  
	CString  m_Result, str, m_FileName;
	m_S19File.GetWindowTextW(strFile);

	if(JudgeS19(strFile))
	{
		bFileOK = 1;
		ShowMainInfo(L"文件载入成功", 2);
		m_FileName = strFile;
		//m_HexDataLen = ReadS19(m_HexData, m_FileName);
		m_nRegion = ReadS19TOData(&sCodeData, m_FileName);
		
		if (m_nRegion <= HEX_MAX_REGION && m_nRegion != 0)
		{
			m_Result.Format(L"共 %d 个区域 \r\n", m_nRegion);
			ShowMainInfo(m_Result, 2);

			for (int i = 0; i< m_nRegion; i++)
			{
				str.Format(L"第%d区域： 起始地址：0X%x， 长度 %d \r\n",i+1, sCodeData.sRegionMsg[i].BeginAddr, sCodeData.sRegionMsg[i].HexDataLen);
				ShowMainInfo(str, 2);
				str.Format(L"CRC为: 0x%X", sCodeData.sRegionMsg[i].regionchecksum);
				ShowMainInfo(str, 2);
				//m_Result += str;
			}
			m_Result = _T("");
			//计算checksum
			int iStartAddr, iEndAddr;

			//iStartAddr = m_StartAddr.GetInt();
			//iEndAddr  = m_EndAddr.GetInt();
			iStartAddr = CODE_START_ADDR;
			iEndAddr = CODE_END_ADDR;

			int HexStartAddr, HexEndAddr;
			HexStartAddr = sCodeData.sRegionMsg[0].BeginAddr;
			HexEndAddr = sCodeData.sRegionMsg[m_nRegion-1].BeginAddr + sCodeData.sRegionMsg[m_nRegion-1].HexDataLen - 1;
			if (iStartAddr >= iEndAddr     //地址错误
				|| iStartAddr != HexStartAddr || HexEndAddr > iEndAddr) // 起始地址一定要相等,结尾地址要比所求的宽
			//if (0)
			{
				m_Result += (L"地址无效，按照HEX数据求checksum为：\r\n.");
				m_FlashChkSum = 0;
				u32TotalSize = 0;
				for (int i = 0; i< m_nRegion; i++)
				{
					for (int j = 0; j < sCodeData.sRegionMsg[i].HexDataLen; j++)
					{
						m_FlashChkSum += sCodeData.sRegionMsg[i].pData[j];
					}
					u32TotalSize += sCodeData.sRegionMsg[i].HexDataLen;
				}
			}
			else
			{
				//计算少个有多区域未烧写，将此区域补未0xff
				m_Result += (L"地址有效，空地址为0xff计算求checksum为：\r\n.");
				int HexTotalLen = 0;
				for (int i = 0; i< m_nRegion; i++)
				{
					HexTotalLen += sCodeData.sRegionMsg[i].HexDataLen;
				} 
				m_FlashChkSum = 0;
				u32TotalSize =0;
				for (int i = 0; i< m_nRegion; i++)
				{
					for (int j = 0; j < sCodeData.sRegionMsg[i].HexDataLen; j++)
					{
						m_FlashChkSum += sCodeData.sRegionMsg[i].pData[j]; 
					}	
					u32TotalSize += sCodeData.sRegionMsg[i].HexDataLen;
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
			bFileOK = 0;
			if (m_nRegion ==0)
			{
				m_Result.Format(L"数据格式错误");
			}
			else
			{
				m_Result.Format(L"范围错误");
			}
		
		}

		
		//m_Result += str;
		ShowMainInfo(m_Result, 2);
	}
}

//ReadFlashS19TOData
void CCailbration_CANDlg::OnEnChangeEdit13()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString strFile = L"";  
	CString  m_Result, str, m_FileName;
	m_FlashDriveFile.GetWindowTextW(strFile);
	unsigned int checksum;
	
	bFlashFileOK = 0;
	
	if(JudgeS19(strFile))
	{
		ShowMainInfo(L"文件载入成功", 2);
		m_FileName = strFile;
		//m_HexDataLen = ReadS19(m_HexData, m_FileName);
		int  i16Region = ReadS19TOData(&sFlashDrive, m_FileName);
		
		if (i16Region <= 1 && i16Region != 0)
		{
			m_Result.Format(L"flash drvie 共 %d 个区域 \r\n", i16Region);
			ShowMainInfo(m_Result, 2);

		
			//计算checksum
			int HexStartAddr, HexEndAddr;
			HexStartAddr = sFlashDrive.sRegionMsg[0].BeginAddr;
			HexEndAddr = sFlashDrive.sRegionMsg[0].BeginAddr + sFlashDrive.sRegionMsg[0].HexDataLen -1;
			
			if (HexStartAddr == 0x1BFFC && HexEndAddr == 0x1BFFF) // 起始地址一定要相等,结尾地址要比所求的宽
			{
				bFlashFileOK = 1;
				checksum = sFlashDrive.sRegionMsg[0].pData[0] + (sFlashDrive.sRegionMsg[0].pData[1]<<8) + (sFlashDrive.sRegionMsg[0].pData[2]<<16) + (sFlashDrive.sRegionMsg[0].pData[3]<<24);
				m_Result.Format(L"FLASH DRIVE 正确，checksum为：0x%x\r\n.", checksum);
			}
			else
			{
				//计算少个有多区域未烧写，将此区域补未0xff
				m_Result += (L"地址错误：\r\n.");
			}
		}
		else
		{
			bFlashFileOK = 0;
			sFlashDrive.i16RegionNum = 0;
			if (i16Region ==0)
			{
				m_Result.Format(L"数据格式错误");
			}
			else
			{
				m_Result.Format(L"范围错误");
			}
		}

		ShowMainInfo(m_Result, 2);
	}
	else
	{
		bFlashFileOK = 0;
		sFlashDrive.i16RegionNum = 0;
		m_Result.Format(L"文件格式不对");
		ShowMainInfo(m_Result, 2);
	}
}