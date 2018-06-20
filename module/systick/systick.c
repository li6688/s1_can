/**
  ******************************************************************************
  * @file    systick.c
  * @author  LL
  * @version V0.0.0
  * @date    2018-03-27
  * @brief   �ۺ����ô���ʱ�ӣ��жϣ��˿ڵȳ�ʼ������
  ******************************************************************************
  ******************************************************************************
***/
#include <stdint.h>
#include "stm32f4xx.h"

uint32_t SysTickCountValue; //ϵͳ������������ֵ

/*******************************************************************************
* Function Name  : SysTick_Handler
* Description    : This function handles SysTick fault exception,����10ms.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Handler(void)
{
  do
  {
    SysTickCountValue++;  //����ֵ��1
  }while(0);//SysTickCountValue == 0);  
}

/*******************************************************************************
* Function Name  : SysTick_Configuration
* Description    : Configures the SysTick.
* Input          : value: the SysTick LOAD Register loading value.
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Configuration(uint32_t value)
{
    /*Set the SysTick CLK is HCLK/8*/
    SysTick->CTRL &= ~(1<<2);

    SysTick->LOAD = value & 0x00FFFFFF; //ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL  = 0x00; //��ռ�����
	SysTick->CTRL |= 0x03;//��ʼ����,ʹ���ж�  
}

/* ************ ****** ************ THE FILE END ************ ****** ************ */
