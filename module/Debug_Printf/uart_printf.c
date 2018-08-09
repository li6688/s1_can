/**
  ******************************************************************************
  * @file    uart_printf.c
  * @author  LL
  * @version V0.0.0
  * @date    2017-05-11
  * @brief   
  ******************************************************************************
  ******************************************************************************
***/
#define __UART_PRINTF_C__

extern unsigned char UART_PutChar( unsigned char c);

#include <stdint.h>
#include <stdbool.h>
#include "uart_printf.h"
//#include "debug_printf_sys.h"
#include "sys.h"

#if UART_PRINTF_EN

/*******************************************************************************
* Function Name  : UART_Printf_PutChar
* Description    : 打印一个字符
* Input          : c:要打印的字符         
* Output         : None
* Return         : None
* Date           : 2017-05-11
* Author         : LL
*******************************************************************************/
static void UART_Printf_PutChar(char c)
{
   UART_PutChar((unsigned char)c); 
}

/*******************************************************************************
* Function Name  : UART_Printf_Massages
* Description    : 打印信息到缓冲区,自动加回车换行
                   例如：UART_Printf_Massages("this is printfchar");
                         UART_Printf_Massages("");打印一个回车换行。
* Input          : buf:要打印的信息          
* Output         : None
* Return         : None
* Date           : 2017-05-11
* Author         : LL
*******************************************************************************/
void UART_Printf_Massages(char *buf)
{
  while (*buf)
  {
	  UART_Printf_PutChar(*buf);
	  buf++;
  }
	UART_Printf_PutChar(0x0D);
	UART_Printf_PutChar(0x0A);
}

/*******************************************************************************
* Function Name  : UART_Printf_MassagesNOCR
* Description    : 打印信息到缓冲区，不加回车换行
                   例如：UART_Printf_Massages("this is printfchar");
* Input          : buf:要打印的信息          
* Output         : None
* Return         : None
* Date           : 2017-05-11
* Author         : LL
*******************************************************************************/
void UART_Printf_MassagesNOCR(char *buf)
{
  while (*buf)
  {
	  UART_Printf_PutChar(*buf);
	  buf++;
  }
}

/*******************************************************************************
* Function Name  : UART_Printf_MassagesHEX
* Description    : 将数据信息转化为16进制打印，自动加回车换行           
* Input          : buf:要打印的信息 
                   len:信息长度
* Output         : None
* Return         : None
* Date           : 2017-05-11
* Author         : LL
*******************************************************************************/
void UART_Printf_MassagesHEX(unsigned char *buf, unsigned int len) 
{
    unsigned char x;
    unsigned int i;
    for (i = 0; i < len; i++) 
    {
        x = (unsigned char)((buf[i] >> 4) & 0x0F);
        x = HexToAsciiTab[x];
        UART_Printf_PutChar(x);
        x = (unsigned char)(buf[i] & 0x0F);
        x = HexToAsciiTab[x];
        UART_Printf_PutChar(x);
        UART_Printf_PutChar(0x20); //加空格
    }
    UART_Printf_PutChar(0x0D);
    UART_Printf_PutChar(0x0A);
}

/*******************************************************************************
* Function Name  : UART_Printf_NumberHEX
* Description    : 将数据以十六进制字符发送，自动加回车换行                 
* Input          : buf:要打印的信息          
* Output         : None
* Return         : None
* Date           : 2017-05-11
* Author         : LL
*******************************************************************************/
void UART_Printf_NumberHEX(unsigned long int num)
{
  unsigned char buf[11];
  NumberToAsciiHEX(buf,num,0xFF);
  UART_Printf_Massages((char *)buf);
}

/*******************************************************************************
* Function Name  : UART_Printf_NumberBCD
* Description    : 将数据以十进制字符发送，自动加回车换行               
* Input          : num:要打印的数值          
* Output         : None
* Return         : None
* Date           : 2017-05-11
* Author         : LL
*******************************************************************************/
void UART_Printf_NumberBCD(unsigned long int num)
{
  unsigned char buf[11];
  NumberToAscii(buf,num,0xFF);
  UART_Printf_Massages((char *)buf);
}

/*******************************************************************************
* Function Name  : UART_Printf_NumberBCDNOCR
* Description    : 将数据以十进制字符发送，自动加逗号             
* Input          : num:要打印的数值          
* Output         : None
* Return         : None
* Date           : 2017-05-11
* Author         : LL
*******************************************************************************/
void UART_Printf_NumberBCDNOCR(unsigned long int num)
{
  unsigned char buf[11];
  NumberToAscii(buf,num,0xFF);
  UART_Printf_MassagesNOCR((char *)buf);
  UART_Printf_PutChar(',');
}
#endif //UART_PRINTF_EN

/* ************ ****** ************ THE FILE END  ************ ****** ************ */				