/**
******************************************************************************
* @file    spi.c
* @author  LL
* @version V0.0.0
* @date    2017-07-28
* @brief   
******************************************************************************
******************************************************************************
***/
#define __SPI_C__

#include "config.h"

//片选 RC6
#define CS_TRI TRISCbits.TRISC6
#define CS_PORT LATCbits.LATC6
#define CS_ANS  ANSELCbits.ANSC6
#define CS_OUT  {CS_ANS = 0; CS_TRI = 0; NOP();}
#define CS_SET  {CS_PORT = 1;}
#define CS_CLR  {CS_PORT = 0;}

/*******************************************************************************
* Function Name  : SPI_ReadWriteByte 
* Description    : spi读写函数
* Input          : None
* Output         : None
* Return         : None
* Date           : 2017-07-28
* Author         : LL
*******************************************************************************/
uint8_t SPI_ReadWriteByte(uint8_t data)
{ 
    PIR1bits.SSP1IF=0;
    // Wait for free buffer
    while(SSP1STATbits.BF);
    SSP1BUF = data;
    // Wait for data byte
    while(!SSP1STATbits.BF);
    return SSP1BUF;
}

/*******************************************************************************
* Function Name  : SPI_DeInit 
* Description    : spi关闭
* Input          : None
* Output         : None
* Return         : None
* Date           : 2017-07-28
* Author         : LL
*******************************************************************************/
void SPI_DeInit(void)
{
	SSP1CON1bits.SSPEN = 0;/* Disable SPI */
}

/*******************************************************************************
* Function Name  : SPI_Init 
* Description    : spi初始化
* Input          : None
* Output         : None
* Return         : None
* Date           : 2017-07-28
* Author         : LL
*******************************************************************************/
void SPI_Init(void)
{
	// Unlock Registers
    //GIE = 0;//禁止中断
    PPSLOCK = 0x55; // 第一个解锁代码
    PPSLOCK = 0xAA; // 第二个解锁代码
    PPSLOCKbits.PPSLOCKED = 0x00; // 解锁 PPS
    
    //配置端口映射，SPI
    RB6PPSbits.RB6PPS = 0x18;//输出为RB6
    RB4PPSbits.RB4PPS = 0x19;//输出为RB4
    SSP1DATPPSbits.SSP1DATPPS = 0x12;//输入RC2
    
    
    // Lock Registers
    PPSLOCK = 0x55; // 第一个锁定代码
    PPSLOCK = 0xAA; // 第二个锁定代码
    PPSLOCKbits.PPSLOCKED = 0x01; // 锁定 PPS
    
    //GIE = 1; // 允许中断
    
    //关闭引脚的模拟功能
    ANSELBbits.ANSB6 = 0; //SCK
    ANSELBbits.ANSB4 = 0; //OUT
    ANSELCbits.ANSC2 = 0; //IN
    //配置引脚的方向
    TRISBbits.TRISB6 = 0; //SCK
    TRISBbits.TRISB4 = 0; //OUT
    TRISCbits.TRISC2 = 1; //IN 

	CS_OUT;
	CS_SET;
	
    SSP1STAT=0;
    SSP1CON1=0;
	SSP1STATbits.CKE=0;//0时钟状态从空闲转换到有效时发送 1时钟状态从有效转换到空闲时发送
    SSP1STATbits.SMP=0;//0在数据输出时间的中间采样输入数据 1在数据输出时间的末尾采样输入数据
    SSP1CON1bits.SSPM = 0;//0 SPI主模式,时钟Fosc/4 1 SPI主模式,时钟Fosc/16
    SSP1CON1bits.CKP=1;//0时钟空闲状态为低电平 1时钟空闲状态为高电平
    SSP1CON1bits.SSPEN=1;//使能模块并将SCKx、SDOx、SDIx 和SSx 配置为串行端口引脚
}
/* ************ ****** ************ THE FILE END  ************ ****** ************ */				
