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

#define HEXTOASCIITAB_EN 1 //��Щ���������û��ʹ�õ�ȫ�ֱ������棬��ʹ�õ�ʱ���ֹ������ȥ������

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
