/**
  ******************************************************************************
  * @file    
  * @author  LL
  * @version V0.0.0
  * @date    2014-04-23
  * @brief   
  ******************************************************************************
  ******************************************************************************
***/

#ifndef __W25Q64_H__
#define __W25Q64_H__


#ifdef __cplusplus
 extern "C" {
#endif

#define W25Q_BUSY 0x01
#define W25Q_READOK 0x02
#define W25Q_ERASEOK 0x03
#define W25Q_ERASEERROR 0x04
#define W25Q_ADDRERROR 0x05
#define W25Q_WRITEDATAOK 0x06
#define W25Q_WRITEDATAERROR 0x07
#define W25Q_ERROR 0xFF


void W25q_Init(void);
void W25q_EraseTOP1MSapce(uint16_t sectornum,uint32_t addr);
void W25q_WriteTop1MSpace(uint8_t *buf, uint16_t len, uint32_t addr);
uint16_t W25q_ReadDeviceID(void);
uint8_t W25q_Read(uint8_t *buf, uint32_t len, uint32_t addr);
extern void W25q_Test(void);
#ifdef __cplusplus
}
#endif

#endif /* __W25q64_H__ */

/* ************ ****** ************ THE FILE END ************ ****** ************ */
