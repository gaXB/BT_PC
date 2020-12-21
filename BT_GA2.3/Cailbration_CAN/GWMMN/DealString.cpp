/*******************************************************
版本信息
ver0.0.0
2015.8.23
初始版本
1 数组传递，在kea此函数中memcpy有重叠

********************************************************/
#include "stdafx.h"
#include"DataType.h"


/*******************************************************************************
 * Function:        void MemCopy(uint8* dest,uint8* Src,uint8 Length)
 *
 * Description:     数组之间传递数据
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
