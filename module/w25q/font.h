/**
  ******************************************************************************
  * @file    font.h
  * @author  LL
  * @version V0.0.0
  * @date    2014-04-23
  * @brief   
  ******************************************************************************
  ******************************************************************************
***/

#ifndef __FONT_H__
#define __FONT_H__


#ifdef __cplusplus
 extern "C" {
#endif

/*字体类型定义*/
#define FONT_ASCII16     0x01 //ASCii 8*16
#define FONT_CHINESE16   0x02 //汉字   16*16
#define FONT_ASCII24     0x03 //ASCii 16*24
#define FONT_CHINESE24   0x04 //汉字   24*24
#define FONT_CHINESE32   0x05 //汉字   32*32

/*字体大小定义*/
#define FONT_SIZE16 16
#define FONT_SIZE24 24
#define FONT_SIZE32 32
#define FONT_SIZE48 48


extern uint16_t Font_CharSize(uint8_t FontType);
extern uint16_t Font_CharPos(uint8_t FontType, uint8_t *c);
extern uint8_t Font_CharType(uint8_t FontSize, uint8_t *c);
extern void Font_ReadOneChar(uint8_t *buf, uint8_t FontType, uint16_t CharCode);
#ifdef __cplusplus
}
#endif

#endif /* __FONT_H__ */

/* ************ ****** ************ THE FILE END ************ ****** ************ */
