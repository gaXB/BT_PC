#include "stdafx.h"
#include "NetLaye.h"
#include "timerout.h"
#include "APPlayLay.h"
//#include "WinBase.h"

#define  _SK81_SCURITY_  1
//#include "testDlg.h"
//extern  CTestDlg *pWnd;
extern CString str_pathDll;
uint8  SessionMode = SESSION_MODE_EXTERN;    //会话模式

uint8  SecurityState = SECURITY_STATE_SSEED1;   //安全访问模式
uint8  scurityType = 0;

uint32  SECURITY_MASK_LEVEL1  = 0x5c735C73;
uint32  SECURITY_MASK_LEVEL2  = 0x5F735F73;

static   uint8  SecurityErrTimes ;    //访问错误次数
static   uint16 TimerSecurity;        //定时参数

//uint8 A_Data[8];
A_PDU APData;
static   uint16 TimerP2Can,TimerP3Sever;
static   uint8 AppState;           //应用层状态
static   uint8 P3SeverStop;
static   uint8 UDS_ResetState;  //复位状态， 1 需要复位， 当一帧命令发送完成后进行复位，在 F_N_USData_CON 函数中实现

//发送状态定义
//CCSTATE
static union
{
   struct
   {
      unsigned st_ncm   :1;      //1 禁止发送
      unsigned st_nwmcm :1;
   }Bits;
   uint8  Byte;
}CCState;

//storing state  1: not stor
static   uint8 StorState;  
//uint16 pDATAT ;

static uint32  nSeed, nKey;

//pid tbl_char
//const

 
#define  SOFT_VER_H  _SwsV
#define  SOFT_VER_M  _HwV
#define  SOFT_VER_L  0x00

const uint8 tbl_ac[]="AC";
#define  HARD_VER_H  0X03
#define  HARD_VER_L  0X01
#define  HARD_VER_M  0X00


//需要外部调用的函数
extern uint8 F_N_USDATA_REQ(uint8* ReqData,uint16 Length,uint8 mId_Target)  ;
extern void  Respone_POS_2(uint8 SID,uint8 FSID,uint8 Target_Id);
extern void Deal_ReadDID(uint8 *Data, uint8 Length);
extern void Deal_WriteDID(uint8 *Data, uint8 Length);
extern void Deal_ReadDTC(uint8 *Data, uint16 Length);
extern void Deal_IOControl(void);
extern void Deal_ReadAddr(void);
extern void  Save_DTC(void);
void ApplayTranslate(uint8 *data);
void SetSecurityType(uint8 id);
void Send_Uart_Fram(uint8 *tbl_tx,uint8 num);
void CANAPP_TestPresent(void);
extern void Receive_AppData(A_PDU *pAPdu);
/**************************************************************
 回应一个否定响应  
 void  Respone_NRC(uint8 SID,uint8 nrc_type,uint8 Target_Id)
 应用层和uds服务都可以应用                           
*********************************************************************/
void  Respone_NRC(uint8 SID,uint8 nrc_type,uint8 Target_Id)
{
   uint8 A_Data[8];
   if(Target_Id==AID_PHYSIC) 
   {  //phsic nrc respone
      A_Data[0]=0x7f ;    //pci 0
      A_Data[1]= SID;        //pci 1
      A_Data[2]= nrc_type;
   #ifdef _DEBUG_   
      if(Test_Ser)
      {
         SysErcode.Bits.b2=1;
      }
   #endif
      (void)F_N_USDATA_REQ(A_Data,3,Target_Id)  ;
      AppState=APP_STATE_RESPONE;
          
   }
   else
   {  //funct nrc not resp

   }
}


/**************************************************************
回应一个肯定响应 ,2个数据，1个SID，1个subId
                                
*********************************************************************/
void  Respone_POS_2(uint8 SID,uint8 FSID,uint8 Target_Id)
{
   uint8 A_Data[8];
   if(FSID&0x80)
   {//sprmib==1
      return;
   }
   else
   {
      //APData_Respone.A_AI=A_Id;
      A_Data[0]=SID+0x40 ;    //pci
      A_Data[1]= FSID&0x7f;


      (void)F_N_USDATA_REQ(A_Data,2,Target_Id) ;
      AppState=APP_STATE_RESPONE;
   } 
}

/**************************************************************
sessionControl,回应一个肯定响应
                                
*********************************************************************/
void  Respone_POS_SessionC(uint8 SID,uint8 subSID,uint8 Target_Id)
{
   uint8 A_Data[8];
   if(subSID&0x80)
   {//sprmib==1  ,sessioncontrol must need posrespone,so this is no used
      return;
   }
   else
   {
      //APData_Respone.A_AI=A_Id;
      A_Data[0]=SID+0x40 ;    //pci
      A_Data[1]= subSID&0x7f;
      A_Data[2]= P2CAN_Sever>>8;
      A_Data[3]= (P2CAN_Sever&0x00ff);
      A_Data[4]= P2CAN_Sever1>>8;
      A_Data[5]= (P2CAN_Sever1&0x00ff);
     
      (void)F_N_USDATA_REQ(A_Data,6,Target_Id)  ;
      AppState=APP_STATE_RESPONE ;
   }
}

/**************************************************************
sessionControl,回应一个肯定响应
                                
*********************************************************************/

uint32 seedToKey1 (void)
{
   	
	uint32 key = 0;
	uint32  MASK;  
	uint8 i; 
	if (SecurityState == SECURITY_STATE_LEVEL1)
	{
	   MASK =   SECURITY_MASK_LEVEL1;
	}
	else      MASK =   SECURITY_MASK_LEVEL2;
	if (nSeed !=0 )
	{
		for ( i=0; i<35; i++ )
		{
			if (nSeed & 0x80000000)
			{
				nSeed = nSeed <<1;
				nSeed = nSeed^MASK;
			}
			else
			{
				nSeed = nSeed << 1;
			}
		}
		key = nSeed;
	}
	return key;
}



uint8 ROR3_Fun(uint8 data)
{
	uint8 i;
	
	for (i = 0; i < 3; i++)
	{
		if (data & 0x01)
		{
			data >>= 1;
			data |= 0x80;
		}
		else
		{
			data >>= 1;
		}
	}
	
	return data;
}

uint32 seedToKey (void)
{
   	
	uint8 u8ConStant[4] =
	{
			0x11, 0x22, 0x33, 0x44
	};
	
	uint32 u32V0 = nSeed;
	uint32 u32V1 = 0xffffffff - u32V0;
	uint32 key = 0;
	uint8 i;
	uint32 k[4];
	uint32 sum = 0;
	uint32 delta = 0x9e3779b9;
	
	if (SecurityState == SECURITY_STATE_LEVEL2)
	{
		u8ConStant[0] = ROR3_Fun(u8ConStant[0]);
		u8ConStant[1] = ROR3_Fun(u8ConStant[1]);
		u8ConStant[2] = ROR3_Fun(u8ConStant[2]);
		u8ConStant[3] = ROR3_Fun(u8ConStant[3]);
	}
	
	k[0] = u8ConStant[0];
	k[1] = u8ConStant[1];
	k[2] = u8ConStant[2];
	k[3] = u8ConStant[3];
	
	for (i =0; i< 2; i++)
	{
		u32V0 +=  (((u32V1 << 4) ^ (u32V1 >> 5)) + u32V1) ^ (sum + k[sum & 3]);
		sum += delta;
		u32V1 += (((u32V0 << 4) ^ (u32V0 >> 5)) + u32V0) ^ (sum + k[(sum>>11) & 3]);
	}

	key = u32V0;

	return key;
}



/*******************************************************************************
 * Function:      void  F_N_USData_IND(N_USDATA_IND*  NetInd)
 *
 * Description:    网络层收到 1个请求
 *
 * Returns:         none
 *
 * Notes:          收到服务并进行处理
********************************************************************************/
typedef uint32 (*seedToKeyFun) (uint32, uint8);
typedef uint32 (*SetSecurityMask) (uint32, uint32);
void  F_N_USData_IND(N_USDATA_IND*  NetInd)
{
   uint8 DTCAvailMask,dtc_i;
   uint8 SubFunction;
   uint8 Data[8];
   if(NetInd->N_Result==N_Success)
   {
      //成功接受到1个帧应停止P3 直到，处理完成才能开始，
      (void)TimeOutChkMsLong(&TimerP3Sever,0) ;
      //P3SeverStop=1;    //处理好了又重新开始所以不用停止
      APData.A_AI=NetInd->nId;
      APData.A_PCI=NetInd->Data[0];
      APData.A_Length=NetInd->Length;
      if(APData.A_Length<1) return;
      MemCopy(APData.A_Data, &NetInd->Data[1], (APData.A_Length-1));
      //begin p2can_server
     // (void)TimeOutChkMsLong(&TimerP2Can,0);
      //AppState=APP_STATE_RESPONE;    
   
	  (void)TimeOutChkMsLong(&TimerP3Sever,0);
      SubFunction = (APData.A_Data[0]&0x7f);

	  Receive_AppData(&APData);
      switch(APData.A_PCI- 0x40)  
      {
         case  SEVER_SID_SESSIONCONTROL:
			 if (SubFunction <= 3 )
			 {
				// FBSessionMode = SubFunction;
			 }
			 else
			 {
			 }
            //session control begin
            break;
       /***************session control end********************************/
         case  SEVER_SID_RESET:
         // 复位服务

            break;
        /***************reset sever end********************************/  
         case  SEVER_SID_SECURITY:
            if (APData.A_Data[0] == SECURITY_REQSD_LEV1 || 
                APData.A_Data[0] == SECURITY_REQSD_LEV2)
            {
               
            nSeed =  APData.A_Data[1];
            nSeed <<= 8;
            nSeed += APData.A_Data[2];
            nSeed <<= 8;
            nSeed += APData.A_Data[3];
            nSeed <<= 8;
            nSeed += APData.A_Data[4];

			
			HMODULE hDll = LoadLibrary(str_pathDll);
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
			/*if (_SK81_SCURITY_)
			{
				nKey = seedToKey1(); 
			}else{
			if (scurityType == 0)
			{
				nKey = seedToKey(); 
			}
			else
			{
				nKey = seedToKey1(); 
			}   
			}*/
			
            Data[0] = 0x27;
            if(SecurityState == SECURITY_STATE_LEVEL1)
               Data[1] = SECURITY_SENDK_LEV1;
            else Data[1] = SECURITY_SENDK_LEV2;
            Data[2] = (uint8)(nKey >> 24);
            Data[3] = (uint8)((nKey >> 16) & 0x00ff);
            Data[4] = (uint8)((nKey & 0x0000ff00) >> 8);
            Data[5] = (uint8) (nKey & 0x000000ff);
            (void)F_N_USDATA_REQ(Data, 6, 0x77); 
            SecurityState =  255;
            }
            break;
        /***************security sever end********************************/
         case  SEVER_SID_TP:
         //tp begin   test present
			CANAPP_TestPresent();
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
            Deal_WriteDID(&APData.A_Data[0], APData.A_Length);
            break;
         // write data by info   end
         case SEVER_SID_IOCONTROL:
          //  Deal_IOControl();
            break;         
         case SEVER_SID_WRITEBYADDR:
            Respone_NRC(APData.A_PCI,NRC_SNS,APData.A_AI);
            break;
         case SEVER_SID_READBYADDR:
          //  Deal_ReadAddr();
            break; 
         //IO CONTROL  end
         case  SEVER_SID_DTCCONTROL:
         //dtc control begine

            break;
            //dtc control end
          case  SEVER_SID_READDTC :
            //read   dtc  begin
			Deal_ReadDTC(&APData.A_Data[0], APData.A_Length);
            break;
           //read   dtc  end 
         case  0x34:
         case  0x36:
         case  0x37:
            //Data[0] = APData.A_PCI - 0x40;
		//	ApplayTranslate(Data);
          //  Send_Uart_Fram(Data, 2); 
            break;
          case SEVER_SID_READDATABYINF:
         // read data by info
     
          //  Send_Uart_Fram(NetInd->Data, APData.A_Length);
			Deal_ReadDID(&APData.A_Data[0], APData.A_Length);
            break;    
         case 0x3f:
         //否定响应吗
            Data[0] = 0x7f;
            Data[1] = APData.A_Data[0];
            Data[2] = APData.A_Data[1];
		//	ApplayTranslate(Data);
           // Send_Uart_Fram(Data, 3); 
            break;
      }
     

   }
   else
   {//result not success
      switch(NetInd->N_Result)
      {
         default:
            break;
         case  N_Timeout_A:

         case  N_Timeout_Cr:
         //rec err
         case  N_Wrong_SN:
          //rec err ,start s3 
            (void)TimeOutChkMsLong(&TimerP3Sever,0) ;
            P3SeverStop=0;
            AppState= APP_STATE_IDLE;
            break;  
      }
   }

}

void  F_N_USData_FF_IND(uint8 nId,uint16 Length)
{
   if(AppState==APP_STATE_IDLE)
   {  //stop p3 sever
      P3SeverStop=1; 
   }
}


/*******************************************************************************
 * Function:        F_N_USData_CON(N_USDATA_CON   NetCon)
 *
 * Description:     网络层发送一帧出去，返回的状态
 *
 * Returns:        none   
 *
 *
 * Notes:          NetCon 的状态 为 以下。复位服务在这里实现		
 *
********************************************************************************/
void  F_N_USData_CON(N_USDATA_CON   NetCon)
{
   switch(NetCon.N_Result)
   {
      default:
         break;
      case  N_Buffer_Overflow:
      case  N_WFT_OVRN:
      case  N_Invalid_FS:
      case  N_Timeout_Bs:
      //发送出现故障重新开始计时
            (void)TimeOutChkMsLong(&TimerP3Sever,0) ;
            P3SeverStop=0;
            AppState= APP_STATE_IDLE;
            break;
      case  N_Success:
            //发送完成重新定时
            (void)TimeOutChkMsLong(&TimerP3Sever,0) ;
            P3SeverStop=0;
            AppState= APP_STATE_IDLE;
            //复位相关处理
        /*    if (UDS_ResetState)
            {  //hardware reset use watchdog out reset
               UDS_ResetState = 0;
               while (1)
               {
                  asm("nop");
               }
            }*/
            break;
      
   }
}

/*******************************************************************************
 * Function:       void  AppLay_DiagControl(void)
 *
 * Description:     CAN总线网络层 接受到一个信号，并处理
 *
 * Returns:         none
 *
 * Notes:           在诊断帧()收到时,调用		
 *
********************************************************************************/
uint8 DiagData[50], Length;
uint8 SendFlg;
uint16 nTPTimes; 
void  AppLay_DiagControl(void)
{
    uint8 Data[8];
 /*  switch(AppState)
   {
      case  APP_STATE_IDLE:
         if(TimeOutChkMsLong(&TimerP3Sever,S3_Sever)&&
         SessionMode&&!P3SeverStop)
         {
            SessionMode=SESSION_MODE_DEFAULT;      //out udefault session
            CCState.Byte=0;
            StorState=0; 
            SecurityState = SECURITY_STATE_LOCK;
         }
         break;
      case  APP_STATE_RESPONE:
         break;
      case  APP_STATE_DEALREQUEST:
         if(TimeOutChkMsLong(&TimerP2Can,P2CAN_Sever))
         {//p2 can sever timeout
            
         }
         break;
      default:
         break  ;
   }*/
   //安全访问 延时机制处理
   if (SecurityState == SECURITY_STATE_TIMELOCK)
   {
      if (TimeOutChkTenthSLong(&TimerSecurity, 100))
      {
         SecurityErrTimes--;
         SecurityState = SECURITY_STATE_LOCK;      
      }
   }
   if (SendFlg)
   {
      SendFlg = 0;
      F_N_USDATA_REQ(DiagData, Length, 0x21) ;
   }
   if (TimeOutChkMsLong(&TimerP3Sever,3000)) 
   {
      Data[0] = 0x3e;
      Data[1] = 0;
    //  F_N_USDATA_REQ(Data, 2, 0x21);
      nTPTimes++;
   }
   
   //回话控制
   switch(SessionMode)
   {
      case SESSION_MODE_DEFAULT:
         Data[0] = 0x10;
         Data[1] = SESSIONCONTROL_TYPE_DS;
         F_N_USDATA_REQ(Data, 2, 0x77); 
         SessionMode = 255;
         break;   
      case SESSION_MODE_EXTERN:  
         Data[0] = 0x10;
         Data[1] = SESSIONCONTROL_TYPE_EXTDSE;
         (void)F_N_USDATA_REQ(Data, 2, 0x77); 
         SessionMode = 255;
         
         break;
      case SESSION_MODE_PROGRAM:
         Data[0] = 0x10;
         Data[1] = SESSIONCONTROL_TYPE_PRGS;
         (void)F_N_USDATA_REQ(Data, 2, 0x77); 
         SessionMode = 255;
         break;        
   }
   
  /* switch(SecurityState)
   {
      case  SECURITY_STATE_SSEED1 :
         Data[0] = SEVER_SID_SECURITY;
         Data[1] = SECURITY_REQSD_LEV1;
          (void)F_N_USDATA_REQ(Data, 2, 0x77); 
         SecurityState = SECURITY_STATE_LEVEL1;
         break; 
      case  SECURITY_STATE_SSEED2:
         Data[0] = SEVER_SID_SECURITY;
         Data[1] = SECURITY_REQSD_LEV2;
         (void)F_N_USDATA_REQ(Data, 2, 0x77); 
         SecurityState = SECURITY_STATE_LEVEL2;
         break; 
   }*/
  // SetSecurityType(1);
//   Deal_ReadDID() ;
}


void SetSecurityType(uint8 id)
{

}