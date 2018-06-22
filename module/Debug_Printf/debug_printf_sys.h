/**
  ******************************************************************************
  * @file    debug_printf_sys.h
  * @author  LL
  * @version V0.0.0
  * @date    2014-04-23
  * @brief   
  ******************************************************************************
  ******************************************************************************
***/

#ifndef __DEBUG_PRINTF_SYS_H__
#define __DEBUG_PRINTF_SYS_H__

#ifdef __cplusplus
  extern "C" {
#endif

#define HEXTOASCIITAB_EN 1 //有些编译器会对没有使用的全局变量警告，不使用的时候禁止，可以去掉警告

#if HEXTOASCIITAB_EN
extern unsigned char HexToAsciiTab[];
#endif

extern  Flag_StructType Flag;
extern  GlobalVariable_StructType GlobalVar;

extern void NumberToAscii(uint8_t *buf, uint32_t number, uint8_t len);
extern void NumberToAsciiHEX(uint8_t *buf, uint32_t number, uint8_t len);



#ifdef __cplusplus
  }
#endif

#endif /* __DEBUG_PRINTF_SYS_H__ */

/* ************ ****** ************ THE FILE END  ************ ****** ************ */
