/**
  ******************************************************************************
  * @file    systick.h
  * @author  LL
  * @version V0.0.0
  * @date    2018-03-27
  * @brief   ͷ�ļ�
  ******************************************************************************
  ******************************************************************************
***/

#ifndef __SYSTICK_H__
#define __SYSTICK_H__

extern void SysTick_Configuration(uint32_t value);
extern uint32_t Read_UserTimer(uint32_t time);
extern void Reset_UserTimer(uint32_t *time);
extern void Delay_10ms(uint32_t value);


#endif /*__SYSTICK_H__*/

/* ************ ****** ************ THE FILE END ************ ****** ************ */
