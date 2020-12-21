/*******************************************************************
2015.8.20
ver03
1 建立版本号
**********************************************************************/
#ifndef  _NET_LAY_H__
#define  _NET_LAY_H__
#include"InterLay.h"
//#include "DataType.h"


#define  ID_DEFINE_TARGET  0x44

#define  NETLAY_MAXLENGTH    255  //定义网络层所能接受的最大长度,和最大个数相关，支持dtc12 12*4+3=51
#define  MAX_NPDU_BUFFER     50   //网络层处理物理层的缓存
// result define
#define  N_Success      0
#define  N_Timeout_A    1
#define  N_Timeout_Bs   2
#define  N_Timeout_Cr   3
#define  N_Wrong_SN     4
#define  N_Invalid_FS   5   //非法的流控制帧
#define  N_Buffer_Overflow 6  
#define  N_UNEXP_PDU       7   //接收到非预期的PDU，接收方应忽略该报文；
#define  N_WFT_OVRN        8   //等待流控制帧数错误
#define  N_ERROR           9

//result of ChangeParameter
#define  N_Success                  0
#define  N_Undefined_Parameter      1
#define  N_Wrong_Value              2
#define  N_Busy                     3



#define   AID_PHYSIC     (uint8)(ID_DIAG_CLM&0x0ff)
#define   AID_FUNIC     (uint8) (ID_DIAG_FUNCT&0x0ff) 
 
 
/*******************************************************************************
 *数据结构定义
 *
********************************************************************************/
#define  N_AI     uint8
#define  N_DATA   uint8

//netlay fram pci struct
typedef  struct
{
   unsigned    Length_SN  :4;
   unsigned    nPciType   :4;   //    
}N_PCI;

//netlay protect data unit struct
typedef struct
{
   N_AI  nId;
   N_PCI Pci;
   N_DATA   Data[7];
}N_PDU;

//发送接受数据的 数据结构
typedef  struct
{
   N_AI     nId;             //发送时为 目标ID,接受时为发送者的ID
   uint8    Data[NETLAY_MAXLENGTH];
   uint16   Length;
   uint8    N_Result;
}N_USDATA_IND;


/// n_usdata_con to app  struct
typedef  struct
{
   N_AI     nId;
   uint8    N_Result;
}N_USDATA_CON;


//发送1帧的数据结构
typedef struct
{
   uint8    S_State;   //发送状态
   uint16   S_Length;  //长度
   uint8    S_mId;     //目标ID
   uint8    S_SN;     //SN 
   uint8    S_BS;       //BS 
   uint16    S_STmin;    //stmin 
   uint16   S_TimerSend;    
} N_SendData;


//接受1帧的数据结构
typedef struct
{
   uint8    R_State;   //接受状态
   uint16   R_Length;  //长度
   uint8    R_mId;     //接受的ID
   uint8    R_SN;     //SN 
   uint8    R_BS;       //BS 
   uint16    R_STmin;    //stmin   
} N_RecData;
/**********************************************************************

总线状态相关定义  ,  NM_IND(XXX) 
************************************************************************/



/*********************************************************

NM lay  相关定义NetMange_State
************************************************/

extern void MemCopy(uint8* dest,uint8* Src,uint8 Length);
void  Deal_UDSNetLay(void);
#endif
