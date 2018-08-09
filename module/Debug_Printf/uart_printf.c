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
* Description    : ��ӡһ���ַ�
* Input          : c:Ҫ��ӡ���ַ�         
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
* Description    : ��ӡ��Ϣ��������,�Զ��ӻس�����
                   ���磺UART_Printf_Massages("this is printfchar");
                         UART_Printf_Massages("");��ӡһ���س����С�
* Input          : buf:Ҫ��ӡ����Ϣ          
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
* Description    : ��ӡ��Ϣ�������������ӻس�����
                   ���磺UART_Printf_Massages("this is printfchar");
* Input          : buf:Ҫ��ӡ����Ϣ          
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
* Description    : ��������Ϣת��Ϊ16���ƴ�ӡ���Զ��ӻس�����           
* Input          : buf:Ҫ��ӡ����Ϣ 
                   len:��Ϣ����
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
        UART_Printf_PutChar(0x20); //�ӿո�
    }
    UART_Printf_PutChar(0x0D);
    UART_Printf_PutChar(0x0A);
}

/*******************************************************************************
* Function Name  : UART_Printf_NumberHEX
* Description    : ��������ʮ�������ַ����ͣ��Զ��ӻس�����                 
* Input          : buf:Ҫ��ӡ����Ϣ          
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
* Description    : ��������ʮ�����ַ����ͣ��Զ��ӻس�����               
* Input          : num:Ҫ��ӡ����ֵ          
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
* Description    : ��������ʮ�����ַ����ͣ��Զ��Ӷ���             
* Input          : num:Ҫ��ӡ����ֵ          
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