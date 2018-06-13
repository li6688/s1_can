/**
  ******************************************************************************
  * @file    systick.c
  * @author  LL
  * @version V0.0.0
  * @date    2018-03-27
  * @brief   综合配置处理，时钟，中断，端口等初始化配置
  ******************************************************************************
  ******************************************************************************
***/
#include <stdint.h>
#include "stm32f4xx.h"

uint32_t SysTickCountValue; //系统定计数器计数值

/*******************************************************************************
* Function Name  : SysTick_Handler
* Description    : This function handles SysTick fault exception,周期10ms.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Handler(void)
{
  do
  {
    SysTickCountValue++;  //计数值加1
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

    SysTick->LOAD = value & 0x00FFFFFF; //时间加载(SysTick->LOAD为24bit)
	SysTick->VAL  = 0x00; //清空计数器
	SysTick->CTRL |= 0x03;//开始倒数,使能中断  
}

/* ************ ****** ************ THE FILE END ************ ****** ************ */
