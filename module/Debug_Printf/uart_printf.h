/**
  ******************************************************************************
  * @file    uart_printf.h
  * @author  LL
  * @version V0.0.0
  * @date    2017-05-11
  * @brief   
  ******************************************************************************
  ******************************************************************************
***/



#ifndef __UART_PRINTF_H_
#define __UART_PRINTF_H_
#ifdef __cplusplus
 extern "C" {
#endif 

#define UART_PRINTF_DEBUG 1//使能打印宏定义

#if UART_PRINTF_DEBUG

#define UART_PRINTF_EN 1 //使能打印函数
#if UART_PRINTF_EN
extern void UART_Printf_NumberHEX(unsigned long int num);
extern void UART_Printf_NumberBCD(unsigned long int num);
extern void UART_Printf_NumberBCDNOCR(unsigned long int num);
extern void UART_Printf_Massages(char *buf);
extern void UART_Printf_MassagesNOCR(char *buf);
extern void UART_Printf_MassagesHEX(unsigned char *buf, unsigned int len);
#endif /* UART_PRINTF_EN */

#define DEBUG_PRINTFMASSAGES(buf)         UART_Printf_Massages(buf)
#define DEBUG_PRINTFMASSAGESNOCR(buf)     UART_Printf_MassagesNOCR(buf)
#define DEBUG_PRINTFMASSAGESHEX(buf,len)  UART_Printf_MassagesHEX(buf,len)
#define DEBUG_PRINTFNUMBERHEX(d)          UART_Printf_NumberHEX(d) 
#define DEBUG_PRINTFNUMBERBCD(d)          UART_Printf_NumberBCD(d)
#define DEBUG_PRINTFNUMBERBCDNOCR(d)      UART_Printf_NumberBCDNOCR(d)
#else
#define UART_PRINTF_EN 0 //禁止打印函数
#define DEBUG_PRINTFMASSAGES(buf) 
#define DEBUG_PRINTFMASSAGESNOCR(buf) 
#define DEBUG_PRINTFMASSAGESHEX(buf,len) 
#define DEBUG_PRINTFNUMBERHEX(d)
#define DEBUG_PRINTFNUMBERBCD(d)
#define DEBUG_PRINTFNUMBERBCDNOCR(d)
#endif /*UART_PRINTF_DEBUG*/

#ifdef __cplusplus
}
#endif 

#endif /* __UART_PRINTF_H_ */
