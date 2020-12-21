/*******************************************************************
2015.8.20
ver03
1 �����汾��
**********************************************************************/
#ifndef  _NET_LAY_H__
#define  _NET_LAY_H__
#include"InterLay.h"
//#include "DataType.h"


#define  ID_DEFINE_TARGET  0x44

#define  NETLAY_MAXLENGTH    255  //������������ܽ��ܵ���󳤶�,����������أ�֧��dtc12 12*4+3=51
#define  MAX_NPDU_BUFFER     50   //����㴦�������Ļ���
// result define
#define  N_Success      0
#define  N_Timeout_A    1
#define  N_Timeout_Bs   2
#define  N_Timeout_Cr   3
#define  N_Wrong_SN     4
#define  N_Invalid_FS   5   //�Ƿ���������֡
#define  N_Buffer_Overflow 6  
#define  N_UNEXP_PDU       7   //���յ���Ԥ�ڵ�PDU�����շ�Ӧ���Ըñ��ģ�
#define  N_WFT_OVRN        8   //�ȴ�������֡������
#define  N_ERROR           9

//result of ChangeParameter
#define  N_Success                  0
#define  N_Undefined_Parameter      1
#define  N_Wrong_Value              2
#define  N_Busy                     3



#define   AID_PHYSIC     (uint8)(ID_DIAG_CLM&0x0ff)
#define   AID_FUNIC     (uint8) (ID_DIAG_FUNCT&0x0ff) 
 
 
/*******************************************************************************
 *���ݽṹ����
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

//���ͽ������ݵ� ���ݽṹ
typedef  struct
{
   N_AI     nId;             //����ʱΪ Ŀ��ID,����ʱΪ�����ߵ�ID
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


//����1֡�����ݽṹ
typedef struct
{
   uint8    S_State;   //����״̬
   uint16   S_Length;  //����
   uint8    S_mId;     //Ŀ��ID
   uint8    S_SN;     //SN 
   uint8    S_BS;       //BS 
   uint16    S_STmin;    //stmin 
   uint16   S_TimerSend;    
} N_SendData;


//����1֡�����ݽṹ
typedef struct
{
   uint8    R_State;   //����״̬
   uint16   R_Length;  //����
   uint8    R_mId;     //���ܵ�ID
   uint8    R_SN;     //SN 
   uint8    R_BS;       //BS 
   uint16    R_STmin;    //stmin   
} N_RecData;
/**********************************************************************

����״̬��ض���  ,  NM_IND(XXX) 
************************************************************************/



/*********************************************************

NM lay  ��ض���NetMange_State
************************************************/

extern void MemCopy(uint8* dest,uint8* Src,uint8 Length);
void  Deal_UDSNetLay(void);
#endif
