/**
  ******************************************************************************
  * @file    w25q64.c
  * @author  ADT LL
  * @version V0.0.0
  * @date    2014-10-20
  * @brief   字库操作，所有操作都是通过SPI接口进行的。
  ******************************************************************************
  ******************************************************************************
***/

#include "stm32f10x.h"
#include "main.h"
#include "spi.h"
#include "lcd.h"
#include "w25q64.h"

#define CODE_READDATA 0x03

/*字符地址定义*/
#define CHAR_ASCII16_ADDR   0x00000000
#define CHAR_CHINESE16_ADDR 0x000005F0
#define CHAR_ASCII24_ADDR   0x000353F0
#define CHAR_CHINESE24_ADDR 0x0003614C
#define CHAR_ASCII32_ADDR   0x000AD0CC
#define CHAR_CHINESE32_ADDR 0x000AE88C
#define CHAR_ASCII48_ADDR   0x0018208C
#define CHAR_CHINESE48_ADDR 0x001855FE

#define CHAR_SYMBOL16_ADDR  0x00361400
#define CHAR_SYMBOL24_ADDR  0x00363720
#define CHAR_SYMBOL32_ADDR  0x00368628
#define CHAR_SYMBOL48_ADDR  0x003712A8


#define PICTURE1_ADDR       0x00384EC8
#define PICTURE2_ADDR       0x003E48C8

#define ICON_SELECTR_ADDR       0x004A16AC
#define ICON_SELECT_ADDR        0x004A241D

#define PICTURE_SIZE1  (480<<16|272)     

#define W25Q64_TOPADDR       0x00800000
#define W25Q64SECTORSIZE_4K  0x00001000
#define W25Q64SPAGESIZE_256  0x00000100
#define W25Q64_TOTALSECTOR   (W25Q64_TOPADDR/W25Q64SECTORSIZE_4K)   
#define WRITEDATASPACE_1M    0x00100000
#define WRITEDATA_STARTADDR  (W25Q64_TOPADDR-WRITEDATASPACE_1M)


/*******************************************************************************
* Function Name  : Text_Location
* Description    : 字符定位，根据字符码，查找字符所在的位置
* Input          : FontSize:字体大小
                   FontType:字体类型
                   CharCode:字符编码                  
* Output         : None
* Return         : None
* Date           : 2014-10-20
* Author         : ADT LL
*******************************************************************************/
uint32_t Text_Location(uint8_t FontSize, uint8_t FontType, uint16_t CharCode)
{
  uint32_t addr = 0;
  switch (FontType)
  {
    case FONT_ASCII:
      switch (FontSize)
      {
        case FONTSIZE_16:
          addr = CHAR_ASCII16_ADDR+CharCode*16;
          break;
        case FONTSIZE_24:
          addr = CHAR_ASCII24_ADDR+CharCode*36;
          break;
        case FONTSIZE_32:
          addr = CHAR_ASCII32_ADDR+CharCode*64;
          break;
        case FONTSIZE_48:
          addr = CHAR_ASCII48_ADDR+CharCode*144;
          break;
        default:
          break;
      }
      break;
    case FONT_CHINESE:
      switch (FontSize)
      {
        case FONTSIZE_16:
          addr = CHAR_CHINESE16_ADDR+CharCode*32;
          break;
        case FONTSIZE_24:
          addr = CHAR_CHINESE24_ADDR+CharCode*72;
          break;
        case FONTSIZE_32:
          addr = CHAR_CHINESE32_ADDR+CharCode*128;
          break;
        case FONTSIZE_48:
          addr = CHAR_CHINESE48_ADDR+CharCode*288;
          break;
        default:
          break;
      }
      break;
    case FONT_SYMBOL:
      switch (FontSize)
      {
        case FONTSIZE_16:
          addr = CHAR_SYMBOL16_ADDR+CharCode*32;
          break;
        case FONTSIZE_24:
          addr = CHAR_SYMBOL24_ADDR+CharCode*72;
          break;
        case FONTSIZE_32:
          addr = CHAR_SYMBOL32_ADDR+CharCode*128;
          break;
        case FONTSIZE_48:
          addr = CHAR_SYMBOL48_ADDR+CharCode*288;
          break;
        default:
          break;
      }
      break;
    default:
      break;     
  }

  return addr;
}

/*******************************************************************************
* Function Name  : Picture_Location
* Description    : 图片定位，根据图片编号，查找所在的位置
* Input          : PictureNum:图片编号
* Output         : None
* Return         : 图片地址
* Date           : 2015-01-16
* Author         : ADT LL
*******************************************************************************/
uint32_t Picture_Location(uint8_t PictureNum)
{
  uint32_t addr = 0;
  switch (PictureNum)
  {    
    case PICTURE_1:
      addr = PICTURE1_ADDR;
      break;
    case PICTURE_2:
      addr = PICTURE2_ADDR;
      break;
    case ICON_SELECTR:
      addr = ICON_SELECTR_ADDR;
      break; 
    case ICON_SELECT:
      addr = ICON_SELECT_ADDR;
      break; 
    default:break;     
  }
  return addr;
}

/*******************************************************************************
* Function Name  : Picture_Location
* Description    : 根据图片编号，得出图片的尺寸
* Input          : PictureNum:图片编号
* Output         : None
* Return         : 图片尺寸,高16位宽度，低16位高度
* Date           : 2015-01-16
* Author         : ADT LL
*******************************************************************************/
uint32_t Get_PictureSize(uint8_t PictureNum)
{
  uint32_t Size = 0;
  switch (PictureNum)
  {    
    case PICTURE_1:
    case PICTURE_2:
      Size = PICTURE_SIZE1;
      break;
    case ICON_SELECTR:
    case ICON_SELECT:
      Size = 37<<16|31;
      break; 
    default:break;     
  }
  return Size; 
}

/*******************************************************************************
* Function Name  : W25q64_Init
* Description    : 字库芯片W25Q64初始化
* Input          : None
* Output         : None
* Return         : None
* Date           : 2014-10-20
* Author         : ADT LL
*******************************************************************************/
void W25q64_Init(void)
{
  /*
  CS1 -- PB12
  SPI -- PB13-PB15(SPI2)
  */
  SPI2_Init();  
}

/*******************************************************************************
* Function Name  : W25q_ReadWriteByte 
* Description    : W25q芯片SPI读写接口函数
* Input          : None
* Output         : None
* Return         : None
* Date           : 2014-05-14
* Author         : ADT LL
*******************************************************************************/
static uint8_t W25q_ReadWriteByte(uint8_t data)
{
  return SPI2_ReadWriteByte(data);  
}


/*******************************************************************************
* Function Name  : W25q64_WaitForIdle
* Description    : 读状态寄存器
* Input          : None
* Output         : None
* Return         : None
* Date           : 2014-10-24
* Author         : ADT LL
*******************************************************************************/
void W25q64_WaitForIdle(void)
{
  uint8_t status=0;
  CS1_RESET;

  W25q_ReadWriteByte(0x05);

  do
  {
    status = W25q_ReadWriteByte(0xA5);
  }while (status&0x01 == 0x01);

  CS1_SET;
  
}

/*******************************************************************************
* Function Name  : W25q64_WriteEnable
* Description    : 写使能
* Input          : None
* Output         : None
* Return         : None
* Date           : 2014-10-24
* Author         : ADT LL
*******************************************************************************/
void W25q64_WriteEnable(void)
{
  CS1_RESET;

  W25q_ReadWriteByte(0x06);

  CS1_SET; 
}

/*******************************************************************************
* Function Name  : W25q64_4KErase
* Description    : 4K片擦除
* Input          : addr:起始地址
* Output         : None
* Return         : None
* Date           : 2014-10-24
* Author         : ADT LL
*******************************************************************************/
void  W25q64_4KErase(uint32_t addr)
{
  W25q64_WriteEnable();

  CS1_RESET;

  W25q_ReadWriteByte(0x20);
  W25q_ReadWriteByte((addr & 0xFF0000)>>16);
  W25q_ReadWriteByte((addr & 0x00FF00)>>8);
  W25q_ReadWriteByte(addr & 0xFF);

  CS1_SET; 
  
  W25q64_WaitForIdle(); 
}

/*******************************************************************************
* Function Name  : W25q64_32KErase
* Description    : 32K片擦除
* Input          : addr:起始地址
* Output         : None
* Return         : None
* Date           : 2014-10-24
* Author         : ADT LL
*******************************************************************************/
void  W25q64_32KErase(uint32_t addr)
{
  W25q64_WriteEnable();

  CS1_RESET;

  W25q_ReadWriteByte(0x52);
  W25q_ReadWriteByte((addr & 0xFF0000)>>16);
  W25q_ReadWriteByte((addr & 0x00FF00)>>8);
  W25q_ReadWriteByte(addr & 0xFF);

  CS1_SET;
  
  W25q64_WaitForIdle();  
}

/*******************************************************************************
* Function Name  : W25q64_64KErase
* Description    : 64K片擦除
* Input          : addr:起始地址
* Output         : None
* Return         : None
* Date           : 2014-10-24
* Author         : ADT LL
*******************************************************************************/
void  W25q64_64KErase(uint32_t addr)
{
  W25q64_WriteEnable();

  CS1_RESET;

  W25q_ReadWriteByte(0xD8);
  W25q_ReadWriteByte((addr & 0xFF0000)>>16);
  W25q_ReadWriteByte((addr & 0x00FF00)>>8);
  W25q_ReadWriteByte(addr & 0xFF);

  CS1_SET;
  
  W25q64_WaitForIdle();  
}

/*******************************************************************************
* Function Name  : W25q64_ChipErase
* Description    : Chip片擦除
* Input          : addr:起始地址
* Output         : None
* Return         : None
* Date           : 2014-10-24
* Author         : ADT LL
*******************************************************************************/
void  W25q64_ChipErase(uint32_t addr)
{
  W25q64_WriteEnable();

  CS1_RESET;

  W25q_ReadWriteByte(0xC7);
  W25q_ReadWriteByte((addr & 0xFF0000)>>16);
  W25q_ReadWriteByte((addr & 0x00FF00)>>8);
  W25q_ReadWriteByte(addr & 0xFF);

  CS1_SET;
  
  W25q64_WaitForIdle();  
}

/*******************************************************************************
* Function Name  : W25q64_Erase
* Description    : 擦除操作，最小以4K为单位进行，从擦除。
* Input          : sectornum:块数,擦除的空间大小为sectornum*4K
                    --例如:如果sectornum=1，则擦除4K字节，
                      sectornum=3，则擦除12K=4K*3个字节
                   addr:起始地址
* Output         : None
* Return         : 状态码
* Date           : 2014-10-25
* Author         : ADT LL
*******************************************************************************/
uint8_t W25q64_Erase(uint16_t sectornum,uint32_t addr)
{
  uint8_t blocknum;

  if (addr>=W25Q64_TOPADDR) return W25Q64_ADDRERROR;
  if (sectornum<=0 || sectornum>W25Q64_TOTALSECTOR) return W25Q64_ERASEERROR;//没有擦除空间不合理不操作
  
  blocknum = sectornum/16;
  while (blocknum--)//以64K为单位擦除
  {
    W25q64_64KErase(addr);
    addr += W25Q64SECTORSIZE_4K*16;
    sectornum -= 16;   
  }
  blocknum = sectornum/8;
  while (blocknum--)//以32K为单位擦除
  {
    W25q64_32KErase(addr);
    addr += W25Q64SECTORSIZE_4K*8;
    sectornum -= 8;   
  }
  blocknum = sectornum;  
  while (blocknum--)//以4K为单位擦除
  {
    W25q64_4KErase(addr);
    addr += W25Q64SECTORSIZE_4K;   
  }

  return W25Q64_ERASEOK; 
}

/*******************************************************************************
* Function Name  : W25q64_Erase
* Description    : 擦除操作，最小以4K为单位进行，从擦除。
* Input          : sectornum:块数,擦除的空间大小为sectornum*4K
                   addr:起始地址,相对于可写的顶部1M空间的偏移量
* Output         : None
* Return         : None
* Date           : 2014-10-25
* Author         : ADT LL
*******************************************************************************/
void W25q64_EraseTOP1MSapce(uint16_t sectornum,uint32_t addr)
{
  uint32_t startaddr = WRITEDATA_STARTADDR + (addr&0x000FFFFF);
   
  W25q64_Erase(sectornum, startaddr);    
}

/*******************************************************************************
* Function Name  : W25q64_PageProgram
* Description    : 页写
* Input          : buf:要写内的数据
                   len:数据长度
                   add:起始地址
* Output         : None
* Return         : 状态码
* Date           : 2014-10-24
* Author         : ADT LL
*******************************************************************************/
static uint8_t  W25q64_PageProgram(uint8_t *buf, uint16_t len, uint32_t addr)
{
  if (addr>=W25Q64_TOPADDR) return W25Q64_ADDRERROR;
  if (len<=0 || len>256) return W25Q64_WRITEDATAERROR;//没有数据不操作

  W25q64_WriteEnable();

  CS1_RESET;

  W25q_ReadWriteByte(0x02);
  W25q_ReadWriteByte((addr & 0xFF0000)>>16);
  W25q_ReadWriteByte((addr & 0x00FF00)>>8);
  W25q_ReadWriteByte(addr & 0xFF);

  while (len--)
  {
    W25q_ReadWriteByte(*buf);
    buf++;   
  }

  CS1_SET;
  
  W25q64_WaitForIdle();
  
  return W25Q64_WRITEDATAOK;   
}

/*******************************************************************************
* Function Name  : W25q64_Write
* Description    : 数据存储,为防止意外的擦除
* Input          : buf:要写内的数据
                   len:数据长度
                   add:起始地址
* Output         : None
* Return         : None
* Date           : 2014-10-24
* Author         : ADT LL
*******************************************************************************/
void W25q64_Write(uint8_t *buf, uint16_t len, uint32_t addr)
{
  uint8_t pagenum;
  uint8_t addrbyte;//最低八位地址
  
  addrbyte = addr%W25Q64SPAGESIZE_256;
  if (len > (W25Q64SPAGESIZE_256-addrbyte))//跨页了
  {
      W25q64_PageProgram(buf, W25Q64SPAGESIZE_256-addrbyte, addr);//写满本页
      addr += W25Q64SPAGESIZE_256-addrbyte;
      buf += W25Q64SPAGESIZE_256-addrbyte;
      len -= W25Q64SPAGESIZE_256-addrbyte;
      pagenum = len/W25Q64SPAGESIZE_256;
 
      while (pagenum--)
      {
        W25q64_PageProgram(buf, W25Q64SPAGESIZE_256, addr);
        addr += W25Q64SPAGESIZE_256;
        buf += W25Q64SPAGESIZE_256;
        len -= W25Q64SPAGESIZE_256;  
      }
      W25q64_PageProgram(buf, len, addr);
      
  }
  else
  {
    W25q64_PageProgram(buf, len, addr);
   
  } 
}

/*******************************************************************************
* Function Name  : W25q64_WriteTop1MSpace
* Description    : 数据存储
* Input          : buf:要写内的数据
                   len:数据长度
                   add:起始地址,相对于可写空间的偏移量
* Output         : None
* Return         : None
* Date           : 2014-10-24
* Author         : ADT LL
*******************************************************************************/
void W25q64_WriteTop1MSpace(uint8_t *buf, uint16_t len, uint32_t addr)
{
  uint32_t startaddr = WRITEDATA_STARTADDR + (addr&0x000FFFFF);
   
  W25q64_Write(buf, len, startaddr);  
}

/*******************************************************************************
* Function Name  : W25q64_WriteStatus
* Description    : 写状态寄存器
* Input          : status:状态值
* Output         : None
* Return         : None
* Date           : 2014-10-24
* Author         : ADT LL
*******************************************************************************/
void W25q64_WriteStatus(uint16_t status)
{
  W25q64_WriteEnable();

  CS1_RESET;

  W25q_ReadWriteByte(0x01);
  W25q_ReadWriteByte(status & 0xFF);
  W25q_ReadWriteByte((status>>8) & 0xFF);

  CS1_SET;

  W25q64_WaitForIdle();  
  
}

/*******************************************************************************
* Function Name  : W25q64_ReadStatus
* Description    : 读状态寄存器
* Input          : None
* Output         : None
* Return         : 状态寄存器值
* Date           : 2014-10-24
* Author         : ADT LL
*******************************************************************************/
uint16_t W25q64_ReadStatus(void)
{
  uint16_t status=0;
  CS1_RESET;

  W25q_ReadWriteByte(0x05);
  status = W25q_ReadWriteByte(0xFF);

  CS1_SET;

  CS1_RESET;
  W25q_ReadWriteByte(0x35);
  status |= W25q_ReadWriteByte(0xFF)<<8;
  CS1_SET;

  
  return status;  
}

/*******************************************************************************
* Function Name  : W25q64_ReadDeviceID
* Description    : 读字库芯片ID
* Input          : None
* Output         : None
* Return         : None
* Date           : 2014-10-20
* Author         : ADT LL
*******************************************************************************/
uint16_t W25q64_ReadDeviceID(void)
{
  uint16_t DeviceID=0;

  CS1_RESET;

  W25q_ReadWriteByte(0x90);
  W25q_ReadWriteByte(0x00);
  W25q_ReadWriteByte(0x00);
  W25q_ReadWriteByte(0x00);

  DeviceID = W25q_ReadWriteByte(0xFF)<<8;
  DeviceID |= W25q_ReadWriteByte(0xFF);

  CS1_SET;

  return DeviceID;
}

/*******************************************************************************
* Function Name  : W25q64_Read
* Description    : 读数据
* Input          : buf:数据缓冲区
                   len:长度
                   addr:起始地址
* Output         : None
* Return         : 返回状态，暂无意义
* Date           : 2014-10-20
* Author         : ADT LL
*******************************************************************************/
uint8_t W25q64_Read(uint8_t *buf, uint32_t len, uint32_t addr)
{
  uint32_t i;
  uint8_t status;

  if (addr>=W25Q64_TOPADDR) return W25Q64_ADDRERROR;
  CS1_RESET;
    
  status = W25q_ReadWriteByte(CODE_READDATA);
  status = W25q_ReadWriteByte((addr & 0xFF0000)>>16);
  status = W25q_ReadWriteByte((addr & 0x00FF00)>>8);
  status = W25q_ReadWriteByte(addr & 0xFF);
  for (i=0; i<len; i++)
  {
    buf[i] = W25q_ReadWriteByte(0xFF);
  }
  buf[i] = '\0';

  status = W25Q64_READOK;

  CS1_SET;

  return status;
}

/*******************************************************************************
* Function Name  : W25q64_ReadOneChar
* Description    : 从字库中读一个字符
* Input          : buf:数据缓冲区
                   FontSize:字体大小
                   FontType:字体类型
                   CharCode:字符编码
* Output         : None
* Return         : None
* Date           : 2014-10-20
* Author         : ADT LL
*******************************************************************************/
void W25q64_ReadOneChar(uint8_t *buf, uint8_t FontSize, uint8_t FontType, uint16_t CharCode)
{
  uint32_t addr;

  addr = Text_Location(FontSize, FontType, CharCode);
  switch (FontType)
  {
    case FONT_ASCII:
      W25q64_Read(buf,FontSize/2*FontSize/8, addr);
      break;
    case FONT_CHINESE:
      W25q64_Read(buf,FontSize*FontSize/8, addr);
      break;
    case FONT_SYMBOL:
      W25q64_Read(buf,FontSize*FontSize/8, addr);
      break;
    default:
      break;
  }      
}

/*******************************************************************************
* Function Name  : W25q64_ReadPicture
* Description    : 从字库中读一副图片的指定区域
* Input          : buf:数据缓冲区
                   Size:图片尺寸，高半字宽度，低半字是高度
                   PictureNum:图片编号
                   Position:区域在图片中的起始位置，高半字X轴，低半字是Y轴
* Output         : None
* Return         : None
* Date           : 2014-10-20
* Author         : ADT LL
*******************************************************************************/
void W25q64_ReadPicture(uint8_t *buf, uint32_t Size, uint8_t PictureNum, uint32_t Position)
{
  uint32_t addr;
  uint16_t Px,Py;
  uint16_t width,height;
  uint16_t Pwidth,PHeight;
  uint16_t i;

  Px = (Position>>16) & 0xFFFF;
  Py = Position & 0xFFFF;
  width = (Size>>16) & 0xFFFF;
  height = Size & 0xFFFF; 
  Pwidth = Get_PictureSize(PictureNum)>>16 & 0xFFFF;
  PHeight = Get_PictureSize(PictureNum) & 0xFFFF;
  Px = (Px<Pwidth) ? Px : (Px-Pwidth);
  Py = (Py<PHeight) ? Py : (Py-PHeight);


  addr = Picture_Location(PictureNum);
  addr += (Px*PHeight+Py)*3;
  for (i=0; i<width; i++)
  {
    W25q64_Read(&buf[i*height*3],height*3, addr);
    addr += PHeight*3;
  }   
}


/*******************************************************************************
* Function Name  : W25q64_Test 
* Description    : 测试函数
* Input          : None
* Output         : None
* Return         : None
* Date           : 2014-10-27
* Author         : ADT LL
*******************************************************************************/
void W25q64_Test(void)
{
  uint16_t status;  
  uint16_t startline = FONTSIZE_16+1;
  uint16_t startcolumn = 0;

  LCD_Clear(ColorTable[BACKGROUND_COLOUR]);
  Display_Character(100,0,0,FONTSIZE_16, 0, "W25q64读写测试");

  status = W25q64_ReadStatus();
  sprintf((char *)ucResult, "0x%x", status);  
  Display_Character(startcolumn,startline,0,FONTSIZE_32, 0, ucResult);

}

/* ************ ****** ************ THE FILE END ************ ****** ************ */
