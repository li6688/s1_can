/**
  ******************************************************************************
  * @file    lis3dsh.h
  * @author  LL
  * @version V0.0.0
  * @date    2018-06-23
  * @brief   
  ******************************************************************************
  ******************************************************************************
***/

#ifndef __LIS3DSH_H__
#define __LIS3DSH_H__


#ifdef __cplusplus
 extern "C" {
#endif

void Lis3dsh_Init(void);
uint8_t Lis3dsh_Write(uint8_t *buf, uint8_t len, uint8_t addr);
uint8_t Lis3dsh_Read(uint8_t *buf, uint8_t len, uint8_t addr);
extern uint8_t Lis3dsh_ReadAccData(void);
extern void Lis3dsh_Test(void);
#ifdef __cplusplus
}
#endif

#endif /* __LIS3DSH_H__ */

/* ************ ****** ************ THE FILE END ************ ****** ************ */
