/**
******************************************************************************
* @file    ad7790.h
* @author  LL
* @version V0.0.0
* @date    2017-07-28
* @brief   Í·ÎÄ¼þ  
******************************************************************************
******************************************************************************
***/


#ifndef __AD7790_H_
#define __AD7790_H_

extern void AD7790_Reset(void);
extern void AD7790_WriteCR(uint8_t data);
extern uint8_t AD7790_ReadSR(void);
extern uint8_t AD7790_ReadWriteMR(uint8_t m);
extern uint8_t AD7790_ReadWriteFR(uint8_t f);
extern uint16_t AD7790_ReadDR(void);
extern uint16_t AD7790_ConverStable(void);
extern void AD7790_Init(void);
extern void AD7790_Process(void);


#endif /* __AD7790_H_ */
