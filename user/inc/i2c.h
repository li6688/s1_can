/**
  ******************************************************************************
  * @file    i2c.h
  * @author  LL
  * @version V0.0.0
  * @date    2014-10-20
  * @brief   Í·ÎÄ¼þ
  ******************************************************************************
  ******************************************************************************
***/

#ifndef __I2C_H__
#define __I2C_H__


#ifdef __cplusplus
 extern "C" {
#endif

#define I2C_OK       0
#define I2C_TIMEOUT  1

extern uint8_t I2C1_Read(uint8_t *pBuffer, uint16_t ReadAddr, uint8_t NumByteToRead);
extern void I2C1_Init(void); 

#ifdef __cplusplus
}
#endif


#endif /* __I2C_H__ */

/* ************ ****** ************ THE FILE END ************ ****** ************ */
