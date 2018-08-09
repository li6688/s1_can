/**
  ******************************************************************************
  * @file    w25q.c
  * @author  LL
  * @version V0.0.2
  * @date    2014-10-20
  * @brief   字库操作，所有操作都是通过SPI接口进行的。
  ******************************************************************************
  ******************************************************************************
***/

#include "stm32f10x.h"
#include "main.h"
#include "spi.h"
#include "w25q.h"
#include "font.h"
#include "lcd12864.h"
#include "systick.h"

#define W25Q_TOPADDR       0x00800000
#define W25QSECTORSIZE_4K  0x00001000
#define W25QSPAGESIZE_256  0x00000100
#define W25Q_TOTALSECTOR   (W25Q_TOPADDR/W25QSECTORSIZE_4K)   
#define WRITEDATASPACE_1M    0x00100000
#define WRITEDATA_STARTADDR  (W25Q_TOPADDR-WRITEDATASPACE_1M)

//片选
#define W25Q_CS_SET CS1_SET 
#define W25Q_CS_CLR CS1_CLR

/*******************************************************************************
* Function Name  : W25q_Init
* Description    : 字库芯片W25q初始化
* Input          : None
* Output         : None
* Return         : None
* Date           : 2014-10-20
* Author         : LL
*******************************************************************************/
void W25q_Init(void)
{
  /*
  CS1 -- PB12
  SPI -- PB13-PB15(SPI2)
  */
  SPI2_Init();
  W25Q_CS_SET;
}

/*******************************************************************************
* Function Name  : W25q_ReadWriteByte 
* Description    : W25q芯片SPI读写接口函数
* Input          : None
* Output         : None
* Return         : None
* Date           : 2014-05-14
* Author         : LL
*******************************************************************************/
static uint8_t W25q_ReadWriteByte(uint8_t data)
{
  return SPI2_ReadWriteByte(data);  
}


/*******************************************************************************
* Function Name  : W25q_WaitForIdle
* Description    : 读状态寄存器
* Input          : None
* Output         : None
* Return         : None
* Date           : 2014-10-24
* Author         : LL
*******************************************************************************/
void W25q_WaitForIdle(void)
{
  uint8_t status=0;
  W25Q_CS_CLR;

  W25q_ReadWriteByte(0x05);

  do
  {
    status = W25q_ReadWriteByte(0xFF);
  }while (status&0x01 == 0x01);

  W25Q_CS_SET;
  
}

/*******************************************************************************
* Function Name  : W25q_WriteEnable
* Description    : 写使能
* Input          : None
* Output         : None
* Return         : None
* Date           : 2014-10-24
* Author         : LL
*******************************************************************************/
void W25q_WriteEnable(void)
{
  W25Q_CS_CLR;

  W25q_ReadWriteByte(0x06);

  W25Q_CS_SET; 
}

/*******************************************************************************
* Function Name  : W25q_4KErase
* Description    : 4K片擦除
* Input          : addr:起始地址
* Output         : None
* Return         : None
* Date           : 2014-10-24
* Author         : LL
*******************************************************************************/
void  W25q_4KErase(uint32_t addr)
{
  W25q_WriteEnable();

  W25Q_CS_CLR;

  W25q_ReadWriteByte(0x20);
  W25q_ReadWriteByte((addr & 0xFF0000)>>16);
  W25q_ReadWriteByte((addr & 0x00FF00)>>8);
  W25q_ReadWriteByte(addr & 0xFF);

  W25Q_CS_SET; 
  
  W25q_WaitForIdle(); 
}

/*******************************************************************************
* Function Name  : W25q_32KErase
* Description    : 32K片擦除
* Input          : addr:起始地址
* Output         : None
* Return         : None
* Date           : 2014-10-24
* Author         : LL
*******************************************************************************/
void  W25q_32KErase(uint32_t addr)
{
  W25q_WriteEnable();

  W25Q_CS_CLR;

  W25q_ReadWriteByte(0x52);
  W25q_ReadWriteByte((addr & 0xFF0000)>>16);
  W25q_ReadWriteByte((addr & 0x00FF00)>>8);
  W25q_ReadWriteByte(addr & 0xFF);

  W25Q_CS_SET;
  
  W25q_WaitForIdle();  
}

/*******************************************************************************
* Function Name  : W25q_64KErase
* Description    : 64K片擦除
* Input          : addr:起始地址
* Output         : None
* Return         : None
* Date           : 2014-10-24
* Author         : LL
*******************************************************************************/
void  W25q_64KErase(uint32_t addr)
{
  W25q_WriteEnable();

  W25Q_CS_CLR;

  W25q_ReadWriteByte(0xD8);
  W25q_ReadWriteByte((addr & 0xFF0000)>>16);
  W25q_ReadWriteByte((addr & 0x00FF00)>>8);
  W25q_ReadWriteByte(addr & 0xFF);

  W25Q_CS_SET;
  
  W25q_WaitForIdle();  
}

/*******************************************************************************
* Function Name  : W25q_ChipErase
* Description    : Chip片擦除
* Input          : addr:起始地址
* Output         : None
* Return         : None
* Date           : 2014-10-24
* Author         : LL
*******************************************************************************/
void  W25q_ChipErase(void)
{
  W25q_WriteEnable();

  W25Q_CS_CLR;

  W25q_ReadWriteByte(0xC7);
 
  W25Q_CS_SET;
  
  W25q_WaitForIdle();  
}

/*******************************************************************************
* Function Name  : W25q_Erase
* Description    : 擦除操作，最小以4K为单位进行，从擦除。
* Input          : sectornum:块数,擦除的空间大小为sectornum*4K
                    --例如:如果sectornum=1，则擦除4K字节，
                      sectornum=3，则擦除12K=4K*3个字节
                   addr:起始地址
* Output         : None
* Return         : 状态码
* Date           : 2014-10-25
* Author         : LL
*******************************************************************************/
uint8_t W25q_Erase(uint16_t sectornum,uint32_t addr)
{
  uint8_t blocknum;

  if (addr>=W25Q_TOPADDR) return W25Q_ADDRERROR;
  if (sectornum<=0 || sectornum>W25Q_TOTALSECTOR) return W25Q_ERASEERROR;//没有擦除空间不合理不操作
  
  blocknum = sectornum/16;
  while (blocknum--)//以64K为单位擦除
  {
    W25q_64KErase(addr);
    addr += W25QSECTORSIZE_4K*16;
    sectornum -= 16;   
  }
  blocknum = sectornum/8;
  while (blocknum--)//以32K为单位擦除
  {
    W25q_32KErase(addr);
    addr += W25QSECTORSIZE_4K*8;
    sectornum -= 8;   
  }
  blocknum = sectornum;  
  while (blocknum--)//以4K为单位擦除
  {
    W25q_4KErase(addr);
    addr += W25QSECTORSIZE_4K;   
  }

  return W25Q_ERASEOK; 
}

/*******************************************************************************
* Function Name  : W25q_Erase
* Description    : 擦除操作，最小以4K为单位进行，从擦除。
* Input          : sectornum:块数,擦除的空间大小为sectornum*4K
                   addr:起始地址,相对于可写的顶部1M空间的偏移量
* Output         : None
* Return         : None
* Date           : 2014-10-25
* Author         : LL
*******************************************************************************/
void W25q_EraseTOP1MSapce(uint16_t sectornum,uint32_t addr)
{
  uint32_t startaddr = WRITEDATA_STARTADDR + (addr&0x000FFFFF);
   
  W25q_Erase(sectornum, startaddr);    
}

/*******************************************************************************
* Function Name  : W25q_PageProgram
* Description    : 页写
* Input          : buf:要写内的数据
                   len:数据长度
                   add:起始地址
* Output         : None
* Return         : 状态码
* Date           : 2014-10-24
* Author         : LL
*******************************************************************************/
static uint8_t  W25q_PageProgram(uint8_t *buf, uint16_t len, uint32_t addr)
{
  if (addr>=W25Q_TOPADDR) return W25Q_ADDRERROR;
  if (len<=0 || len>256) return W25Q_WRITEDATAERROR;//没有数据不操作

  W25q_WriteEnable();

  W25Q_CS_CLR;

  W25q_ReadWriteByte(0x02);
  W25q_ReadWriteByte((addr & 0xFF0000)>>16);
  W25q_ReadWriteByte((addr & 0x00FF00)>>8);
  W25q_ReadWriteByte(addr & 0xFF);

  while (len--)
  {
    W25q_ReadWriteByte(*buf);
    buf++;   
  }

  W25Q_CS_SET;
  
  W25q_WaitForIdle();
  
  return W25Q_WRITEDATAOK;   
}

/*******************************************************************************
* Function Name  : W25q_Write
* Description    : 数据存储,为防止意外的擦除
* Input          : buf:要写内的数据
                   len:数据长度
                   add:起始地址
* Output         : None
* Return         : None
* Date           : 2014-10-24
* Author         : LL
*******************************************************************************/
void W25q_Write(uint8_t *buf, uint16_t len, uint32_t addr)
{
  uint8_t pagenum;
  uint8_t addrbyte;//最低八位地址
  
  addrbyte = addr%W25QSPAGESIZE_256;
  if (len > (W25QSPAGESIZE_256-addrbyte))//跨页了
  {
      W25q_PageProgram(buf, W25QSPAGESIZE_256-addrbyte, addr);//写满本页
      addr += W25QSPAGESIZE_256-addrbyte;
      buf += W25QSPAGESIZE_256-addrbyte;
      len -= W25QSPAGESIZE_256-addrbyte;
      pagenum = len/W25QSPAGESIZE_256;
 
      while (pagenum--)
      {
        W25q_PageProgram(buf, W25QSPAGESIZE_256, addr);
        addr += W25QSPAGESIZE_256;
        buf += W25QSPAGESIZE_256;
        len -= W25QSPAGESIZE_256;  
      }
      W25q_PageProgram(buf, len, addr);
      
  }
  else
  {
    W25q_PageProgram(buf, len, addr);
  } 
}

/*******************************************************************************
* Function Name  : W25q_WriteTop1MSpace
* Description    : 数据存储
* Input          : buf:要写内的数据
                   len:数据长度
                   add:起始地址,相对于可写空间的偏移量
* Output         : None
* Return         : None
* Date           : 2014-10-24
* Author         : LL
*******************************************************************************/
void W25q_WriteTop1MSpace(uint8_t *buf, uint16_t len, uint32_t addr)
{
  uint32_t startaddr = WRITEDATA_STARTADDR + (addr&0x000FFFFF);
   
  W25q_Write(buf, len, startaddr);  
}

/*******************************************************************************
* Function Name  : W25q_WriteStatus
* Description    : 写状态寄存器
* Input          : status:状态值
* Output         : None
* Return         : None
* Date           : 2014-10-24
* Author         : LL
*******************************************************************************/
void W25q_WriteStatus(uint16_t status)
{
  W25q_WriteEnable();

  W25Q_CS_CLR;

  W25q_ReadWriteByte(0x01);
  W25q_ReadWriteByte(status & 0xFF);
  W25q_ReadWriteByte((status>>8) & 0xFF);

  W25Q_CS_SET;

  W25q_WaitForIdle();  
  
}

/*******************************************************************************
* Function Name  : W25q_ReadStatus
* Description    : 读状态寄存器
* Input          : None
* Output         : None
* Return         : 状态寄存器值
* Date           : 2014-10-24
* Author         : LL
*******************************************************************************/
uint16_t W25q_ReadStatus(void)
{
  uint16_t status=0;
  W25Q_CS_CLR;

  W25q_ReadWriteByte(0x05);
  status = W25q_ReadWriteByte(0xFF);

  W25Q_CS_SET;

  W25Q_CS_CLR;
  W25q_ReadWriteByte(0x35);
  status |= W25q_ReadWriteByte(0xFF)<<8;
  W25Q_CS_SET;

  
  return status;  
}

/*******************************************************************************
* Function Name  : W25q_ReadDeviceID
* Description    : 读字库芯片ID
* Input          : None
* Output         : None
* Return         : None
* Date           : 2014-10-20
* Author         : LL
*******************************************************************************/
uint16_t W25q_ReadDeviceID(void)
{
  uint16_t DeviceID=0;

  W25Q_CS_CLR;

  W25q_ReadWriteByte(0x90);
  W25q_ReadWriteByte(0x00);
  W25q_ReadWriteByte(0x00);
  W25q_ReadWriteByte(0x00);

  DeviceID = W25q_ReadWriteByte(0xFF)<<8;
  DeviceID |= W25q_ReadWriteByte(0xFF);

  W25Q_CS_SET;

  return DeviceID;
}

/*******************************************************************************
* Function Name  : W25q_Read
* Description    : 读数据
* Input          : buf:数据缓冲区
                   len:长度
                   addr:起始地址
* Output         : None
* Return         : 返回状态，暂无意义
* Date           : 2014-10-20
* Author         : LL
*******************************************************************************/
uint8_t W25q_Read(uint8_t *buf, uint32_t len, uint32_t addr)
{
  uint32_t i;
  uint8_t status;

  if (addr>=W25Q_TOPADDR) return W25Q_ADDRERROR;
  W25Q_CS_CLR;
    
  status = W25q_ReadWriteByte(0x03);
  status = W25q_ReadWriteByte((addr & 0xFF0000)>>16);
  status = W25q_ReadWriteByte((addr & 0x00FF00)>>8);
  status = W25q_ReadWriteByte(addr & 0xFF);
  for (i=0; i<len; i++)
  {
    buf[i] = W25q_ReadWriteByte(0xFF);
  }
  buf[i] = '\0';

  status = W25Q_READOK;

  W25Q_CS_SET;

  return status;
}

/*******************************************************************************
* Function Name  : W25q_Test 
* Description    : 测试函数
* Input          : None
* Output         : None
* Return         : None
* Date           : 2014-10-27
* Author         : LL
*******************************************************************************/
void W25q_Test(void)
{
    uint8_t sta;
    uint8_t rbuf[32];
    uint16_t i;
    uint16_t len;
    uint32_t addr;

    LCD12864_DisplayStr(0,0,0,FONT_SIZE16,"---W25Q TEST---");
    len = 4;
    addr = 32;
    for (i=0; i<len; i++)
    {
        rbuf[i] = 0;
    }
    sta=W25q_Read(rbuf, len, addr);
    if (sta != W25Q_READOK)
    {
    }
}

/* ************ ****** ************ THE FILE END ************ ****** ************ */
