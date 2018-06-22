/**
  ******************************************************************************
  * @file    font.c
  * @author  LL
  * @version V0.0.0
  * @date    2014-10-20
  * @brief   字库操作，所有操作都是通过SPI接口进行的。
  ******************************************************************************
  ******************************************************************************
***/

#include "stm32f10x.h"
#include "main.h"
#include "w25q.h"
#include "font.h"

/*字符地址定义*/
#define CHAR_ASCII16_ADDR   0x00000000
#define CHAR_CHINESE16_ADDR 0x00001800
#define CHAR_ASCII24_ADDR   0x00000600
#define CHAR_CHINESE24_ADDR 0x00043800
#define CHAR_ASCII32_ADDR   0x000AD0CC
#define CHAR_CHINESE32_ADDR 0x000D3800
#define CHAR_ASCII48_ADDR   0x0018208C
#define CHAR_CHINESE48_ADDR 0x001855FE


/*******************************************************************************
* Function Name  : Font_CharLocation
* Description    : 字符定位，根据字符码，查找字符所在的位置
* Input          : FontType:字体类型
                   CharCode:字符编码                  
* Output         : None
* Return         : None
* Date           : 2018-04-19
* Author         : LL
*******************************************************************************/
static uint32_t Font_CharLocation(uint8_t FontType, uint16_t CharCode)
{
    uint32_t addr = 0;
    
    switch (FontType)
    {
        case FONT_ASCII16:
            addr = CHAR_ASCII16_ADDR+CharCode*16;
            break;
        case FONT_ASCII24:
            addr = CHAR_ASCII24_ADDR+CharCode*48;
            break;
        case FONT_CHINESE16:
            addr = CHAR_CHINESE16_ADDR+CharCode*32;
            break;
        case FONT_CHINESE24:
            addr = CHAR_CHINESE24_ADDR+CharCode*72;
            break;
        case FONT_CHINESE32:
            addr = CHAR_CHINESE32_ADDR+CharCode*128;
            break;
        default:
            break;     
    }

    return addr;
}

/*******************************************************************************
* Function Name  : Font_CharSize
* Description    : 根据字符码，确定字符大小
* Input          : FontType:字体类型
* Output         : None
* Return         : 字符大小，高字节字符高度H，低字节字符宽度W
* Date           : 2018-04-19
* Author         : LL
*******************************************************************************/
uint16_t Font_CharSize(uint8_t FontType)
{
    uint16_t size = 0;
    switch (FontType)
    {
        case FONT_ASCII16:
            size = 16<<8 | 8;
            break;
        case FONT_ASCII24:
            size = 24<<8 | 16;
            break;
        case FONT_CHINESE16:
            size = 16<<8 | 16;
            break;
        case FONT_CHINESE24:
            size = 24<<8 | 24;
            break;
        case FONT_CHINESE32:
            size = 32<<8 | 32;
            break;
        default:
            break;     
    }
   
    return size;
}

/*******************************************************************************
* Function Name  : Font_CharPos
* Description    : 根据字符码，确定字符在其字库中的位值
* Input          : FontType:字体类型
                   c:字符
* Output         : None
* Return         : 字符位值
* Date           : 2018-04-19
* Author         : LL
*******************************************************************************/
uint16_t Font_CharPos(uint8_t FontType, uint8_t *c)
{
    uint16_t pos = 0;
    
    switch (FontType)
    {
        case FONT_ASCII16:
        case FONT_ASCII24:
            pos = c[0] - ' ';
            break;
        case FONT_CHINESE16:
        case FONT_CHINESE24:
        case FONT_CHINESE32:
            pos = (c[0] - 0xA1) * 94 + (c[1] - 0xA1);
            break;
        default:
            break;     
    }

    return pos;
}

/*******************************************************************************
* Function Name  : Font_CharType
* Description    : 根据字符码和字体大小，确定字体类型
* Input          : FontSize:字体大小
                   c:字符
* Output         : None
* Return         : 字体类型
* Date           : 2018-04-19
* Author         : LL
*******************************************************************************/
uint8_t Font_CharType(uint8_t FontSize, uint8_t *c)
{
    uint8_t ftype = 0;

    if(c[0] >= 0xA1)
    {
        switch (FontSize)
        {
            case FONT_SIZE16:
                ftype = FONT_CHINESE16;
                break;
            case FONT_SIZE24:
                ftype = FONT_CHINESE24;
                break;
            case FONT_SIZE32:
                ftype = FONT_CHINESE32;
                break;
            default:
                break;     
        }
    }  			
    else if(c[0] <= 0x7F)
    {
        switch (FontSize)
        {
            case FONT_SIZE16:
                ftype = FONT_ASCII16;
                break;
            case FONT_SIZE24:
                ftype = FONT_ASCII24;
                break;
            default:
                break;     
        }
    }
    else
    {
    }
    
    return ftype;
}


/*******************************************************************************
* Function Name  : Font_DotConvert
* Description    : 将先上后下，从左到右字符，转为先左后右，从上到下
                   且转换后按字节，先上部后下部
* Input          : FontType:字体类型
                   c:字符点阵数据
* Output         : None
* Return         : 字符位值
* Date           : 2018-04-19
* Author         : LL
*******************************************************************************/
void Font_DotConvert(uint8_t FontType, uint8_t *c)
{
    uint8_t  buf[256];    
    uint8_t  h_size;//字符高度
    uint8_t  w_size;//字符宽度
    uint8_t  dbyte;
    uint8_t  b[8];
    uint8_t  *p;
    uint16_t size;
    uint16_t  i,j,k,m;
    size = Font_CharSize(FontType);
    h_size = (size>>8) & 0xFF;
    w_size = size & 0xFF;

    dbyte = 0;
    p = buf;
    for (k=0; k<h_size/8; k++)
    {
        for (j=0; j<w_size; j++)//
        {
            for (i=0; i<8; i++)
            {
                m = (k*8+i)*w_size/8+j/8;
                b[i] = (c[m] & (0x80>>(j%8)))>>(7-(j%8));
                //b[i] = (c[m] & (0x80>>(j%8)))>>(j%8);
            }
            dbyte = 0;
            for (i=0; i<8; i++)
            {
                dbyte |= b[i]<<(7-i);
            }
            *p++ = dbyte;
        }
    }
    for (i=0; i< h_size*w_size/8; i++)
    {
        c[i] = buf[i];
    }
}

/*******************************************************************************
* Function Name  : Font_ReadOneChar
* Description    : 从字库中读一个字符
* Input          : buf:数据缓冲区
                   FontType:字体类型
                   CharCode:字符编码
* Output         : None
* Return         : None
* Date           : 2018-04-19
* Author         : LL
*******************************************************************************/
void Font_ReadOneChar(uint8_t *buf, uint8_t FontType, uint16_t CharCode)
{
    uint8_t h_size;//字符高度
    uint8_t w_size;//字符宽度
    uint16_t size;
    uint32_t addr;

    addr = Font_CharLocation(FontType, CharCode);
    size = Font_CharSize(FontType);
    h_size = (size>>8) & 0xFF;
    w_size = size & 0xFF;
    W25q_Read(buf,h_size*w_size/8, addr);
    Font_DotConvert(FontType, buf);
}

/* ************ ****** ************ THE FILE END ************ ****** ************ */
