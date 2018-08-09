/**
  ******************************************************************************
  * @file    systemcommon.c
  * @author  LL
  * @version V0.0.0
  * @date    2014-09-09
  * @brief   综合配置处理，时钟，中断，端口等初始化配置
  ******************************************************************************
  ******************************************************************************
***/
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"
#include "sys.h"
#include "systick.h"
#include "timer.h"
#include "uart.h"

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void RCC_Configuration(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* Connect PXx to USARTx_Tx*/
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART2);

    /* Connect PXx to USARTx_Rx*/
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART2);

    /* Configure USART Tx as alternate function  */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* Configure USART Rx as alternate function  */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configure the nested vectored interrupt controller.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置优先级组

    /*设置中断向量表的位置*/
    //SCB->VTOR &= ~0x1FFFFF80;

    /* Enable the TIM3 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


//JTAG模式设置,用于设置JTAG的模式
//mode:jtag,swd模式设置;0x0:复位状态;    0x1:全使能无NJTRST;    0x2:使能SWD;  0x4:全关闭；
static void JTAG_Set(u8 mode)
{
    uint32_t temp;
    bool ref = false;
    if(0x01 == mode)
    {
        temp = (mode<<24) & ~0XF8FFFFFF;
        ref = true;
    }
    else if (0x02 == mode)
    {
        temp = (mode<<24) & ~0XF8FFFFFF;
        ref = true;
    }
    else if (0x04 == mode)
    {
        temp = (mode<<24) & ~0XF8FFFFFF;
        ref = true;
    }
    else
    {
        return;
    }

    if (true == ref)
    {
        temp = temp;
        //AFIO->MAPR |= temp;
    }
}


/*******************************************************************************
* Function Name  : Basic_Configuration
* Description    : 基本的初始化配置.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Basic_Configuration(void)
{
  GlobalVar.ResetSource = RCC->CSR;
  /*配置需要开启的时钟*/
  RCC_Configuration();

  /*Config the systick period is 10ms*/
  SysTick_Configuration(239999);
  
  /* 配置需要的IO */
  GPIO_Configuration();

  /*设置JTAG为SWD模式*/
  JTAG_Set(0x2);
}

/*******************************************************************************
* Function Name  : Configuration_Init
* Description    : 基本的初始化配置.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Configuration_Init(void)
{
  Basic_Configuration();
  
  /*开启串口*/
  UART_Configuration();
  
  TIM_Configuration();
  
  /* 配置中断 */
  NVIC_Configuration();

}

/*******************************************************************************
* Function Name  : IWDG_ReloadCounter
* Description    : 看门狗重新计数
* Input          : None
* Output         : None
* Return         : None
* Date           : 2014-09-09
* Author         : LL
*******************************************************************************/
void IWDG_ReloadCounter(void)
{
  IWDG->KR = 0xAAAA;
}

/*******************************************************************************
* Function Name  : IWDG_Configuration
* Description    : 独立看门狗配置，并启动
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void IWDG_Configuration(void)
{
  IWDG->KR = 0x5555; 
  IWDG->PR = 0x07;
  IWDG->RLR = 781;
  IWDG->KR = 0xAAAA;
  IWDG->KR = 0xCCCC;
}

/* ************ ****** ************ THE FILE END ************ ****** ************ */
