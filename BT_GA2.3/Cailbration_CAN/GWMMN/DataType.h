/*
 * 
//2015-8-23
ver 2.0
1 为了符合kea系列的mcu对uint16，uint32 进行了修改
符合 hcs08， mc， kea 

//2014-2-26
ver 1.0
1 建立版本
2 增加电机驱动宏定义

//2014-2-26
ver 1.0
1 建立版本
2 增加电机驱动宏定义

*/

#ifndef	__TYPE_DEFINITION__
#define	__TYPE_DEFINITION__ 
 
 
#define  uint8  unsigned char 
#define  uint16 unsigned short
#define	uint32	unsigned	long	

#define  int8   signed char
#define  int16  signed short
#define	int32		signed long


 
 //电机运行状态
#define  MOTOR_RUN_HALT    0X00
#define  MOTOR_RUN_FORWARD 0x01
#define  MOTOR_RUN_BACKWARD   0x02

#endif

 
 
