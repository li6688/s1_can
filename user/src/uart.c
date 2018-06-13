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


//485������� RB4
#define DE485_TRI TRISBbits.TRISB4
#define DE485_PORT LATBbits.LATB4
#define DE485_ANS  ANSELBbits.ANSB4
#define DE485_OUT  {DE485_ANS = 0; DE485_TRI = 0; NOP();}
#define DE485_SET  {DE485_PORT = 1;}
#define DE485_CLR  {DE485_PORT = 0;}

#define UART_PRINTFDATETIME_EN 0

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
	
    DE485_SET;
    
    TX1REG = data; 
    while(0==TX1STAbits.TRMT);
    
    DE485_CLR;	
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
	if (TRUE==Flag.ReciveOK) //���յ�һ֡����
    {
        Modbus_DataDecode_RTU(pModbus_DataPort1->DataCodeBuff,pModbus_DataPort1->len);
		GlobalVar.PortConnectTime = 0;
        Flag.ReciveOK = FALSE;
    }
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
    //���ö˿�ӳ�䣬����
    RXPPSbits.RXPPS = 0x0D;//����RB5
    RB6PPSbits.RB6PPS = 0x14;//���ΪRB6
  
	//����
    //�ر����ŵ�ģ�⹦��
    ANSELBbits.ANSB5 = 0;
    ANSELBbits.ANSB6 = 0;
    //�������ŵķ���
    TRISBbits.TRISB5 = 1;
    TRISBbits.TRISB6 = 0;

	RC1STAbits.SPEN = 0;//�رմ���

    SP1BRGH = (UART_BRGVAL>>8) & 0xFF; // Baud Rate setting
    SP1BRGL = UART_BRGVAL & 0xFF;
    
    TX1STAbits.TX9 = 0; //8-Data bits
    TX1STAbits.SYNC = 0;//�첽ģʽ
    TX1STAbits.BRGH = 1;//����,��BRG16һ�𣬾��������ʼ��㹫ʽ
    TX1STAbits.TXEN = 1;//ʹ�ܷ���
    
    RC1STAbits.RX9 = 0;//����8bit
    RC1STAbits.ADDEN = 0;//��ֹ��ַ���
    RC1STAbits.CREN = 1;//ʹ����������
    
    BAUD1CONbits.SCKP = 0;//���ͽſ���ʱΪ�ߵ�ƽ
    BAUD1CONbits.BRG16 = 1;//16λ�����ʷ���������BRGHһ�𣬾��������ʼ��㹫ʽ
    BAUD1CONbits.ABDEN = 0;//��ֹ�Զ������ʼ��
   
    RC1STAbits.SPEN = 1;//ʹ�ܴ���
    
    PIR1bits.RCIF = 0;//�����־
    PIE1bits.RCIE = 1;//shi���ж�
    
    //485 ��������
    DE485_OUT;
    DE485_CLR;
    
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
* Description    : ���ڳ�ʼ��,���ڶ�����ýӿ�
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
