/**
  ******************************************************************************
  * @file    eeprom.h
  * @author  LL
  * @version V0.0.0
  * @date    2017-05-19
  * @brief   头文件
  ******************************************************************************
  ******************************************************************************
***/

#ifndef __EEPROM_H__
#define __EEPROM_H__

#define EEPROM_OK 0x01
#define EEPROM_ADDR_ERR 0x02 //地址错误 

extern void Eeprom_Read(uint16_t addr, uint8_t *buf, uint16_t len);
extern void Eeprom_Write(uint16_t addr, uint8_t *buf, uint16_t len);
extern void Eeprom_Test(void);

#endif /* __EEPROM_H__ */

/* ************ ****** ************ THE FILE END  ************ ****** ************ */