/**
  ******************************************************************************
  * @file    adc.h
  * @author  LL
  * @version V0.0.0
  * @date    2014-04-23
  * @brief   
  ******************************************************************************
  ******************************************************************************
***/

#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
  extern "C" {
#endif


extern uint16_t ADC_ConverStable(uint8_t channel);
extern void ADC_Init(void);
extern void ADC_Process(void);

#ifdef __cplusplus
  }
#endif

#endif /* __ADC_H__ */

/* ************ ****** ************ THE FILE END  ************ ****** ************ */
