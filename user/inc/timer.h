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
#ifndef __TIMER_H__
#define __TIMER_H__

#define TIMER_SAPMLE_EN T0CON0bits.T0EN=1 //ʹ�ܶ�ʱ��
#define TIMER_SAPMLE_DI T0CON0bits.T0EN=0 //��ֹ��ʱ��

void Timer_Init(void);

#endif /* __TIMER_H__ */

/* ************ ****** ************ THE FILE END  ************ ****** ************ */