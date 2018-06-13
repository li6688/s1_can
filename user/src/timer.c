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
* Description    : ��ʼ����ʱ�����ж�����1S
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-03-21
* Author         : LL
*******************************************************************************/
static void Timer0_Init(void) 
{
	TMR0H = 121;
    T0CON0bits.T016BIT = 0;//8λ��ʱ��
    T0CON0bits.T0OUTPS = 0x00;//�����1:1

	T0CON1bits.T0CS = 4;//ʱ��Դ���ڲ�31K
	T0CON1bits.T0ASYNC = 1;//�첽����
	T0CON1bits.T0CKPS = 0x08;//F;//Ԥ��Ƶ1:32786
    T0CON0bits.T0EN = 0;//��ֹ��ʱ��
    
    PIR0bits.TMR0IF = 0;//�����־
    PIE0bits.TMR0IE = 1;//ʹ���ж�
  
}
#endif

/*******************************************************************************
* Function Name  : Timer4_Init 
* Description    : ��ʼ����ʱ�����ж�����1mS
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-03-21
* Author         : LL
*******************************************************************************/
static void Timer4_Init(void) 
{
    T4CONbits.T4CKPS = 0x03;//64��Ƶ
    T4CONbits.T4OUTPS = 0x00;//�����1:1
    PR4 = 125;
    T4CONbits.TMR4ON = 0x01;//ʹ�ܶ�ʱ��
    
    PIR2bits.TMR4IF = 0;//�����־
    PIE2bits.TMR4IE = 1;//ʹ���ж�
  
}

/*******************************************************************************
* Function Name  : Timer2_Init 
* Description    : ��ʼ����ʱ�����ж�����10mS
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-03-21
* Author         : LL
*******************************************************************************/
static void Timer2_Init(void) 
{
    T2CONbits.T2CKPS = 0x03;//64��Ƶ
    T2CONbits.T2OUTPS = 0x09;//�����1:10
    PR2 = 31;//125;
    T2CONbits.TMR2ON = 0x01;//ʹ�ܶ�ʱ��
    PIR1bits.TMR2IF = 0;//�����־
    PIE1bits.TMR2IE = 1;//ʹ���ж�
}

/*******************************************************************************
* Function Name  : Timer_Init 
* Description    : ��ʼ����ʱ��
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