/**
  ******************************************************************************
  * @file    lis3dsh.c
  * @author  LL
  * @version V0.0.0
  * @date    2018-06-23
  * @brief   所有操作都是通过SPI接口进行的。
  ******************************************************************************
  ******************************************************************************
***/

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"
#include "spi.h"
#include "systick.h"
#include "uart_printf.h"
#include "lis3dsh.h"
#include "sys.h"

//片选
#define LIS3DSH_CS_SET CS1_SET 
#define LIS3DSH_CS_CLR CS1_CLR

/*******************************************************************************
* Function Name  : Lis3dsh_Init
* Description    : 初始化
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
* Description    : SPI读写接口函数
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
* Description    : 数据存储,为防止意外的擦除
* Input          : buf:要写内的数据
                   len:数据长度
                   add:起始地址
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
* Description    : 读数据
* Input          : buf:数据缓冲区
                   len:长度
                   addr:起始地址
* Output         : None
* Return         : 返回状态，暂无意义
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
* Function Name  : Lis3dsh_ReadAccData
* Description    : 读数据
* Input          : None
* Output         : None
* Return         : 返回状态，暂无意义
* Date           : 2018-06-23
* Author         : LL
*******************************************************************************/
uint8_t Lis3dsh_ReadAccData(void)
{  
    uint8_t  dir=0;
    uint16_t x,y,z;
    uint16_t ax,ay,az;
    uint8_t buf[8];

    Lis3dsh_Read(buf, 7, 0x27);
    //DEBUG_PRINTFMASSAGESHEX(buf, 7);
    x = buf[1]+(buf[2]<<8);
    if (x&0x8000)
    {
        x=0xFFFF-x;
        dir = 1;
    }
    ax = x*6/100;
    y = buf[3]+(buf[4]<<8);
    if (y&0x8000)
    {
        y=0xFFFF-y;
        dir |= 1<<1; 
    }
    ay = y*6/100;
    z = buf[5]+(buf[6]<<8);
    if (z&0x8000)
    {
        z=0xFFFF-z;
    }
    az = z*6/100;
    az = az;
    //DEBUG_PRINTFNUMBERBCD(ax);
    //DEBUG_PRINTFNUMBERBCD(ay);
    //DEBUG_PRINTFNUMBERBCD(az);

    if (ax>ay)
    {
        GlobalVar.Dir = (dir & 0x01) ? 1 : 4 ;
    }
    else
    {
        GlobalVar.Dir = (dir & 0x02) ? 8 : 2 ;
    }

    
    return 0;
}


/*******************************************************************************
* Function Name  : Lis3dsh_Test 
* Description    : 测试函数
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
