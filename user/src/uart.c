/**
******************************************************************************
* @file    uart.c
* @author  LL
* @version V0.0.0
* @date    2014-04-23
* @brief   
******************************************************************************
******************************************************************************
***/
#define __UART_C__

#include "config.h"

#define UART_BRGVAL (((FP/GlobalVar.UartBaudRate)/4)-1)


//485方向控制 RB4
#define DE485_TRI TRISBbits.TRISB4
#define DE485_PORT LATBbits.LATB4
#define DE485_ANS  ANSELBbits.ANSB4
#define DE485_OUT  {DE485_ANS = 0; DE485_TRI = 0; NOP();}
#define DE485_SET  {DE485_PORT = 1;}
#define DE485_CLR  {DE485_PORT = 0;}

#define UART_PRINTFDATETIME_EN 0

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
	
    DE485_SET;
    
    TX1REG = data; 
    while(0==TX1STAbits.TRMT);
    
    DE485_CLR;	
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
	if (TRUE==Flag.ReciveOK) //接收到一帧数据
    {
        Modbus_DataDecode_RTU(pModbus_DataPort1->DataCodeBuff,pModbus_DataPort1->len);
		GlobalVar.PortConnectTime = 0;
        Flag.ReciveOK = FALSE;
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
* Function Name  : UART2_Init 
* Description    : 串口初始化，设置IO口，波特率
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-03-16
* Author         : LL
*******************************************************************************/
static void UART2_Init(void)
{  
    //配置端口映射，串口
    RXPPSbits.RXPPS = 0x0D;//输入RB5
    RB6PPSbits.RB6PPS = 0x14;//输出为RB6
  
	//串口
    //关闭引脚的模拟功能
    ANSELBbits.ANSB5 = 0;
    ANSELBbits.ANSB6 = 0;
    //配置引脚的方向
    TRISBbits.TRISB5 = 1;
    TRISBbits.TRISB6 = 0;

	RC1STAbits.SPEN = 0;//关闭串口

    SP1BRGH = (UART_BRGVAL>>8) & 0xFF; // Baud Rate setting
    SP1BRGL = UART_BRGVAL & 0xFF;
    
    TX1STAbits.TX9 = 0; //8-Data bits
    TX1STAbits.SYNC = 0;//异步模式
    TX1STAbits.BRGH = 1;//低速,和BRG16一起，决定波特率计算公式
    TX1STAbits.TXEN = 1;//使能发送
    
    RC1STAbits.RX9 = 0;//接收8bit
    RC1STAbits.ADDEN = 0;//禁止地址监测
    RC1STAbits.CREN = 1;//使能连续接收
    
    BAUD1CONbits.SCKP = 0;//发送脚空闲时为高电平
    BAUD1CONbits.BRG16 = 1;//16位波特率发生器，和BRGH一起，决定波特率计算公式
    BAUD1CONbits.ABDEN = 0;//禁止自动波特率监测
   
    RC1STAbits.SPEN = 1;//使能串口
    
    PIR1bits.RCIF = 0;//清楚标志
    PIE1bits.RCIE = 1;//shi能中断
    
    //485 方向引脚
    DE485_OUT;
    DE485_CLR;
    
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
	uint8_t crcx;
    uint16_t i;
	uint16_t crc16x;
	
	for (i=0; i<len; i++)
	{
		UART_PutChar(buf[i]);  
	}
	crc16x = CRC_16(buf,len);
	crcx = crc16x & 0xFF; 
	UART_PutChar(crcx); 
	crcx = (crc16x>>8) & 0xFF; 
	UART_PutChar(crcx); 
    //DEBUG_PRINTFNUMBERHEX(crc16x);
}

/*******************************************************************************
* Function Name  : UART_Init 
* Description    : 串口初始化,串口对外调用接口
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-03-16
* Author         : LL
*******************************************************************************/
void UART_Init(void)
{
	UART2_Init();
    Modbus_Init();
   
	DEBUG_PRINTFMASSAGES("UART OK");
    DEBUG_PRINTFMASSAGES(__DATE__);
    DEBUG_PRINTFMASSAGES(__TIME__);
#if UART_PRINTFDATETIME_EN
	UART_PrintfDateTime();
#endif /* UART_PRINTFDATETIME_EN */
}

/*******************************************************************************
* Function Name  : USART_Process
* Description    : 串口处理
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
