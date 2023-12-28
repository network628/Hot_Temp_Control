/******************************************************************************
                                閺呰桨淇婇悽闈涚摍缁夋垶濡?
******************************************************************************/
   
#ifndef     __CONFIG_H__
#define     __CONFIG_H__


#ifndef TRUE
# define TRUE       1
#endif

#ifndef FALSE
# define FALSE      0
#endif

#ifndef NULL
# define NULL       0
#endif
 
typedef unsigned char   U8;
typedef signed   char   S8;
typedef unsigned int    U16;
typedef signed   int    S16;
typedef unsigned long   U32;
typedef signed   long   S32;

#define uint8_t unsigned char
#define u8 unsigned char
#define u16 unsigned short
#define uint16_t unsigned short
#define u32 unsigned int

#include <stdio.h>
#include <intrins.h>
#include"STC15W.h"
#include "Uart.H"
#include "STCEEPROM.H"
#include "PWM.H"
#include "LED3.H"
#include "NTC.H"
#include "pid.h"
#include "timer.h"
#include "key.h"

extern struct NTC{

// 	U16 Hour,Min,Second;
    U16 T2_CNT;		 
	float Set_T;     
	float Now_T;	 
 	short Cool_PWM;
	
//	U16 Tick;
}ntc;

extern struct LIGNT{
	U8 sw_flag;	        //开关变量 1://调温模式   0://最大档 模式
	// U16 L_value;	    //0-255	   亮度值变量
	U16 AC_Voltage;	    //交流电压 
    U16 counter;		//加热4秒停11秒   
	U8 timer0_interrupt;
	U16 Delay_Open_Cnt;
	U16 Display_Cnt; //5秒前显示设置温度，5秒后显示当前温度
}Light;

extern U16 disp_num;

sbit lamp_pwm = P3^4;   

sbit key_up   = P5^4;  
sbit key_down = P5^5;  
sbit key_sw   = P3^2;  

#endif // end of __CONFIG_H__











