

/***********************************************************************
版本清单
ver0.0.4
2015-8-23
1 增加 CPU_8BIT 的宏定义 ，在不是8位的mcu中不需要停中断

ver0.0.3
2014-8-14
1 uint16 Cal_MaxLoopTime(void) 改为    void Cal_MaxLoopTime(void) 
2 MaxLoopTime 改为 volatile类型

ver0.0.2
2014-2-27
1 名字更改 timeout.c 改为timerout.c
2 增加uint16 Cal_MaxLoopTime(void)


ver0.0.1
2013-4-12
1 修改备注，
2 将变量hSdata 改为 TenthSdata

ver0.0.0
2012-11-10
1初始版本确定

***************************************************************************/
//
#include "stdafx.h"
#include "timerout.h"

#define  uint8  unsigned char 
//#define  uint16 unsigned short
#define	uint32	unsigned	long	

#define  int8   signed char
#define  int16  signed short
#define	int32		signed long

static unsigned short mSdata;         //每1ms加1

static uint16 TenthSdata;       //每0.1s加1

static uint16 Sdata;          //每1s加1



static uint16 TenthSecTimer,SecTimer;
 
volatile uint16 MaxLoopTime;  
 
 /***********************************
 通过硬件驱动需要调用的函数声明
 ************************************/
 //定时器模块
#if   CPU_8BIT
void  TimerCounterHalt(void) ;
void  TimerCounterResume(void) ;
#endif



/*******************************************************************************
 * Function:    uint8 TimeOutChkMsLong(uint16 *time ,uint16 timeOutVal) 
 *
 * Description: ms定时函数， *time 为定时的参数。timeOutVal为定时时间，为0则初始化*time
 *
 * Returns:     当到达时间时返回1， 且 *time初始化。否则为0
 *
 * Notes:    调用此函数查看定时是否到，考虑到实际情况，最大为65525ms
			
 *
*******************************************************************************/

uint8 TimeOutChkMsLong(uint16 *time ,uint16 timeOutVal)
{
  uint16 shadow,diftime;
  
#if   CPU_8BIT
   TimerCounterHalt();
#endif
  shadow= mSdata;
#if  CPU_8BIT
  TimerCounterResume();
#endif   
  diftime=shadow-*time;
    
  if(timeOutVal==0)
  {
    *time=shadow;
    return 1;
  }
  else if(diftime>=timeOutVal)    //判断是否到时
  {
      *time+=diftime;
      return 1; 
  }
  else  return 0;
}
  
/*******************************************************************************
 * Function:    uint8 TimeOutChkHsLong(uint16 *time ,uint16 timeOutVal) 
 *
 * Description: 0.1s定时函数， *time 为定时的参数。timeOutVal为定时时间，为0则初始化*time
 *
 * Returns:     当到达时间时返回1， 且 *time初始化。否则为0
 *
 * Notes:    调用此函数查看定时是否到，考虑到实际情况，最大为65530/2 s
			
 *
*******************************************************************************/
uint8 TimeOutChkTenthSLong(uint16 *time ,uint16 timeOutVal)
{
  uint16 diftime=TenthSdata-*time;
    
  if(timeOutVal==0)
  {
    *time=TenthSdata;
    return 1;
  }
  else if(diftime>=timeOutVal)    //判断是否到时
  {
      *time+=diftime;
      return 1; 
  }
  else  return 0;
}
      
      
  
/*******************************************************************************
 * Function:    uint8 TimeOutChkHsLong(uint16 *time ,uint16 timeOutVal) 
 *
 * Description: 1s定时函数， *time 为定时的参数。timeOutVal为定时时间，为0则初始化*time
 *
 * Returns:     当到达时间时返回1， 且 *time初始化。否则为0
 *
 * Notes:    调用此函数查看定时是否到，考虑到实际情况，最大为65530 s
			
 *
*******************************************************************************/
uint8 TimeOutChkSLong(uint16 *time ,uint16 timeOutVal)
{
  uint16 diftime=Sdata-*time;
    
  if(timeOutVal==0)
  {
    *time=Sdata;
    return 1;
  }
  else if(diftime>=timeOutVal)    //判断是否到时
  {
      *time+=diftime;
      return 1; 
  }
  else  return 0;
}


/*******************************************************************************
 * Function:   uint16 Cal_MaxLoopTime(void)
 *
 * Description: 测试用，测试主循环的最大LOOp时间   
 *
 * Returns:    NONE
 *
 * Notes:    调试可 使用
			
 *
*******************************************************************************/
void Cal_MaxLoopTime(void)
{
   static   uint16   LastmSdata;
   uint16   shadow,CurLoopTime;
#if   CPU_8BIT  
   TimerCounterHalt();
#endif
   shadow= mSdata;
#if   CPU_8BIT  
   TimerCounterResume();
#endif
   
   CurLoopTime =shadow-LastmSdata;
   LastmSdata=mSdata;
   if(MaxLoopTime< CurLoopTime)
     MaxLoopTime =CurLoopTime;
}



  
/*******************************************************************************
 * Function:   MsCounterUpadat()  
 *
 * Description:每隔一定时间+1
 *
 * Returns:     noe
 *
 * Notes:     
			
 *
*******************************************************************************/


void MsCounterUpadat(void) 
{
  mSdata++;
}


void TenthSCounterUpadat(void) 
{
  if(TimeOutChkMsLong(&TenthSecTimer,100)) TenthSdata++;
}

void SCounterUpadat(void) 
{
  if(TimeOutChkMsLong(&SecTimer,1000))Sdata++;
}

