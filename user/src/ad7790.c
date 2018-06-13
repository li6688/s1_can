/**
******************************************************************************
* @file    ad7790.c
* @author  LL
* @version V0.0.0
* @date    2017-07-28
* @brief   
******************************************************************************
******************************************************************************
***/
#define __AD7790_C__

#include <xc.h>
#include <stdint.h>
#include "spi.h"
#include "uart_printf.h"

//片选 RC6
#define AD7790CS_TRI TRISCbits.TRISC6
#define AD7790CS_PORT LATCbits.LATC6
#define AD7790CS_ANS  ANSELCbits.ANSC6
#define AD7790CS_OUT  {AD7790CS_ANS = 0; AD7790CS_TRI = 0; NOP();}
#define AD7790CS_SET  {AD7790CS_PORT = 1;}
#define AD7790CS_CLR  {AD7790CS_PORT = 0;}

#define AD7790_DOUT PORTCbits.RC2
#define AD7790_CH 0 //转换通道
#define AD7790_LEN 18

/*******************************************************************************
* Function Name  : AD7790_ReadWriteByte 
* Description    : 读写函数
* Input          : data:写入的数据
* Output         : None
* Return         : 读出的数据
* Date           : 2017-07-28
* Author         : LL
*******************************************************************************/
uint8_t AD7790_ReadWriteByte(uint8_t data)
{ 
   return SPI_ReadWriteByte(data);
}

/*******************************************************************************
* Function Name  : AD7790_Reset 
* Description    : 复位
* Input          : None
* Output         : None
* Return         : None
* Date           : 2017-07-31
* Author         : LL
*******************************************************************************/
void AD7790_Reset(void)
{
    AD7790CS_CLR;
    AD7790_ReadWriteByte(0xFF);
    AD7790_ReadWriteByte(0xFF);
    AD7790_ReadWriteByte(0xFF);
    AD7790_ReadWriteByte(0xFF);
    AD7790CS_SET;
}

/*******************************************************************************
* Function Name  : AD7790_WriteCR 
* Description    : 写通信寄存器
* Input          : None
* Output         : None
* Return         : None
* Date           : 2017-07-28
* Author         : LL
*******************************************************************************/
void AD7790_WriteCR(uint8_t data)
{ 
    AD7790CS_CLR;
    AD7790_ReadWriteByte(data);
    AD7790_ReadWriteByte(0x00);
    AD7790CS_SET;
}

/*******************************************************************************
* Function Name  : AD7790_ReadSR 
* Description    : 读状态寄存器
* Input          : None
* Output         : None
* Return         : 状态值
* Date           : 2017-07-31
* Author         : LL
*******************************************************************************/
uint8_t AD7790_ReadSR(void)
{ 
    uint8_t sta;
    
    AD7790CS_CLR;
    AD7790_ReadWriteByte(0x08|AD7790_CH);
    sta = AD7790_ReadWriteByte(0x00);
    AD7790CS_SET;
    return sta;
}

/*******************************************************************************
* Function Name  : AD7790_ReadWriteMR 
* Description    : 读写模式寄存器
* Input          : m:写入的值，如果值为0xFF,则表示读
* Output         : None
* Return         : 写入的结果
* Date           : 2017-07-31
* Author         : LL
*******************************************************************************/
uint8_t AD7790_ReadWriteMR(uint8_t m)
{ 
    uint8_t sta;
    
    AD7790CS_CLR;
    if(0xFF != m)
    {
        AD7790_ReadWriteByte(0x10|AD7790_CH);
        AD7790_ReadWriteByte(m);
    }
    AD7790_ReadWriteByte(0x18|AD7790_CH);
    sta = AD7790_ReadWriteByte(0x00);
    AD7790CS_SET;
	DEBUG_PRINTFNUMBERHEX(sta);
    return sta;
}

/*******************************************************************************
* Function Name  : AD7790_ReadWriteFR 
* Description    : 读写滤波寄存器
* Input          : f:写入的值，如果值为0xFF,则表示读
* Output         : None
* Return         : 写入的结果
* Date           : 2017-07-31
* Author         : LL
*******************************************************************************/
uint8_t AD7790_ReadWriteFR(uint8_t f)
{ 
    uint8_t sta;
    
    AD7790CS_CLR;
    if(0xFF != f)
    {
        AD7790_ReadWriteByte(0x20|AD7790_CH);
        AD7790_ReadWriteByte(f);
    }
    AD7790_ReadWriteByte(0x28|AD7790_CH);
    sta = AD7790_ReadWriteByte(0x00);
    AD7790CS_SET;
    return sta;
}

/*******************************************************************************
* Function Name  : AD7790_ReadDR 
* Description    : 读数据寄存器
* Input          : None
* Output         : None
* Return         : 数据
* Date           : 2017-07-31
* Author         : LL
*******************************************************************************/
uint16_t AD7790_ReadDR(void)
{ 
    uint8_t dh,dl;
    uint8_t  sta;
    uint16_t d;
    uint16_t retry=2000;
    //等待数据就绪
    do
    {
        sta = AD7790_ReadSR();
        if (0x00 == (sta&0x80)) break;
    }while(retry--);
    if (0==retry)
    {
        AD7790_Reset();
        return 0;
    }
    AD7790CS_CLR;
    AD7790_ReadWriteByte(0x38|AD7790_CH);
    dh = AD7790_ReadWriteByte(0x00);
    dl = AD7790_ReadWriteByte(0x00);
    AD7790CS_SET;
    d = ((uint16_t)dh<<8) | dl;
    DEBUG_PRINTFNUMBERHEX(d);
    return d;
}
/*******************************************************************************
* Function Name  : AD7790_ConverSingle 
* Description    : 转化一次数据
* Input          : None
* Output         : None
* Return         : 数据
* Date           : 2017-07-31
* Author         : LL
*******************************************************************************/
uint16_t AD7790_ConverSingle(void)
{ 
    uint8_t dh,dl;
    uint16_t d;
    uint16_t retry=2000;

    AD7790CS_CLR;
    //启动一次转化
    AD7790_ReadWriteByte(0x10|AD7790_CH);
    AD7790_ReadWriteByte(0x8A);
    //等待转化完成
    do
    {
        if(0==AD7790_DOUT) break;
    }while(retry--);
    if (0==retry)
    {
        AD7790CS_SET;
        AD7790_Reset();
        return 0;
    }
    //读取数据
    AD7790_ReadWriteByte(0x38|AD7790_CH);
    dh = AD7790_ReadWriteByte(0x00);
    dl = AD7790_ReadWriteByte(0x00);
    d = ((uint16_t)dh<<8) | dl;
    AD7790CS_SET;
    return d;
}

/*******************************************************************************
* Function Name  : AD7790_ConverStable
* Description    : 多次转化后取平均值，转化一次稳定值

处理方法
采样AD7790_LEN = 18 次
去掉最大值和最小值，其他值求和取平均

* Input          : None
* Output         : None
* Return         : averageval多次采样处理后的结果
* Date           : 2017-08-01
* Author         : LL
*******************************************************************************/
uint16_t AD7790_ConverStable(void)
{
	uint8_t i;
	uint16_t adcval;
	uint16_t minval=0xFFFF;
	uint16_t maxval=0;
	uint16_t averageval;
	uint32_t totalval=0;
	
	for (i=0; i<AD7790_LEN; i++)
	{
		adcval = AD7790_ConverSingle();
		totalval += adcval;
		if (minval>adcval) minval = adcval;
		if (maxval<adcval) maxval = adcval;
	}
	
	totalval = totalval - minval - maxval;
	averageval = totalval >> 4;
	
	return averageval;
}

/*******************************************************************************
* Function Name  : AD7790_Init 
* Description    : 初始化
* Input          : None
* Output         : None
* Return         : None
* Date           : 2017-07-28
* Author         : LL
*******************************************************************************/
void AD7790_Init(void)
{
   SPI_Init();
   AD7790CS_OUT;
   AD7790CS_SET;
   AD7790_Reset();
   AD7790_ReadWriteMR(0x0A);
}

/*******************************************************************************
* Function Name  : AD7790_Init 
* Description    : 初始化
* Input          : None
* Output         : None
* Return         : None
* Date           : 2017-07-28
* Author         : LL
*******************************************************************************/
void AD7790_Process(void)
{
   
}

/* ************ ****** ************ THE FILE END  ************ ****** ************ */				
