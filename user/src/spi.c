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

//Ƭѡ RC6
#define CS_TRI TRISCbits.TRISC6
#define CS_PORT LATCbits.LATC6
#define CS_ANS  ANSELCbits.ANSC6
#define CS_OUT  {CS_ANS = 0; CS_TRI = 0; NOP();}
#define CS_SET  {CS_PORT = 1;}
#define CS_CLR  {CS_PORT = 0;}

/*******************************************************************************
* Function Name  : SPI_ReadWriteByte 
* Description    : spi��д����
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
* Description    : spi�ر�
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
* Description    : spi��ʼ��
* Input          : None
* Output         : None
* Return         : None
* Date           : 2017-07-28
* Author         : LL
*******************************************************************************/
void SPI_Init(void)
{
	// Unlock Registers
    //GIE = 0;//��ֹ�ж�
    PPSLOCK = 0x55; // ��һ����������
    PPSLOCK = 0xAA; // �ڶ�����������
    PPSLOCKbits.PPSLOCKED = 0x00; // ���� PPS
    
    //���ö˿�ӳ�䣬SPI
    RB6PPSbits.RB6PPS = 0x18;//���ΪRB6
    RB4PPSbits.RB4PPS = 0x19;//���ΪRB4
    SSP1DATPPSbits.SSP1DATPPS = 0x12;//����RC2
    
    
    // Lock Registers
    PPSLOCK = 0x55; // ��һ����������
    PPSLOCK = 0xAA; // �ڶ�����������
    PPSLOCKbits.PPSLOCKED = 0x01; // ���� PPS
    
    //GIE = 1; // �����ж�
    
    //�ر����ŵ�ģ�⹦��
    ANSELBbits.ANSB6 = 0; //SCK
    ANSELBbits.ANSB4 = 0; //OUT
    ANSELCbits.ANSC2 = 0; //IN
    //�������ŵķ���
    TRISBbits.TRISB6 = 0; //SCK
    TRISBbits.TRISB4 = 0; //OUT
    TRISCbits.TRISC2 = 1; //IN 

	CS_OUT;
	CS_SET;
	
    SSP1STAT=0;
    SSP1CON1=0;
	SSP1STATbits.CKE=0;//0ʱ��״̬�ӿ���ת������Чʱ���� 1ʱ��״̬����Чת��������ʱ����
    SSP1STATbits.SMP=0;//0���������ʱ����м������������ 1���������ʱ���ĩβ������������
    SSP1CON1bits.SSPM = 0;//0 SPI��ģʽ,ʱ��Fosc/4 1 SPI��ģʽ,ʱ��Fosc/16
    SSP1CON1bits.CKP=1;//0ʱ�ӿ���״̬Ϊ�͵�ƽ 1ʱ�ӿ���״̬Ϊ�ߵ�ƽ
    SSP1CON1bits.SSPEN=1;//ʹ��ģ�鲢��SCKx��SDOx��SDIx ��SSx ����Ϊ���ж˿�����
}
/* ************ ****** ************ THE FILE END  ************ ****** ************ */				
