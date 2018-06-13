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

//Ƭѡ RC6
#define AD7790CS_TRI TRISCbits.TRISC6
#define AD7790CS_PORT LATCbits.LATC6
#define AD7790CS_ANS  ANSELCbits.ANSC6
#define AD7790CS_OUT  {AD7790CS_ANS = 0; AD7790CS_TRI = 0; NOP();}
#define AD7790CS_SET  {AD7790CS_PORT = 1;}
#define AD7790CS_CLR  {AD7790CS_PORT = 0;}

#define AD7790_DOUT PORTCbits.RC2
#define AD7790_CH 0 //ת��ͨ��
#define AD7790_LEN 18

/*******************************************************************************
* Function Name  : AD7790_ReadWriteByte 
* Description    : ��д����
* Input          : data:д�������
* Output         : None
* Return         : ����������
* Date           : 2017-07-28
* Author         : LL
*******************************************************************************/
uint8_t AD7790_ReadWriteByte(uint8_t data)
{ 
   return SPI_ReadWriteByte(data);
}

/*******************************************************************************
* Function Name  : AD7790_Reset 
* Description    : ��λ
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
* Description    : дͨ�żĴ���
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
* Description    : ��״̬�Ĵ���
* Input          : None
* Output         : None
* Return         : ״ֵ̬
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
* Description    : ��дģʽ�Ĵ���
* Input          : m:д���ֵ�����ֵΪ0xFF,���ʾ��
* Output         : None
* Return         : д��Ľ��
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
* Description    : ��д�˲��Ĵ���
* Input          : f:д���ֵ�����ֵΪ0xFF,���ʾ��
* Output         : None
* Return         : д��Ľ��
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
* Description    : �����ݼĴ���
* Input          : None
* Output         : None
* Return         : ����
* Date           : 2017-07-31
* Author         : LL
*******************************************************************************/
uint16_t AD7790_ReadDR(void)
{ 
    uint8_t dh,dl;
    uint8_t  sta;
    uint16_t d;
    uint16_t retry=2000;
    //�ȴ����ݾ���
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
* Description    : ת��һ������
* Input          : None
* Output         : None
* Return         : ����
* Date           : 2017-07-31
* Author         : LL
*******************************************************************************/
uint16_t AD7790_ConverSingle(void)
{ 
    uint8_t dh,dl;
    uint16_t d;
    uint16_t retry=2000;

    AD7790CS_CLR;
    //����һ��ת��
    AD7790_ReadWriteByte(0x10|AD7790_CH);
    AD7790_ReadWriteByte(0x8A);
    //�ȴ�ת�����
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
    //��ȡ����
    AD7790_ReadWriteByte(0x38|AD7790_CH);
    dh = AD7790_ReadWriteByte(0x00);
    dl = AD7790_ReadWriteByte(0x00);
    d = ((uint16_t)dh<<8) | dl;
    AD7790CS_SET;
    return d;
}

/*******************************************************************************
* Function Name  : AD7790_ConverStable
* Description    : ���ת����ȡƽ��ֵ��ת��һ���ȶ�ֵ

������
����AD7790_LEN = 18 ��
ȥ�����ֵ����Сֵ������ֵ���ȡƽ��

* Input          : None
* Output         : None
* Return         : averageval��β��������Ľ��
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
* Description    : ��ʼ��
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
* Description    : ��ʼ��
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
