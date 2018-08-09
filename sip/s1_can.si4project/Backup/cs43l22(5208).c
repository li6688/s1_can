/**
  ******************************************************************************
  * @file    cs43l22.c
  * @author  LL
  * @version V0.0.0
  * @date    2018-06-23
  * @brief   
  ******************************************************************************
  ******************************************************************************
***/

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"
#include "spi.h"
#include "systick.h"
#include "uart_printf.h"
#include "sys.h"

//��λ
#define CS43L22_RST_SET CS1_SET 
#define CS43L22_RST_CLR CS1_CLR

/*******************************************************************************
* Function Name  : Lis3dsh_Init
* Description    : ��ʼ��
* Input          : None
* Output         : None
* Return         : None
* Date           : 2018-06-23
* Author         : LL
*******************************************************************************/
void Lis3dsh_Init(void)
{
    uint8_t buf[8];
    /*
    CS1 -- PE3
    SPI -- PA5-PA7(SPI1)
    */
    SPI1_Init();
    LIS3DSH_CS_SET;


    buf[0]=0x97;
    Lis3dsh_Write(buf, 1, 0x20);
    Lis3dsh_Read(buf, 1, 0x0C);
    Lis3dsh_Read(buf, 7, 0x27);
    //buf[0]=0xC8;
    //Lis3dsh_Write(buf, 1, 0x23);
}

/*******************************************************************************
* Function Name  : Lis3dsh_ReadWriteByte 
* Description    : SPI��д�ӿں���
* Input          : None
* Output         : None
* Return         : None
* Date           : 2018-06-23
* Author         : LL
*******************************************************************************/
static uint8_t Lis3dsh_ReadWriteByte(uint8_t data)
{
  return SPI1_ReadWriteByte(data);  
}

/*******************************************************************************
* Function Name  : Lis3dsh_Write
* Description    : ���ݴ洢,Ϊ��ֹ����Ĳ���
* Input          : buf:Ҫд�ڵ�����
                   len:���ݳ���
                   add:��ʼ��ַ
* Output         : None
* Return         : None
* Date           : 2018-06-23
* Author         : LL
*******************************************************************************/
uint8_t Lis3dsh_Write(uint8_t *buf, uint8_t len, uint8_t addr)
{
    uint8_t i;
    LIS3DSH_CS_CLR;

    Lis3dsh_ReadWriteByte(addr & 0x7F);
    for (i=0; i<len; i++)
    {
       Lis3dsh_ReadWriteByte(buf[i]);
    }
    
    LIS3DSH_CS_SET;

    return 0;
}

/*******************************************************************************
* Function Name  : Lis3dsh_Read
* Description    : ������
* Input          : buf:���ݻ�����
                   len:����
                   addr:��ʼ��ַ
* Output         : None
* Return         : ����״̬����������
* Date           : 2018-06-23
* Author         : LL
*******************************************************************************/
uint8_t Lis3dsh_Read(uint8_t *buf, uint8_t len, uint8_t addr)
{ 
    uint8_t i;
    LIS3DSH_CS_CLR;

    Lis3dsh_ReadWriteByte(addr | 0x80);
    for (i=0; i<len; i++)
    {
        buf[i] = Lis3dsh_ReadWriteByte(0xFF);
    }
    
    LIS3DSH_CS_SET;

    return 0;
}

/*******************************************************************************
* Function Name  : Cs43l22_Reset
* Description    : ��λ
* Input          : None
* Output         : None
* Return         : None
* Date           : 2018-07-07
* Author         : LL
*******************************************************************************/
void Cs43l22_Reset(void)
{  
    CS43L22_RST_CLR;
    Delay_10ms(1);
    CS43L22_RST_SET;
}


/*******************************************************************************
* Function Name  : Lis3dsh_Test 
* Description    : ���Ժ���
* Input          : None
* Output         : None
* Return         : None
* Date           : 2018-06-23
* Author         : LL
*******************************************************************************/
void Lis3dsh_Test(void)
{
    uint8_t buf[8];
    //buf[0]=0xE1;
    //Lis3dsh_Write(buf, 1, 0x20);
    Lis3dsh_Read(buf, 4, 0x0C);
    DEBUG_PRINTFMASSAGESHEX(buf, 4);
    Lis3dsh_Read(buf, 1, 0x27);
    DEBUG_PRINTFMASSAGESHEX(buf, 1);
    Lis3dsh_ReadAccData();
}

/* ************ ****** ************ THE FILE END ************ ****** ************ */
