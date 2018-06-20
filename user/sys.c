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
#include <stdbool.h>
#include "sys.h"

Flag_StructType Flag;  //ȫ�ֱ�־����
GlobalVariable_StructType GlobalVar;

#if HEXTOASCIITAB_EN
uint8_t HexToAsciiTab[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
#endif

/*******************************************************************************
* Function Name  : NumberToAscii
* Description    : ��һ������ת��Ϊ�ַ���������Ϊ�����Ҳ�ᰴҪ���λ������ת����
                   ת��Ϊʮ���ƣ�
                   ����:�ȼ����ת����λ�����ڽ���ת��
                   ���磺������0�����Ҫ��ת��Ϊ2λ����ת��Ϊ"00"��
                         ������1�����Ҫ��ת��Ϊ6λ����ת��Ϊ"000001"��
                         ������2014�����Ҫ��ת��Ϊ6λ����ת��Ϊ"002014"��
                         ������123456789�����Ҫ��ת��Ϊ6λ����ת��Ϊ"456789"
                    ���lenΪ0xFF��������ת�� �Զ���λ��ת�� 0ת��Ϊ'0'
                    ���磺198 ת��Ϊ"198"
                   ע��32λ���ε����ֵΪ4294967295��10λ��
* Input          : buf:�ַ����洢��
                   number:Ҫת��������
                   len:���ֵ�λ��
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
		//����ת��������ת����λ��
		i=0;
		temp = number;
		do
		{
			//����ת��1λ������0��ʱ���ת��Ϊ'0'��1λ
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
* Description    : ��һ������ת��Ϊ�ַ���������Ϊ�����Ҳ�ᰴҪ���λ������ת����
                   ת��Ϊ16�����ַ���
                   ����:����ȫת����Ȼ�����Ҫ��ĳ��ȣ�Ȼ���ȡҪ�������
                   ���磺������0x0�����Ҫ��ת��Ϊ2λ����ת��Ϊ"0x00"��
                         ������0x1�����Ҫ��ת��Ϊ6λ����ת��Ϊ"0x000001"��
                         ������0x2014�����Ҫ��ת��Ϊ6λ����ת��Ϊ"0x002014"��
                         ������0x12345678�����Ҫ��ת��Ϊ6λ����ת��Ϊ"0x345678"
                    ���lenΪ0xFF��������ת�� �Զ���λ��ת�� 0x0ת��Ϊ0x0
                    ���磺0x198 ת��Ϊ"0x198"
                   ע��32λ���ε����ֵΪ0xFFFFFFFF��8λ16������
* Input          : buf:�ַ����洢��
                   number:Ҫת��������
                   len:���ֵ�λ��
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
		����ת��1λ���ݣ����Լ���7�ξͿ����ˣ����������ǲ���0�����1λ��Ҫ��ʾ����
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
* Description    : ��ʱһ��ʱ��.��ʱʱ��=value*10���롣
* Input          : value: ��ʱ��10ms ��.
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
* Description    : ���û�ʱ�䣬�鿴�û����˶೤ʱ�䣬�����û���ʱ.
* Input          : time: �û�ʱ��ֵ.
* Output         : None
* Return         : �û�ʱ����ϵͳʱ��Ĳ�ֵ
*******************************************************************************/
uint32_t Read_UserTimer(uint32_t time)
{
  return (SysTickCountValue-time);
}

/*******************************************************************************
* Function Name  : Reset_UserTimer
* Description    : �����û�ʱ�䣬����ǰ���û�ʱ�临λ��ϵͳʱ��.
* Input          : time: �û�ʱ��ֵ.
* Output         : None
* Return         : None
*******************************************************************************/
void Reset_UserTimer(uint32_t *time)
{
  *time = SysTickCountValue;
}
#endif /* SYSTICKCOUNTVALUE_EN */
/* ************ ****** ************ THE FILE END  ************ ****** ************ */