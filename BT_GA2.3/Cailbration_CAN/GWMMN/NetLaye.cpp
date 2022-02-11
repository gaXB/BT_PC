/*******************************************************************
ver 000 
2013.1.1
UDS 网络层 15765-2 
初始版本建立，EJ04调试成功

2015.01.30
ver01
1 修改部分功能经过saic测试
2 去掉流控制帧的id 判定（未测试）
3 发送信号未用到默认为0

2015.2.23
ver02
1 2015 2 13 在新电调试
2  通信为半双工 模式，所以在发送时候不能进行接收 
   在接收的时候发送的处理 未做
3  流控制帧必须为物理寻址
4  在其他情况接受到流控制帧，忽略
5  发送完不需要等待流控帧

2015.8.20
ver03
1 加入关闭CAN的函数 Dis/EnableCanRxInterrupt

2022.2.11
ver04
1 缓存机制bug修改

**********************************************************************/
#include "stdafx.h"
#include "NetLaye.h"
#include "timerout.h"

/**************************************************************
网络层宏定义                                      
*********************************************************************/

//pci type define
#define  PCI_TYPE_SF    0
#define  PCI_TYPE_FF    1
#define  PCI_TYPE_CF    2
#define  PCI_TYPE_FC    3

//flow control fram fs type
#define FC_FS_CONTISEND   0
#define FC_FS_WAIT        1
#define FC_FS_OVER        2

     
//接受状态

#define  RECIVE_STATE_IDLE 0
#define  RECIVE_STATE_     
#define  RECIVE_STATE_RECCONDION    2  //接受连续帧
#define  RECIVE_STATE_SENDFC   3

//发送状态
#define  SEND_STATE_IDLE 0
#define  SEND_STATE_SEND_FF   1     
#define  SEND_STATE_SENCONDION    2  //SEND连续帧
#define  SEND_STATE_WAITFC    3
//#define  SEND_STATE_REC_FC    4

//定时参数
#define     TIMER_N_AR     70   //物理层发送的时间
#define     TIMER_N_AS     70
#define     TIMER_N_BS     1500 // 发送放接受流控制帧的时间
#define     TIMER_N_CR     1500 // 接受放接受连续帧的时间

//网络层参数
#define  NETLAY_BS      0     //fc  continue times
#define  NETLAY_STmin   15    // continue fram  send time must small than it
#define  NETLAY_N_WFTMAX      0  // The FC Receive times must small than it

uint32 ID_DIAG_FUNCT = 0x600;
uint32 ID_DIAG_PHYSIC = 0x618;
uint32 ID_DIAG_CLM = 0x619;
uint32 ID_FRAMESIZE = 0;  //标准帧
/**************************************************************
寄存器定义                                   
*********************************************************************/

static   N_PDU   nPdu_Buffer[MAX_NPDU_BUFFER];
static   uint8   nPduDealNumber=0;
static   N_USDATA_IND   nUsdata_Rec; //  接受数据缓存

static   N_USDATA_CON   nUsdata_Con;           //confirm
static   N_USDATA_IND   nUsdata_Send,nUsdata_Send_1;        //  发送数据缓存
static   uint8          SendBuffNumber=0;
//定时参数
static   uint16 TimerN_Ar,   //物理层发送的时间
      TimerN_As,
      TimerN_Bs,     // 发送放接受流控制帧的时间
      TimerN_Cr;     // 接受放接受连续帧的时间
 
static N_SendData SData_FL;     //funct and  phy  all use this
static N_RecData  RData_FL=
 {
 0,0,0,0,NETLAY_BS,NETLAY_STmin
 };         //funct and phy all use this

static uint8 Bug_Num;
  // static uint16 st_i,stmin[8];
  
/*******************************************************************************
需调用的外部函数声明
********************************************************************************/
void  L_SendDiagFram(uint8* NetData);
void  F_N_USData_IND(N_USDATA_IND*   NetInd) ;
void  F_N_USData_CON(N_USDATA_CON   NetCon) ;
void  F_N_USData_FF_IND(uint8 nId,uint8 Length);
void DisableCanRxInterrupt(void);
void EnableCanRxInterrupt(void);
//本模块实现的函数
void  NetLay_RecFC(N_PDU    *nPdu) ;
void  NetLay_Recive(N_PDU   *nPdu);



void Send_78NRC(uint8 SID)
{
   uint8 LL_Data[8] = 
   {
   0,0,0,0,0,0,0,0
   };
   LL_Data[0] = 0x03;
   LL_Data[1]=0x7f ;    //pci 0
   LL_Data[2]= SID;        //pci 1
   LL_Data[3]= 0x78;
   
   L_SendDiagFram(LL_Data);
}
/*******************************************************************************
 * Function:        void  NetLay_Recive(N_PDU *  nPdu)
 *
 * Description:     CAN总线网络层 接受到一个信号，并处理
 *
 * Returns:         none
 *
 * Notes:           在诊断帧()收到时,调用		
 *
********************************************************************************/
uint8 test1;
void NetLay_Recive(N_PDU*   nPdu)
{
//   uint8 i;

   N_PCI CCF_Pci;
   uint8 LL_Data[8] = 
   {
   0,0,0,0,0,0,0,0
   };
   
   //fc fram   deal
  // NetLay_RecFC(nPdu);
   //rec deal
   if (SData_FL.S_State != SEND_STATE_IDLE) 
   { //通信为半双工 模式，所以在发送时候不能进行接收 
     //在接收的时候发送的处理 未做
     return ;
   }
   if(nPdu->Pci.nPciType==PCI_TYPE_SF||
   nPdu->Pci.nPciType==PCI_TYPE_FF )
   {
     RData_FL.R_State=RECIVE_STATE_IDLE; 
   }
   switch(RData_FL.R_State)
   {
      case RECIVE_STATE_IDLE:
         if(nPdu->Pci.nPciType==PCI_TYPE_SF) 
         {  //SF
            if(nPdu->Pci.Length_SN&&
            nPdu->Pci.Length_SN<=7)
            {   
            #ifdef _DEBUG_   
               if(Test_Ser)
               {
                  SysErcode.Bits.b0=1;
               }
            #endif
               nUsdata_Rec.N_Result=N_Success;
               nUsdata_Rec.nId= nPdu->nId;
               nUsdata_Rec.Length=nPdu->Pci.Length_SN;
              //recive sf
               MemCopy(nUsdata_Rec.Data, nPdu->Data, (uint8)nUsdata_Rec.Length)  ; 
               RData_FL.R_Length=nUsdata_Rec.Length;
               F_N_USData_IND(&nUsdata_Rec) ;
            }
         }
         else  if(nPdu->Pci.nPciType==PCI_TYPE_FF)
         {  //FF
            nUsdata_Rec.nId= nPdu->nId;
            if(nPdu->Pci.Length_SN==0&&nPdu->Data[0]<NETLAY_MAXLENGTH)
            {
               if(nPdu->Data[0]==0) return;
               nUsdata_Rec.Length=nPdu->Data[0];  //只支持最大255 个数
               CCF_Pci.Length_SN=FC_FS_CONTISEND ; // 
               RData_FL.R_State=RECIVE_STATE_RECCONDION;  //begin rec continue fram
               
               // 物理层定时
               (void)TimeOutChkMsLong(&TimerN_Ar,0) ;
               (void)TimeOutChkMsLong(&TimerN_Cr,0) ;
               RData_FL.R_mId=nPdu->nId;
               //reciv ff  ,first fram  data[0]=length ,data[1]-ind.data[0]
               MemCopy(nUsdata_Rec.Data, &nPdu->Data[1], 6) ;  
               RData_FL.R_Length=6;
               RData_FL.R_SN=1;
               
               F_N_USData_FF_IND(nUsdata_Rec.nId,(uint8)nUsdata_Rec.Length);
            }
            else
            {  // max lenth<256 err  N_Buffer_Overflow
              // nUsdata_Rec.nId=nPdu.nId;
              // nUsdata_Rec.N_Result = N_Buffer_Overflow ;
               //confirm to applay
              // F_N_USData_CON(nUsdata_Con)  ;    //receiver don't need con to app
               
               CCF_Pci.Length_SN=FC_FS_OVER; //send over cf
               
            }

          //  if(nPdu->nId==AID_PHYSIC)
            {
               CCF_Pci.nPciType=PCI_TYPE_FC;
               //send flow control fram
               LL_Data[0]=*(uint8*)&CCF_Pci;
               LL_Data[1] = NETLAY_BS;         
               RData_FL.R_BS = NETLAY_BS;      // 更改bs，设定下个流控制帧发送的bs
               LL_Data[2]=(uint8)NETLAY_STmin; 
               //流控制帧只支持物理寻址
               L_SendDiagFram(LL_Data);
            }
         }
         //other fram is err
         break;
      case  RECIVE_STATE_RECCONDION :
         if(TimeOutChkMsLong(&TimerN_Cr,TIMER_N_CR) )
         {//超时
             nUsdata_Rec.nId= nPdu->nId;
             nUsdata_Rec.N_Result = N_Timeout_Cr ;
            //ind to applay
             F_N_USData_IND(&nUsdata_Rec)  ;
             RData_FL.R_State=RECIVE_STATE_IDLE;      //timerout cr stop rec
         }
         else
         {
            if( RData_FL.R_mId==nPdu->nId&&nPdu->Pci.nPciType==PCI_TYPE_CF)
            {
               //RECIVE CONTINUE
               (void)TimeOutChkMsLong(&TimerN_Cr,0)  ;
              
               if (RData_FL.R_SN==nPdu->Pci.Length_SN)
               { //drge SN
                  RData_FL.R_SN++;
                  if(RData_FL.R_SN>=16)  RData_FL.R_SN=0;
                  if((nUsdata_Rec.Length-RData_FL.R_Length)<=7)
                  {  //recieve ok
                     //Recive 
                     MemCopy(&nUsdata_Rec.Data[RData_FL.R_Length], nPdu->Data,  nUsdata_Rec.Length-RData_FL.R_Length) ;  
                     nUsdata_Rec.N_Result= N_Success;
                     RData_FL.R_Length+=nUsdata_Rec.Length;
                     
                     RData_FL.R_State=RECIVE_STATE_IDLE;
                     F_N_USData_IND(&nUsdata_Rec);
                     

                  }
                  else
                  {
                     //Recive cf 7
                     MemCopy(&nUsdata_Rec.Data[RData_FL.R_Length], nPdu->Data, 7) ; 
                     RData_FL.R_Length+=7;
                     
                     //add bs !=0  program
                     RData_FL.R_BS--;
                     test1 = RData_FL.R_BS;
                     if (test1 < 2)
                     {
                        test1++;
						test1--;
                     }
                   //  if (!RData_FL.R_BS && NETLAY_BS && nPdu->nId==AID_PHYSIC)
					 if (!RData_FL.R_BS && NETLAY_BS)
                     {// bs发送数到， 需要发送fc， 是物理寻址
                        //send flow control fram
                       // CCF_Pci.nPciType=PCI_TYPE_FC;
                        LL_Data[0]= 0x30;     //不需要判断长度前面已经判断过了
                        LL_Data[1] = NETLAY_BS;         
                        RData_FL.R_BS = NETLAY_BS; 
                        LL_Data[2]=(uint8)NETLAY_STmin; 
                        //流控制帧只支持物理寻址
                        L_SendDiagFram(LL_Data);   
                     }
                     
                  }
               }
               else
               {
                   nUsdata_Rec.nId= nPdu->nId;
                   nUsdata_Rec.N_Result = N_Wrong_SN ;
                  //IND to applay Wrong SN
                   F_N_USData_IND(&nUsdata_Rec)  ;
                   RData_FL.R_State=RECIVE_STATE_IDLE;      // stop rec
               }   
            } 
         }
         break;
      default:
         break;  
   }
  //nPdu.nid
  //switch(nPdu.pci.PCI.nPciType) 
}

/*******************************************************************************
 * Function:        uint8  F_N_USDATA_REQ(uint8* ReqData,uint16 Length)
 *
 * Description:     CAN总线网络层 ,由应用层调用发送数据
 *
 * Returns:         none
 *
 * Notes:           应用层调用		
 *
********************************************************************************/
uint8  F_N_USDATA_REQ(uint8* ReqData,uint16 Length,uint8 mId_Target)
{

  
   if(Length>NETLAY_MAXLENGTH)
   {
      return 0;
   }
   #ifdef _DEBUG_
   if(Test_Ser)
   {
      SysErcode.Bits.b3=1;
   }
   #endif
   if(SData_FL.S_State!=SEND_STATE_IDLE)
   {  //放入缓存空间
      nUsdata_Send_1.Length=Length;
      nUsdata_Send_1.N_Result=0;
      MemCopy(nUsdata_Send_1.Data, ReqData, (uint8)Length);
      nUsdata_Send_1.nId=mId_Target; 
      SendBuffNumber=1;
   }
   else
   {
      nUsdata_Send.Length=Length;
      nUsdata_Send.N_Result=0;
      MemCopy(nUsdata_Send.Data, ReqData, (uint8)Length);
         
      nUsdata_Send.nId=mId_Target;  
      SData_FL.S_mId=mId_Target;
      SData_FL.S_State=SEND_STATE_SEND_FF;
   }
   return 1;
}
/*******************************************************************************
 * Function:       void  NetLay_RecFC(N_PDU   *nPdu) 
 *
 * Description:     CAN总线网络层 Receive Control Fram
 *
 * Returns:         none
 *
 * Notes:         接受到 control fram 时调用
 *
********************************************************************************/
void  NetLay_RecFC(N_PDU   *nPdu) 
{

   if(SData_FL.S_State==SEND_STATE_WAITFC
   &&nPdu->Pci.nPciType==PCI_TYPE_FC)
  // && nPdu->nId == AID_PHYSIC)
   {
       //接受到流控制帧继续发送
     // if(SData_FL.S_mId==nPdu->nId)
    //  {不需要判断发过来的id和上次请求的ID是不是同一个ID 20150130 xb
         
         // Pci.Length_SN==FS data[0]=bs  data[1]=stmin  
         switch(nPdu->Pci.Length_SN)
         {
            case  FC_FS_CONTISEND:
            //接受到流控制帧
               if(TimeOutChkMsLong(&TimerN_Bs,TIMER_N_BS))
               {  //发送失败，流控制帧等待时间超时
                  nUsdata_Con.nId= nUsdata_Send.nId;
                  nUsdata_Con.N_Result = N_Timeout_Bs ;
                  //con to  TIMER_N_BS
                  F_N_USData_CON(nUsdata_Con)  ;
                  SData_FL.S_State=SEND_STATE_IDLE;
                  if(nUsdata_Send.Length==51)
                  {
                     Bug_Num=128;
                      
                  }
               }
               else
               {
                 //接受到流控制帧
                  SData_FL.S_BS=nPdu->Data[0];
                  SData_FL.S_STmin=nPdu->Data[1];

                  SData_FL.S_State=SEND_STATE_SENCONDION ;
               }
               break;
            case  FC_FS_WAIT:
            // 等待下一帧流控制帧
               (void)TimeOutChkMsLong(&TimerN_Bs,0);
               //此处需要注意是否超过最大N_WFT,
               if(NETLAY_N_WFTMAX==0)
               {
                  nUsdata_Con.nId= nUsdata_Send.nId;
                  nUsdata_Con.N_Result = N_WFT_OVRN ;
                  //con to applay N_WFT_OVRN
                  F_N_USData_CON(nUsdata_Con)  ;
                  //don't send  
                  SData_FL.S_State=SEND_STATE_IDLE ;
               }
               else
               {
               }
               
               break;
            case  FC_FS_OVER:
              //发送数据长度太长
               nUsdata_Con.nId= nUsdata_Send.nId;
               nUsdata_Con.N_Result = N_Buffer_Overflow ;
               //con to applay length over
               F_N_USData_CON(nUsdata_Con)  ;
               //don't send  
               SData_FL.S_State=SEND_STATE_IDLE ;
               break;
            default:           //err fc
               nUsdata_Con.nId= nUsdata_Send.nId;
               nUsdata_Con.N_Result = N_Invalid_FS ;
               //con to applay N_Invalid_FS
               F_N_USData_CON(nUsdata_Con)  ;
               //don't send  
               SData_FL.S_State=SEND_STATE_IDLE ;
               break;
               
         }
      }
      else
      {   ////只有在等待流控制帧的状态才有效，其他就忽略此帧   150212
      //   SData_FL.S_State=SEND_STATE_IDLE ;
      }
  // }
}

/*******************************************************************************
 * Function:     void  NetLay_SendFram(void)
 *
 * Description:     CAN总线网络层1  数据发送处理   2 接受连续帧时 TimerN_Cr超时处理
 *
 * Returns:         none
 *
 * Notes:       LOOP处理
 *
********************************************************************************/
void  NetLay_SendFram(void)
{
   uint8   LL_Data[8] =
   {
   0, 0, 0, 0, 0, 0, 0, 0
   };
   N_PDU   nPdu;
   //150831增加，如果数据正在接受则不能发送
      //rec continue fram  TimerN_Cr timeout
   if(RData_FL.R_State==RECIVE_STATE_RECCONDION)
   {
      if(TimeOutChkMsLong(&TimerN_Cr,TIMER_N_CR) )
      {//超时
      //    nUsdata_Rec.nId= nPdu.nId;
          nUsdata_Rec.N_Result = N_Timeout_Cr ;
         //ind to applay
          F_N_USData_IND(&nUsdata_Rec)  ;
          RData_FL.R_State=RECIVE_STATE_IDLE;      //timerout cr stop rec
      }  
   }
   if (RData_FL.R_State != RECIVE_STATE_IDLE)  return;
   switch(SData_FL.S_State) 
   {
      default:
      case SEND_STATE_IDLE:
         if(SendBuffNumber)
         {
            SendBuffNumber=0;
            nUsdata_Send=nUsdata_Send_1;
            SData_FL.S_mId=nUsdata_Send.nId;
            SData_FL.S_State=SEND_STATE_SEND_FF;   
         }
         break; 
      case  SEND_STATE_SEND_FF:
        // send first fram
         if(nUsdata_Send.Length<=7)
         {  //send signal fram
          //  nPdu.Pci.PCI.             
            nPdu.Pci.Length_SN=(uint8)nUsdata_Send.Length;
            nPdu.Pci.nPciType=PCI_TYPE_SF;
            LL_Data[0]=*(uint8*)&nPdu.Pci;
            MemCopy(&LL_Data[1], nUsdata_Send.Data, nPdu.Pci.Length_SN) ;
            SData_FL.S_State=SEND_STATE_IDLE;
            //send ok confrim to app ok
            nUsdata_Con.nId= nUsdata_Send.nId;
            nUsdata_Con.N_Result = N_Success ;
            L_SendDiagFram(LL_Data); 
            F_N_USData_CON(nUsdata_Con) ;   //在success 应该在发送完成在指示给应用层
         }
         else
         {  //send first fram
            nPdu.Pci.Length_SN=0;
            nPdu.Pci.nPciType=PCI_TYPE_FF;
            LL_Data[0]=*(uint8*)&nPdu.Pci;
            LL_Data[1]= nUsdata_Send.Length&0x00ff;
            MemCopy(&LL_Data[2], nUsdata_Send.Data, 6) ;
            SData_FL.S_State=SEND_STATE_WAITFC;
            (void)TimeOutChkMsLong(&TimerN_Bs,0);
            SData_FL.S_Length=6;
            SData_FL.S_SN=1;
            L_SendDiagFram(LL_Data); 
         }
         
         break;
      case  SEND_STATE_WAITFC:
      //等待流控制帧
         if(TimeOutChkMsLong(&TimerN_Bs,TIMER_N_BS))
         {  //发送失败，流控制帧等待时间超时
            nUsdata_Con.nId= nUsdata_Send.nId;
            nUsdata_Con.N_Result = N_Timeout_Bs ;
            //con to applay N_Timeout_Bs 
            F_N_USData_CON(nUsdata_Con)  ;
            SData_FL.S_State=SEND_STATE_IDLE;
            if(nUsdata_Send.Length==51)Bug_Num++;
         }
         break;
      case  SEND_STATE_SENCONDION:
         //send continue fram
         //if(SData_FL.)
         if(TimeOutChkMsLong(&SData_FL.S_TimerSend, SData_FL.S_STmin))
         {
            nPdu.Pci.nPciType=PCI_TYPE_CF; //CONTINUE FRAM
            nPdu.Pci.Length_SN=SData_FL.S_SN++;
            LL_Data[0]=*(uint8*)&nPdu.Pci;
            if((nUsdata_Send.Length-SData_FL.S_Length)>7)
            {
               MemCopy(&LL_Data[1], &nUsdata_Send.Data[SData_FL.S_Length], 7) ;
               L_SendDiagFram(LL_Data);
               
               SData_FL.S_BS--;  //150212
               if (SData_FL.S_BS == 0)
               {//如果连续帧发送次数已经到达BS，需要等待流控制帧
                  SData_FL.S_State=SEND_STATE_WAITFC;
                  (void)TimeOutChkMsLong(&TimerN_Bs,0);  
               }  
            }
            else
            {//send last fram
               MemCopy(&LL_Data[1], &nUsdata_Send.Data[SData_FL.S_Length], nUsdata_Send.Length-SData_FL.S_Length) ;
               SData_FL.S_State=SEND_STATE_IDLE;   
              //send ok confrim to app ok
               nUsdata_Con.nId= nUsdata_Send.nId;
               nUsdata_Con.N_Result = N_Success ;
               L_SendDiagFram(LL_Data); 
               F_N_USData_CON(nUsdata_Con) ;
            }
            
            SData_FL.S_Length+=7;

         }
         //must care the bs
         break;     
   }
   

   
}

/*******************************************************************************
 * Function:        void  NetLay_Recive(N_PDU   nPdu)
 *
 * Description:     CAN LL_LAY 接受到一个CAN 帧时进行处理，放入nPdu_Buffer[] 
 *
 * Returns:         none
 *
 * Notes:           逻辑链路层调用，		
 *
********************************************************************************/
void  NetLay_ReciveDiag(uint8 nId,uint8* L_data)
{
 //  int i=0;
   N_PDU   nPdu;
   nPdu.nId=nId;
   nPdu.Pci=*(N_PCI*)L_data;
   MemCopy(nPdu.Data, &L_data[1], 7) ;
  
   #ifdef _DEBUG_ 
   if( Test_Ser)  Test_Ser=0;

   if(nId==0&&L_data[0]==4&&
   L_data[1]==0X22&&L_data[2]==0&L_data[4]==0)
   {
         asm "nop" ;
      //   Bug_Num++;
         asm "nop";
      #ifdef _DEBUG_
         if(Bug_Num%4==1)
         {
            Test_Ser=1;
         }
         else
         {
            Test_Ser=0;  
            SysErcode.Byte=0;
         }
      #endif
   } 
   #endif
   if(nPduDealNumber<MAX_NPDU_BUFFER)
   {
      nPdu_Buffer[nPduDealNumber]=nPdu; 
      nPduDealNumber++;
   }
   else
   {
   //   NOP;  //溢出
   #ifdef _DEBUG_
      Test_Flg=1;
   #endif
   }
}

/*******************************************************************************
 * Function:        void  Deal_UDSNetLay(void)
 *
 * Description:     处理网络层数据， 1   发送数据  2 处理接受 
 *
 *                 
 * Returns:         none
 *
 * Notes:           放在mainloop中。		
 *
********************************************************************************/
void  Deal_UDSNetLay(void)
{
   N_PDU   nPdu;
   uint8   deal_en=0,deal_num, i;
   
  // DisableCanRxInterrupt();
 //  DisableInterrupts;
   if(nPduDealNumber)
   {  //进行换冲处理
      nPduDealNumber--;
      nPdu=nPdu_Buffer[0]; 
      deal_en=1;
      deal_num=nPduDealNumber;
      //2016/7/4 处理bug， 
      for (i = 0; i < deal_num; i++)
      {
      	nPdu_Buffer[i]=nPdu_Buffer[i+1];
      }
      /*while(deal_num) 
      {
         nPdu_Buffer[deal_num-1]=nPdu_Buffer[deal_num];
         deal_num--;
      }*/
   }
   
//   EnableInterrupts ;
 //  EnableCanRxInterrupt();
   
   if(deal_en)
   {
//	   Clear_WatchDog(); /* feeds the dog */
      //处理接受的数据
      if(nPdu.Pci.nPciType==PCI_TYPE_FC)
      {//接受到流控制帧为调用 Send，其他调用REC
         NetLay_RecFC( &nPdu) ;
      } 
      else
      {
         NetLay_Recive( &nPdu)  ;
      }
   }
   
   //处理发送
   NetLay_SendFram() ;
   
}
