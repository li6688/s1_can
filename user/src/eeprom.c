/**
  ******************************************************************************
  * @file    eeprom.c
  * @author  LL
  * @version V0.0.0
  * @date    2017-05-19
  * @brief   eeprom读写操作
  ******************************************************************************
  ******************************************************************************
***/

#define __EEPROM_C__
#include "config.h"


/*******************************************************************************
* Function Name  : Eeprom_ReadOneByte 
* Description    : 从eeprom中读一个字节的数据
 * 读数据流程
 * 1. 将NMVREGS置1，访问eeprom。
 * 2. 将所需地址写入NVMADRH:NVMADRL寄存器对。
 * 3. 将NVMCON1寄存器的RD位置1可启动读操作。
 * 4. 从NVMDATH:NVMDATL寄存器对中读出数据。
* Input          : 
 * addr:地址(范围0-0xFF)
 *      如果使用直接地址则地址范围0xF000-0xF0FF
 * data:读出的数据 
* Output         : None
* Return         : None
* Date           : 2017-05-19
* Author         : LL
*******************************************************************************/
static void Eeprom_ReadOneByte(uint16_t addr, uint8_t *data)
{
    //可以使用直接地址，屏蔽这两行语句，以减少代码量
    //if (addr>0xFF) return;//地址超出范围
    //addr += 0x7000;//eeprom的起始地址是0x7000

	NVMADRL = (addr & 0xFF);//设置访问的地址
    NVMADRH = ((addr>>8) & 0xFF);
    NVMCON1bits.NVMREGS = 1;//设置为访问eeprom
    NVMCON1bits.RD = 1;//启动读操作
    NOP();//对于高速系统可能需要
	NOP();
    
    *data = NVMDATL;
}

/*******************************************************************************
* Function Name  : Eeprom_WriteOneByte 
* Description    : 向eeprom中写入一个字节的数据
 * 数据的写入流程
 * 1. 将 NVMCON1 寄存器的 NVMREGS 和 WREN 位置1。
 * 2. 将所需地址（地址+7000h）写入NVMADRH:NVMADRL寄存器对。
 * 3. 执行解锁序列。
 * 4.使用NVMDATA 写单个EEPROM 字节。
 * 5.软件查询WR位来确定写操作何时完成
* Input          : 
 * addr:地址(范围0-0xFF)
 *      如果使用直接地址则地址范围0xF000-0xF0FF.
 * data:写入的数据 
* Output         : None
* Return         : None
* Date           : 2017-05-19
* Author         : LL
*******************************************************************************/
static void Eeprom_WriteOneByte(uint16_t addr, uint8_t data)
{
	uint8_t GIEBitValue = INTCONbits.GIE;
	
    //可以使用直接地址，屏蔽这两行语句，以减少代码量
    //if (addr>0xFF) return;//地址超出范围
    //addr += 0x7000;//eeprom的起始地址是0x7000

	NVMADRL = (addr & 0xFF);//设置访问的地址
    NVMADRH = ((addr>>8) & 0xFF);
	NVMDATL = data;
    //NVMDATH = 0;
    NVMCON1bits.NVMREGS = 1;//设置为访问eeprom
    NVMCON1bits.WREN = 1;
    INTCONbits.GIE = 0;//关闭中断。解锁序列，不可被中断
    NVMCON2 = 0x55;//解锁
    NVMCON2 = 0xAA;
    NVMCON1bits.WR = 1;
	INTCONbits.GIE = GIEBitValue; //回复中断状态
    
    while(0!=NVMCON1bits.WR);//等待写完成
    
}

/*******************************************************************************
* Function Name  : Eeprom_Read 
* Description    : 从eeprom中读数据
* Input          : addr:地址
*                  buf:写入的数据 
*                  len:数据长度
* Output         : None
* Return         : None
* Date           : 2017-05-19
* Author         : LL
*******************************************************************************/
void Eeprom_Read(uint16_t addr, uint8_t *buf, uint16_t len)
{
    uint16_t i;
	
    for(i=0; i<len; i++)
    {
        Eeprom_ReadOneByte(addr+i,buf+i);
    }
}

/*******************************************************************************
* Function Name  : Eeprom_Write 
* Description    : 向eeprom中写数据
* Input          : addr:地址
*                  buf:写入的数据 
*                  len:数据长度 
* Output         : None
* Return         : None
* Date           : 2017-05-19
* Author         : LL
*******************************************************************************/
void Eeprom_Write(uint16_t addr, uint8_t *buf, uint16_t len)
{
    uint16_t i;
    
    for(i=0; i<len; i++)
    {
        Eeprom_WriteOneByte(addr+i,buf[i]);
    }
}

/*******************************************************************************
* Function Name  : Eeprom_Test 
* Description    : 测试
* Input          : None
* Output         : None
* Return         : None
* Date           : 2017-05-19
* Author         : LL
*******************************************************************************/
void Eeprom_Test(void)
{
	uint8_t i;
	uint8_t buf[10];
	for(i=0;i<5;i++)
	{
		buf[i] = i+'0';
	}
	Eeprom_Write(0,buf,i);
	for(i=0;i<5;i++)
	{
		buf[i] = 0;
	}
    Eeprom_Read(0,buf,i);
	DEBUG_PRINTFMASSAGESHEX(buf,i);
}

/* ************ ****** ************ THE FILE END  ************ ****** ************ */