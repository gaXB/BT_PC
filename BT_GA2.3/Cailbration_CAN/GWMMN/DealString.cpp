/*******************************************************
�汾��Ϣ
ver0.0.0
2015.8.23
��ʼ�汾
1 ���鴫�ݣ���kea�˺�����memcpy���ص�

********************************************************/
#include "stdafx.h"
#include"DataType.h"


/*******************************************************************************
 * Function:        void MemCopy(uint8* dest,uint8* Src,uint8 Length)
 *
 * Description:     ����֮�䴫������
 *
 * Returns:         none
 *
 * Notes:           none		
 *
********************************************************************************/
void MemCopy(uint8* dest,uint8* Src,uint8 Length)
{
   uint8 i;
   for(i=0;i<Length;i++)
   {
      *dest++=*Src++;
   }
}
