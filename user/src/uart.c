/**
******************************************************************************
* @file    uart.c
* @author  LL
* @version V0.0.1
* @date    2018-06-22
* @brief   本软件使用与STM32F4系列
******************************************************************************
******************************************************************************
***/
#define __UART_C__

#include "stm32f4xx.h"
#include "main.h"
#include "uart_printf.h"
#include "fifo.h"
#include "lis3dsh.h"

#define UART_PRINTFDATETIME_EN 1

/*******************************************************************************
* Function Name  : UART3_PutChar
* Description    : 通过串口发送一个数据
* Input          : data:要发送的数据
* Output         : None
* Return         : None
* Date           : 2017-05-02
* Author         : LL
*******************************************************************************/
void UART3_PutChar(uint8_t data)
{
    while (USART_GetFlagStatus(USART3,USART_FLAG_TXE) != SET);
    USART3->DR = data; 
}

/*******************************************************************************
* Function Name  : put_char
* Description    : 通过串口发送一个数据
* Input          : data:要发送的数据
* Output         : None
* Return         : None
* Date           : 2017-05-02
* Author         : LL
*******************************************************************************/
void UART_PutChar(uint8_t data)
{
	UART3_PutChar(data);
}

/*******************************************************************************
* Function Name  : Recive_DataProcess
* Description    : 串口接收数据处理
* Input          : None
* Output         : None
* Return         : None
* Date           : 2014-05-14
* Author         : LL
*******************************************************************************/
static void UART_ReciveDataProcess(void)
{
    uint8_t d;

    if(FIFO_GetCount(pRx))
    {
        FIFO_GetOne(pRx, &d);
        UART_PutChar(d);
        switch (d)
        {
            case 't':
                Lis3dsh_Test();
                break;
            case 'r':
                Lis3dsh_ReadAccData();
                break;
            
        }
    }
	
}
#if UART_PRINTFDATETIME_EN
/*******************************************************************************
* Function Name  : UART_PrintfDateTime
* Description    : 串口打印日期和时间
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
* Function Name  : USART3_IRQHandler 
* Description    : 中断函数
* Input          : None
* Output         : None
* Return         : None
* Date           : 2018-06-23
* Author         : LL
*******************************************************************************/
void USART3_IRQHandler(void)
{
    uint8_t d;
    if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET)
    {
        d = USART3->DR;
        FIFO_PutOne(pRx, d);
    }
}


/*******************************************************************************
* Function Name  : UART3_Init 
* Description    : 串口初始化，波特率
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-03-16
* Author         : LL
*******************************************************************************/
static void UART3_Init(void)
{  
    USART_InitTypeDef USART_InitStructure;
  
    /* USARTx configuration ------------------------------------------------------*/
    /* USARTx configured as follows:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
    */
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    /* USART configuration */
    USART_Init(USART3, &USART_InitStructure);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//使能接收中断
    USART_Cmd(USART3, ENABLE);
    
}

/*******************************************************************************
* Function Name  : UART_PrintfString
* Description    : 通过串口发送一个字符串
 * 使用方法UART_PrintfString("123");
* Input          : buf:要发送的信息
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
* Description    : 将要发送到串口的数据打包处理，并写入发送缓冲区
 * 把要发送的信息，计算CRC校验，并发送CRC校验。
* Input          : 
 * buf:要发送的信息
 * len:信息长度
* Output         : None
* Return         : None
* Date           : 2014-08-26
* Author         : LL
*******************************************************************************/
void UART_PrintfNormal(uint8_t *buf, uint16_t len)
{	
	
}

/*******************************************************************************
* Function Name  : UART_Configuration 
* Description    : 串口初始化,串口对外调用接口
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-03-16
* Author         : LL
*******************************************************************************/
void UART_Configuration(void)
{
	UART3_Init();
    FIFO_Init(pRx, RX_SIZE, Rx_Buffer);
    DEBUG_PRINTFMASSAGES("UART OK");
#if UART_PRINTFDATETIME_EN
	UART_PrintfDateTime();
#endif /* UART_PRINTFDATETIME_EN */
}

/*******************************************************************************
* Function Name  : UART_Process
* Description    : 串口处理
* Input          : None
* Output         : None
* Return         : None
* Date           : 2014-10-23
* Author         : LL
*******************************************************************************/
void UART_Process(void)
{
	UART_ReciveDataProcess();
}
/* ************ ****** ************ THE FILE END  ************ ****** ************ */				
