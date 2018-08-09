/**
******************************************************************************
* @file    uart.c
* @author  LL
* @version V0.0.1
* @date    2018-06-22
* @brief   �����ʹ����STM32F4ϵ��
******************************************************************************
******************************************************************************
***/
#define __UART_C__

#include "stm32f4xx.h"
#include "main.h"


#define UART_PRINTFDATETIME_EN 0

/*******************************************************************************
* Function Name  : UART2_PutChar
* Description    : ͨ�����ڷ���һ������
* Input          : data:Ҫ���͵�����
* Output         : None
* Return         : None
* Date           : 2017-05-02
* Author         : LL
*******************************************************************************/
void UART2_PutChar(uint8_t data)
{
    while (USART_GetFlagStatus(USART2,USART_FLAG_TXE) != SET);
	USART2->DR = data;
    
}

/*******************************************************************************
* Function Name  : put_char
* Description    : ͨ�����ڷ���һ������
* Input          : data:Ҫ���͵�����
* Output         : None
* Return         : None
* Date           : 2017-05-02
* Author         : LL
*******************************************************************************/
void UART_PutChar(uint8_t data)
{
	UART2_PutChar(data);
}

/*******************************************************************************
* Function Name  : Recive_DataProcess
* Description    : ���ڽ������ݴ���
* Input          : None
* Output         : None
* Return         : None
* Date           : 2014-05-14
* Author         : LL
*******************************************************************************/
static void UART_ReciveDataProcess(void)
{
	
}
#if UART_PRINTFDATETIME_EN
/*******************************************************************************
* Function Name  : UART_PrintfDateTime
* Description    : ���ڴ�ӡ���ں�ʱ��
* Input          : None
* Output         : None
* Return         : None
* Date           : 207-03-10
* Author         : LL
*******************************************************************************/
static void UART_PrintfDateTime(void)
{	
	uint8_t i;
	uint8_t *datestr = (uint8_t *)__DATE__;
	uint8_t *timestr = (uint8_t *)__TIME__;
	for(i = 0; i < 11; i++)
	{
		UART_PutChar(datestr[i]);
	}
	UART_PutChar('-');
	UART_PutChar('-');
	for(i = 0; i < 8; i++)
	{
		UART_PutChar(timestr[i]);
	}
	UART_PutChar(0x0D);UART_PutChar(0x0A);
}
#endif /* UART_PRINTFDATETIME_EN */

/*******************************************************************************
* Function Name  : UART2_Init 
* Description    : ���ڳ�ʼ��������IO�ڣ�������
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-03-16
* Author         : LL
*******************************************************************************/
static void UART2_Init(void)
{  
    USART_InitTypeDef USART_InitStructure;
  
    /* USARTx configuration ------------------------------------------------------*/
    /* USARTx configured as follows:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - Two Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
    */
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_2;
    USART_InitStructure.USART_Parity = USART_Parity_Odd;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    /* USART configuration */
    USART_Init(USART2, USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//ʹ�ܽ����ж�
    USART_Cmd(USART2, ENABLE);
    
}

/*******************************************************************************
* Function Name  : UART_PrintfString
* Description    : ͨ�����ڷ���һ���ַ���
 * ʹ�÷���UART_PrintfString("123");
* Input          : buf:Ҫ���͵���Ϣ
* Output         : None
* Return         : None
* Date           : 2017-03-29
* Author         : LL
*******************************************************************************/
void UART_PrintfString(uint8_t *buf)
{
	while (*buf)
	{
		UART_PutChar(*buf);
		buf++;
	}
	UART_PutChar(0x0D);
	UART_PutChar(0x0A);
}

/*******************************************************************************
* Function Name  : UART_PrintfNormal
* Description    : ��Ҫ���͵����ڵ����ݴ��������д�뷢�ͻ�����
 * ��Ҫ���͵���Ϣ������CRCУ�飬������CRCУ�顣
* Input          : 
 * buf:Ҫ���͵���Ϣ
 * len:��Ϣ����
* Output         : None
* Return         : None
* Date           : 2014-08-26
* Author         : LL
*******************************************************************************/
void UART_PrintfNormal(uint8_t *buf, uint16_t len)
{	
	
}

/*******************************************************************************
* Function Name  : USART_Configuration 
* Description    : ���ڳ�ʼ��,���ڶ�����ýӿ�
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-03-16
* Author         : LL
*******************************************************************************/
void USART_Configuration(void)
{
	UART2_Init();
  
#if UART_PRINTFDATETIME_EN
	UART_PrintfDateTime();
#endif /* UART_PRINTFDATETIME_EN */
}

/*******************************************************************************
* Function Name  : USART_Process
* Description    : ���ڴ���
* Input          : None
* Output         : None
* Return         : None
* Date           : 2014-10-23
* Author         : LL
*******************************************************************************/
void USART_Process(void)
{
	UART_ReciveDataProcess();
}
/* ************ ****** ************ THE FILE END  ************ ****** ************ */				
