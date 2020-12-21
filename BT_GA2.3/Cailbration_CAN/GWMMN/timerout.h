 
#ifndef __TIMER_OUT__
#define __TIMER_OUT__


#include "DataType.h"
//#include "stdafx.h"
#define TPM_INTVALUE   1   

#define  CPU_8BIT      0

void MsCounterUpadat(void) ;

void TenthSCounterUpadat(void) ;

void SCounterUpadat(void) ;

uint8 TimeOutChkMsLong(uint16 *time ,uint16 timeOutVal);
uint8 TimeOutChkTenthSLong(uint16 *time ,uint16 timeOutVal);
uint8 TimeOutChkSLong(uint16 *time ,uint16 timeOutVal);
void Cal_MaxLoopTime(void);
#endif

