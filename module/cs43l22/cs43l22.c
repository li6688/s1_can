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
#include "i2c.h"
#include "cs43l22.h"

#define RST_PORT  GPIOD
#define RST_BIT   GPIO_Pin_4
#define RST_SET   RST_PORT->BSRRL |= RST_BIT;//高电平
#define RST_CLR   RST_PORT->BSRRH |= RST_BIT;//低电平

//复位
#define CS43L22_RST_SET RST_SET 
#define CS43L22_RST_CLR RST_CLR

static void Cs43l22_AudioInterface_Init(uint32_t AudioFreq)
{
    I2S_InitTypeDef I2S_InitStructure;

    /* CODEC_I2S peripheral configuration */
    SPI_I2S_DeInit(SPI3);
    I2S_InitStructure.I2S_AudioFreq = AudioFreq;
    I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;
    I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
    I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;

    I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;
    I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable;

    /* Initialize the I2S peripheral with the structure above */
    I2S_Init(SPI3, &I2S_InitStructure); 
}


static uint32_t Cs43l22_VolumeCtrl(uint8_t Volume)
{  
    uint8_t v;
    if (Volume > 0xE6)
    {
        /* Set the Master volume */
        v = Volume - 0xE7;
        Cs43l22_Write(&v, 1, 0x20);
        v = Volume - 0xE7;
        Cs43l22_Write(&v, 1, 0x21);
    }
    else
    {
        /* Set the Master volume */
        v = Volume - 0x19;
        Cs43l22_Write(&v, 1, 0x20);
        v = Volume - 0x19;
        Cs43l22_Write(&v, 1, 0x21);
    }

    return 0;  
}


/*******************************************************************************
* Function Name  : Cs43l22_Reset
* Description    : 复位
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
* Function Name  : Cs43l22_Init
* Description    : 初始化
* Input          : None
* Output         : None
* Return         : None
* Date           : 2018-06-23
* Author         : LL
*******************************************************************************/
void Cs43l22_Init(void)
{
    uint8_t buf[8];
    
    Cs43l22_Reset();
    I2C1_Init();

    DEBUG_PRINTFMASSAGES("Cs43l22_Init");
    Cs43l22_Read(buf, 1, 0x01);
    DEBUG_PRINTFNUMBERHEX(buf[0]);

    buf[0]=0x01;
    Cs43l22_Write(buf, 1, 0x02);
    Cs43l22_Read(buf, 1, 0x02);
    DEBUG_PRINTFNUMBERHEX(buf[0]);
    buf[0]=0xAF;
    Cs43l22_Write(buf, 1, 0x04);
    buf[0]=0x81;
    Cs43l22_Write(buf, 1, 0x05);
    buf[0]=0x04;
    Cs43l22_Write(buf, 1, 0x06);
    Cs43l22_VolumeCtrl(50);
    if (0)
    {
        /* Enable the PassThrough on AIN1A and AIN1B */
        buf[0]=0x01;
        Cs43l22_Write(buf, 1, 0x08);
        buf[0]=0x01;
        Cs43l22_Write(buf, 1, 0x09);

        /* Route the analog input to the HP line */
        buf[0]=0xC0;
        Cs43l22_Write(buf, 1, 0x0E);

        /* Set the Passthough volume */
        buf[0]=0x00;
        Cs43l22_Write(buf, 1, 0x14);
        buf[0]=0x00;
        Cs43l22_Write(buf, 1, 0x15);
    }
    buf[0]=0x9E;
    Cs43l22_Write(buf, 1, 0x02);
    buf[0]=0x00;
    Cs43l22_Write(buf, 1, 0x0A);
    buf[0]=0x04;
    Cs43l22_Write(buf, 1, 0x0E);
    buf[0]=0x00;
    Cs43l22_Write(buf, 1, 0x27);
    buf[0]=0x0F;
    Cs43l22_Write(buf, 1, 0x1F);
    buf[0]=0x0A;
    Cs43l22_Write(buf, 1, 0x1A);
    buf[0]=0x0A;
    Cs43l22_Write(buf, 1, 0x1B);
    Cs43l22_AudioInterface_Init(1000);
    
}

/*******************************************************************************
* Function Name  : Cs43l22_Write
* Description    : 数据存储,为防止意外的擦除
* Input          : buf:要写内的数据
                   len:数据长度
                   add:起始地址
* Output         : None
* Return         : None
* Date           : 2018-06-23
* Author         : LL
*******************************************************************************/
uint8_t Cs43l22_Write(uint8_t *buf, uint8_t len, uint8_t addr)
{
    
    return 0;
}

/*******************************************************************************
* Function Name  : Cs43l22_Read
* Description    : 读数据
* Input          : buf:数据缓冲区
                   len:长度
                   addr:起始地址
* Output         : None
* Return         : 返回状态，暂无意义
* Date           : 2018-06-23
* Author         : LL
*******************************************************************************/
uint8_t Cs43l22_Read(uint8_t *buf, uint8_t len, uint8_t addr)
{ 
    I2C1_Read(buf, addr, len);

    return 0;
}

/*******************************************************************************
* Function Name  : Cs43l22_Test 
* Description    : 测试函数
* Input          : None
* Output         : None
* Return         : None
* Date           : 2018-06-23
* Author         : LL
*******************************************************************************/
void Cs43l22_Test(void)
{
   
}

/* ************ ****** ************ THE FILE END ************ ****** ************ */
