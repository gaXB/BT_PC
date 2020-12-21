

/***********************************************************************
�汾�嵥
ver0.0.4
2015-8-23
1 ���� CPU_8BIT �ĺ궨�� ���ڲ���8λ��mcu�в���Ҫͣ�ж�

ver0.0.3
2014-8-14
1 uint16 Cal_MaxLoopTime(void) ��Ϊ    void Cal_MaxLoopTime(void) 
2 MaxLoopTime ��Ϊ volatile����

ver0.0.2
2014-2-27
1 ���ָ��� timeout.c ��Ϊtimerout.c
2 ����uint16 Cal_MaxLoopTime(void)


ver0.0.1
2013-4-12
1 �޸ı�ע��
2 ������hSdata ��Ϊ TenthSdata

ver0.0.0
2012-11-10
1��ʼ�汾ȷ��

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

static unsigned short mSdata;         //ÿ1ms��1

static uint16 TenthSdata;       //ÿ0.1s��1

static uint16 Sdata;          //ÿ1s��1



static uint16 TenthSecTimer,SecTimer;
 
volatile uint16 MaxLoopTime;  
 
 /***********************************
 ͨ��Ӳ��������Ҫ���õĺ�������
 ************************************/
 //��ʱ��ģ��
#if   CPU_8BIT
void  TimerCounterHalt(void) ;
void  TimerCounterResume(void) ;
#endif



/*******************************************************************************
 * Function:    uint8 TimeOutChkMsLong(uint16 *time ,uint16 timeOutVal) 
 *
 * Description: ms��ʱ������ *time Ϊ��ʱ�Ĳ�����timeOutValΪ��ʱʱ�䣬Ϊ0���ʼ��*time
 *
 * Returns:     ������ʱ��ʱ����1�� �� *time��ʼ��������Ϊ0
 *
 * Notes:    ���ô˺����鿴��ʱ�Ƿ񵽣����ǵ�ʵ����������Ϊ65525ms
			
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
  else if(diftime>=timeOutVal)    //�ж��Ƿ�ʱ
  {
      *time+=diftime;
      return 1; 
  }
  else  return 0;
}
  
/*******************************************************************************
 * Function:    uint8 TimeOutChkHsLong(uint16 *time ,uint16 timeOutVal) 
 *
 * Description: 0.1s��ʱ������ *time Ϊ��ʱ�Ĳ�����timeOutValΪ��ʱʱ�䣬Ϊ0���ʼ��*time
 *
 * Returns:     ������ʱ��ʱ����1�� �� *time��ʼ��������Ϊ0
 *
 * Notes:    ���ô˺����鿴��ʱ�Ƿ񵽣����ǵ�ʵ����������Ϊ65530/2 s
			
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
  else if(diftime>=timeOutVal)    //�ж��Ƿ�ʱ
  {
      *time+=diftime;
      return 1; 
  }
  else  return 0;
}
      
      
  
/*******************************************************************************
 * Function:    uint8 TimeOutChkHsLong(uint16 *time ,uint16 timeOutVal) 
 *
 * Description: 1s��ʱ������ *time Ϊ��ʱ�Ĳ�����timeOutValΪ��ʱʱ�䣬Ϊ0���ʼ��*time
 *
 * Returns:     ������ʱ��ʱ����1�� �� *time��ʼ��������Ϊ0
 *
 * Notes:    ���ô˺����鿴��ʱ�Ƿ񵽣����ǵ�ʵ����������Ϊ65530 s
			
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
  else if(diftime>=timeOutVal)    //�ж��Ƿ�ʱ
  {
      *time+=diftime;
      return 1; 
  }
  else  return 0;
}


/*******************************************************************************
 * Function:   uint16 Cal_MaxLoopTime(void)
 *
 * Description: �����ã�������ѭ�������LOOpʱ��   
 *
 * Returns:    NONE
 *
 * Notes:    ���Կ� ʹ��
			
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
 * Description:ÿ��һ��ʱ��+1
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

