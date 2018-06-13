/**
  ******************************************************************************
  * @file    crc16.h
  * @author  LL
  * @version V0.0.0
  * @date    2016-11-16
  * @brief   
  ******************************************************************************
  ******************************************************************************
***/



#ifndef __CRC16_H_
#define __CRC16_H_

extern uint16_t CRC_16(uint8_t *puchMsg,uint16_t usDataLen);
extern uint16_t CRC_16ALL_Len1Byte(uint8_t *puchMsg,uint8_t usDataLen); 
extern uint16_t CRC_16ALL_Len2Byte(uint8_t *puchMsg,uint16_t usDataLen);

#endif /* __CRC16_H_ */
