/**
  ******************************************************************************
  * @file    main.c
  * @author  LL
  * @version V0.0.0
  * @date    2018-03-28
  * @brief   
  ******************************************************************************
  ******************************************************************************
***/

#include <stdbool.h>
#include "stm32f4xx.h"
#include "systemcommon.h"
#include "main.h"
#include "uart.h"
#include "sys.h"

#define LED1_ON  GPIOD->BSRRL = GPIO_Pin_12
#define LED1_OFF GPIOD->BSRRH = GPIO_Pin_12

#define LED2_ON  GPIOD->BSRRL = GPIO_Pin_13
#define LED2_OFF GPIOD->BSRRH = GPIO_Pin_13



/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
    char c=0;
    
    Configuration_Init();
    LED1_ON;LED2_ON;
    //IWDG_Configuration();
    while(1)
    {
        //Key_Read();
        //ADC_Process();
        //USART_Process();
        //Other_Process();
        if (true==Flag.flag)
        {
            Flag.flag = false;
            if (c==0)
            {
                c =1;
                LED1_OFF;LED2_ON;
            }
            else
            {
                c = 0;
                LED1_ON;LED2_OFF;
            }
        }
        //Delay_10ms(100);
        
        /* Î¹¹· */
        //IWDG_ReloadCounter();
    }

}

#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  printf("Wrong parameters value: file %s on line %d\r\n", file, line);

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/* ************ ****** ************ THE FILE END  ************ ****** ************ */
