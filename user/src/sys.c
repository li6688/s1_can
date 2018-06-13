/**
  ******************************************************************************
  * @file    sys.c
  * @author  LL
  * @version V0.0.0
  * @date    2014-04-23
  * @brief   
  ******************************************************************************
  ******************************************************************************
***/

#include <stdint.h>
#include "sys.h"

Flag_StructType Flag;  //全局标志变量
GlobalVariable_StructType GlobalVar;
#if SYSTICKCOUNTVALUE_EN //有些编译器会对没有使用的全局变量警告，不使用的时候禁止，可以去掉警告
unsigned long int SysTickCountValue;
#endif
#if HEXTOASCIITAB_EN
uint8_t HexToAsciiTab[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
#endif

/*******************************************************************************
* Function Name  : NumberToAscii
* Description    : 将一个数字转化为字符串，对于为零的数也会按要求的位数进行转化。
                   转化为十进制，
                   方法:先计算出转化的位数，在进行转化
                   例如：对数字0，如果要求转化为2位，则转化为"00"。
                         对数字1，如果要求转化为6位，则转化为"000001"。
                         对数字2014，如果要求转化为6位，则转化为"002014"。
                         对数字123456789，如果要求转化为6位，则转化为"456789"
                    如果len为0xFF则按照自由转化 自动按位数转化 0转化为'0'
                    例如：198 转化为"198"
                   注意32位整形的最大值为4294967295，10位数
* Input          : buf:字符串存储区
                   number:要转化的数字
                   len:数字的位数
* Output         : None
* Return         : None
* Date           : 2014-09-05
* Author         : LL
*******************************************************************************/
void NumberToAscii(uint8_t *buf, uint32_t number, uint8_t len)
{
	uint8_t i;
	uint32_t temp;
	
	if (0xFF == len)
	{
		//自由转化，计算转化的位数
		i=0;
		temp = number;
		do
		{
			//至少转化1位，数据0的时候会转化为'0'是1位
			i++;
			temp = temp/10;
		}while(0!=temp);
		len = i;   
	}
	if (len>10) len=10;
	
	for (i=0; i<len; i++)
	{
		buf[len-1-i] = number%10 + '0';
		number = number/10;
	}
	buf[i]= '\0';
}
#if HEXTOASCIITAB_EN
/*******************************************************************************
* Function Name  : NumberToAsciiHEX
* Description    : 将一个数字转化为字符串，对于为零的数也会按要求的位数进行转化。
                   转化为16进制字符串
                   方法:先完全转化，然后计算要求的长度，然后截取要求的数据
                   例如：对数字0x0，如果要求转化为2位，则转化为"0x00"。
                         对数字0x1，如果要求转化为6位，则转化为"0x000001"。
                         对数字0x2014，如果要求转化为6位，则转化为"0x002014"。
                         对数字0x12345678，如果要求转化为6位，则转化为"0x345678"
                    如果len为0xFF则按照自由转化 自动按位数转化 0x0转化为0x0
                    例如：0x198 转化为"0x198"
                   注意32位整形的最大值为0xFFFFFFFF，8位16进制数
* Input          : buf:字符串存储区
                   number:要转化的数字
                   len:数字的位数
* Output         : None
* Return         : None
* Date           : 2017-02-28
* Author         : LL
*******************************************************************************/
void NumberToAsciiHEX(uint8_t *buf, uint32_t number, uint8_t len)
{
	unsigned char i,j;
	unsigned char buft[9];
	
	for(i=0; i<8; i++)
	{
		j= (number>>(7-i)*4) & 0x0f;
		buft[i] = HexToAsciiTab[j]; 
	}
	
	if (0xFF==len)
	{
		/*
		最少转化1位数据，所以计算7次就可以了，不管数据是不是0，最后1位都要显示出来
		*/
		for (i=0;i<7;i++)
		{
			if ('0'!=buft[i]) break;
		}
		len = 8-i;
	}
	
	if (len>8) len=8;
	
	buf[0] = '0';
	buf[1] = 'x';
	for (i=0; i<len; i++)
	{
		buf[i+2] = buft[i+8-len];
	}
	buf[i+2]= '\0';
}
#endif /* HEXTOASCIITAB_EN */
#if SYSTICKCOUNTVALUE_EN        
/*******************************************************************************
* Function Name  : delay_10ms
* Description    : 延时一段时间.延时时间=value*10毫秒。
* Input          : value: 延时的10ms 数.
* Output         : None
* Return         : None
*******************************************************************************/
void Delay_10ms(uint32_t value)
{
  uint32_t Dtime;
  Dtime = SysTickCountValue;
  while (SysTickCountValue-Dtime < value);
}

/*******************************************************************************
* Function Name  : Read_UserTimer
* Description    : 读用户时间，查看用户过了多长时间，用于用户延时.
* Input          : time: 用户时间值.
* Output         : None
* Return         : 用户时间与系统时间的差值
*******************************************************************************/
uint32_t Read_UserTimer(uint32_t time)
{
  return (SysTickCountValue-time);
}

/*******************************************************************************
* Function Name  : Reset_UserTimer
* Description    : 设置用户时间，将当前的用户时间复位到系统时间.
* Input          : time: 用户时间值.
* Output         : None
* Return         : None
*******************************************************************************/
void Reset_UserTimer(uint32_t *time)
{
  *time = SysTickCountValue;
}
#endif /* SYSTICKCOUNTVALUE_EN */
/* ************ ****** ************ THE FILE END  ************ ****** ************ */