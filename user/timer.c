/**
  ******************************************************************************
  * @file    timer.c
  * @author  LL
  * @version V0.0.0
  * @date    2014-04-23
  * @brief   
  ******************************************************************************
  ******************************************************************************
***/
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"
#include "main.h"
#include "sys.h"

/*******************************************************************/
//全局变量声明区 

/********************************************************************/
//私有变量声明区
typedef struct
{
  uint32_t test_time;//test
  uint32_t adc_time;    //ADC 采样数据读取间隔 
  uint32_t key_time;    //按键检测时间间隔
  uint32_t led_time;    //LED 流水灯闪烁时间间隔
  uint32_t Charge_time;//充电指示动画间隔
}Delay_StructType;

static Delay_StructType Delay;

#define T_10MS 1
#define T_500MS T_10MS*50
#define T_1S T_10MS*100
#define T_1M T_1S*60



#ifdef TIM2_ENABLE
/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles TIM2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_IRQHandler(void)
{
	TIM2->SR &= (u16)~TIM_SR_UIF;	  	//清中断标记

  
}
/*******************************************************************************
* Function Name  : TIM2_Configuration
* Description    : Configures the TIM2.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static TIM2_Configuration(void)
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

  TIM2->ARR = 1; 		 			//初值
	TIM2->PSC = 71; 	 			     //分频值

  //TIM2->CR2 = (TIM_CR2_MMS_0|TIM_CR2_MMS_1); 

  TIM2->CCMR1 = (TIM_CCMR1_OC2M_1|TIM_CCMR1_OC2M_2);
  TIM2->CCR2 = 1;
  TIM2->CCER = TIM_CCER_CC2E;
  TIM2->CCMR1 |= TIM_CCMR1_OC2PE;

	TIM2->CR1 |= TIM_CR1_CEN; 
	TIM2->EGR |= TIM_EGR_UG;
	while((TIM2->SR & TIM_SR_UIF) == 0);
	TIM2->SR &= (u16)~TIM_SR_UIF;
	//TIM2->DIER |= TIM_DIER_UIE;
  
  //NVIC_Init(1,1, TIM2_IRQn);   
}

void TIM2_ReConfiguration(uint16_t arr, uint16_t ccr)
{
  TIM2->CR1 &= ~TIM_CR1_CEN;
  TIM2->ARR = arr; 		 			//初值
	TIM2->PSC = 71; 	 			     //分频值

  TIM2->CCMR1 = (TIM_CCMR1_OC2M_1|TIM_CCMR1_OC2M_2);
  TIM2->CCR2 = ccr;
  TIM2->CCER = TIM_CCER_CC2E;
  TIM2->CCMR1 |= TIM_CCMR1_OC2PE;

	TIM2->CR1 |= TIM_CR1_CEN; 
	TIM2->EGR |= TIM_EGR_UG;
	while((TIM2->SR & TIM_SR_UIF) == 0);
	TIM2->SR &= (u16)~TIM_SR_UIF; 
}
#endif /*TIM2_ENABLE*/

#ifdef TIM3_ENABLE 
/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : This function handles TIM3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM3_IRQHandler(void)				
{

	TIM3->SR &= (u16)~TIM_SR_UIF;	  	//清中断标记
  
  TIM3->CR1 &= ~TIM_CR1_CEN;
}

/*******************************************************************************
* Function Name  : TIM3_Configuration
* Description    : Configures the TIM3.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static TIM3_Configuration(void)
{
  uint32_t temp;
  /*
  BEEP -- PA7(TIM3 通道2)
  */
  temp = GPIOA->CRL & 0x0FFFFFFF;			
	GPIOA->CRL = temp | 0xB0000000;			//PA7

  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

  TIM3->ARR = 999; 		 			//Auto reload value
	TIM3->PSC = 71; 	 		      	//Prescaler value
  TIM3->EGR |= TIM_EGR_UG;				//Update generation 人工初始化

  TIM3->CCMR1 = (TIM_CCMR1_OC2M_1|TIM_CCMR1_OC2M_2);
  TIM3->CCR2 = 499;
  TIM3->CCER = TIM_CCER_CC2E;
  TIM3->CCMR1 |= TIM_CCMR1_OC2PE;

  TIM3->CR1 |= TIM_CR1_ARPE;
	TIM3->CR1 |= TIM_CR1_CEN; 				//counter enable 
  OneBeep(); 
  	
}
#endif /* TIM3_ENABLE */

#ifdef TIM4_ENABLE
/*******************************************************************************
* Function Name  : TIM4_IRQHandler
* Description    : This function handles TIM4 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM4_IRQHandler(void)
{		
	TIM4->SR &= ~TIM_SR_UIF;

}
/*******************************************************************************
* Function Name  : TIM4_Configuration
* Description    : 定时器4配置
* Input          : None
* Output         : None
* Return         : None
* Date           : 2014-09-17
* Author         : ADT LL
*******************************************************************************/
void TIM4_Configuration(void) 
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

	TIM4->ARR = 499; 		 			//Auto reload value
	TIM4->PSC = 71; 	 			//Prescaler value

	TIM4->CR1 |= TIM_CR1_CEN; 				//counter enable	
	TIM4->EGR |= TIM_EGR_UG;				//Update generation 人工初始化

	while((TIM4->SR & TIM_SR_UIF) == 0);	//Update interrupt flag

	TIM4->SR &= ~TIM_SR_UIF;
  TIM4->CR2 |= TIM_CR2_MMS_1; 
}
#endif /*TIM4_ENABLE*/

#ifdef TIM5_ENABLE
/*******************************************************************************
* Function Name  : TIM5_IRQHandler
* Description    : This function handles TIM5 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM5_IRQHandler(void)
{
	TIM5->SR &= ~TIM_SR_UIF;	  	//清中断标记     
}

/*******************************************************************************
* Function Name  : TIM5_Configuration
* Description    : 定时器5配置,频率=72M/(PSC+1)/(ARR+1)
* Input          : None
* Output         : None
* Return         : None
* Date           : 2014-09-17
* Author         : LL
*******************************************************************************/
void TIM5_Configuration(void) 	  	
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;

	TIM5->ARR = 9; 		 			//
	TIM5->PSC = 71; 	 			//Prescaler value

	TIM5->CR1 |= TIM_CR1_CEN; 				//counter enable	
	TIM5->EGR |= TIM_EGR_UG;				//Update generation 人工初始化

	while((TIM5->SR & TIM_SR_UIF) == 0);	//Update interrupt flag

	TIM5->SR &= ~TIM_SR_UIF;
  TIM5->CR2 |= TIM_CR2_MMS_1; 
}
#endif /*TIM5_ENABLE*/

#ifdef TIM6_ENABLE
/*******************************************************************************
* Function Name  : TIM6_IRQHandler
* Description    : This function handles TIM6 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM6_IRQHandler(void)
{
  uint32_t temp;
	TIM6->SR &= ~TIM_SR_UIF;	  	//清中断标记 

  if (MenuPoint->Columnnum>8)
    {
      if (ARRAY_FULL == ArrayFifo_Isempty()) return;
      if (samplecnt<TimeScale[MenuPoint->Columnnum].Div)
      {
      temp = 500*((ADC_ConvertedValue[2]>2047) ? (ADC_ConvertedValue[2]-2047): 0)/4095;
      pArrayFifo->pTail->Wave1[samplecnt] = (temp > 200) ? 200 : temp;
      temp = 50+500*((ADC_ConvertedValue[3]>2047) ? (ADC_ConvertedValue[3]-2047): 0)/4095;
      pArrayFifo->pTail->Wave2[samplecnt] = (temp > 200) ? 200 : temp;
      samplecnt++;
      }
      else
      {
        samplecnt=0;
        ArrayFifo_Input();
      }
    }     
}

/*******************************************************************************
* Function Name  : TIM6_Configuration
* Description    : Configures the TIM6.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static TIM6_Configuration(void)
{

  RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
  /*周期 T = (9+1)*(71+1)/72M = 10uS*/
  TIM6->ARR = 9; 		 			   //初值
	TIM6->PSC = 71; 	 			     //分频值

	TIM6->CR1 |= TIM_CR1_CEN; 
	TIM6->EGR |= TIM_EGR_UG;
	while((TIM6->SR & TIM_SR_UIF) == 0);
	TIM6->SR &= (u16)~TIM_SR_UIF;
	TIM6->DIER |= TIM_DIER_UIE;
  
  NVIC_Init(1,0, TIM6_IRQn);   
  
}

#endif /* TIM6_ENABLE */

#ifdef TIM7_ENABLE
/*******************************************************************************
* Function Name  : TIM7_IRQHandler
* Description    : This function handles TIM7 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM7_IRQHandler(void)
{
  TIM7->SR &= (u16)~TIM_SR_UIF;	  	//清中断标记

  Delay.led_time++;
  
  if (Delay.led_time > T_1S)
  {
    Flag.flag = true;
    Delay.led_time = 0;
  }

}
/*******************************************************************************
* Function Name  : TIM7_Configuration
* Description    : Configures the TIM7.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void TIM7_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 9999;
    TIM_TimeBaseStructure.TIM_Prescaler = 83;

    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);//使能中断
    TIM_Cmd(TIM7, ENABLE);
}
#endif

/*******************************************************************************
* Function Name  : TIM_Configuration
* Description    : Configures the TIM.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM_Configuration(void)
{
  #ifdef TIM2_ENABLE
  TIM2_Configuration();
  #endif
  #ifdef TIM3_ENABLE
  TIM3_Configuration();
  #endif
  #ifdef TIM4_ENABLE
  TIM4_Configuration();
  #endif
  #ifdef TIM5_ENABLE
  TIM5_Configuration();
  #endif   
  #ifdef TIM6_ENABLE
  TIM6_Configuration();
  #endif
  #ifdef TIM7_ENABLE
  TIM7_Configuration();
  #endif

}

/* ************ ****** ************ THE FILE END  ************ ****** ************ */
