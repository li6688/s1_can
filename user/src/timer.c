/**
******************************************************************************
* @file    timer.c
* @author  LL
* @version V0.0.0
* @date    2016-03-21
* @brief   
******************************************************************************
******************************************************************************
***/

#define __TIMER_C__
#include "config.h"

#define TIME0_EN 0
#if TIME0_EN
/*******************************************************************************
* Function Name  : Timer0_Init 
* Description    : 初始化定时器，中断周期1S
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-03-21
* Author         : LL
*******************************************************************************/
static void Timer0_Init(void) 
{
	TMR0H = 121;
    T0CON0bits.T016BIT = 0;//8位定时器
    T0CON0bits.T0OUTPS = 0x00;//输出后1:1

	T0CON1bits.T0CS = 4;//时钟源，内部31K
	T0CON1bits.T0ASYNC = 1;//异步输入
	T0CON1bits.T0CKPS = 0x08;//F;//预分频1:32786
    T0CON0bits.T0EN = 0;//禁止定时器
    
    PIR0bits.TMR0IF = 0;//清楚标志
    PIE0bits.TMR0IE = 1;//使能中断
  
}
#endif

/*******************************************************************************
* Function Name  : Timer4_Init 
* Description    : 初始化定时器，中断周期1mS
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-03-21
* Author         : LL
*******************************************************************************/
static void Timer4_Init(void) 
{
    T4CONbits.T4CKPS = 0x03;//64分频
    T4CONbits.T4OUTPS = 0x00;//输出后1:1
    PR4 = 125;
    T4CONbits.TMR4ON = 0x01;//使能定时器
    
    PIR2bits.TMR4IF = 0;//清楚标志
    PIE2bits.TMR4IE = 1;//使能中断
  
}

/*******************************************************************************
* Function Name  : Timer2_Init 
* Description    : 初始化定时器，中断周期10mS
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-03-21
* Author         : LL
*******************************************************************************/
static void Timer2_Init(void) 
{
    T2CONbits.T2CKPS = 0x03;//64分频
    T2CONbits.T2OUTPS = 0x09;//输出后1:10
    PR2 = 31;//125;
    T2CONbits.TMR2ON = 0x01;//使能定时器
    PIR1bits.TMR2IF = 0;//清楚标志
    PIE1bits.TMR2IE = 1;//使能中断
}

/*******************************************************************************
* Function Name  : Timer_Init 
* Description    : 初始化定时器
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-03-21
* Author         : LL
*******************************************************************************/
void Timer_Init(void)
{
	Timer2_Init();
    Timer4_Init();
	#if TIME0_EN
	Timer0_Init();
	#endif
}

/* ************ ****** ************ THE FILE END  ************ ****** ************ */