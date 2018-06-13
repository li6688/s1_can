/**
  ******************************************************************************
  * @file    
  * @author  ADT LL
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

#define W25Q64_BUSY 0x01
#define W25Q64_READOK 0x02
#define W25Q64_ERASEOK 0x03
#define W25Q64_ERASEERROR 0x04
#define W25Q64_ADDRERROR 0x05
#define W25Q64_WRITEDATAOK 0x06
#define W25Q64_WRITEDATAERROR 0x07
#define W25Q64_ERROR 0xFF


void W25q64_Init(void);
void W25q64_EraseTOP1MSapce(uint16_t sectornum,uint32_t addr);
void W25q64_WriteTop1MSpace(uint8_t *buf, uint16_t len, uint32_t addr);
uint16_t W25q64_ReadDeviceID(void);
uint8_t W25q64_Read(uint8_t *buf, uint32_t len, uint32_t addr);

uint32_t Picture_Location(uint8_t PictureNum);
uint32_t Get_PictureSize(uint8_t PictureNum);
void W25q64_ReadOneChar(uint8_t *buf, uint8_t FontSize, uint8_t FontType, uint16_t CharAddr);
void W25q64_ReadPicture(uint8_t *buf, uint32_t Size, uint8_t PictureNum, uint32_t Position);


#ifdef __cplusplus
}
#endif

#endif /* __W25q64_H__ */

/* ************ ****** ************ THE FILE END ************ ****** ************ */
