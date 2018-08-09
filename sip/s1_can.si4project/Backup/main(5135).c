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
#include "sys.h"
#include "uart.h"
#include "key.h"

#define LED1_ON    GPIOD->BSRRL = GPIO_Pin_12
#define LED1_OFF   GPIOD->BSRRH = GPIO_Pin_12
#define LED1_FLASH GPIOD->ODR  ^= GPIO_Pin_12


#define LED2_ON    GPIOD->BSRRL = GPIO_Pin_13
#define LED2_OFF   GPIOD->BSRRH = GPIO_Pin_13
#define LED2_FLASH GPIOD->ODR  ^= GPIO_Pin_13

#define LED3_ON    GPIOD->BSRRL = GPIO_Pin_14
#define LED3_OFF   GPIOD->BSRRH = GPIO_Pin_14
#define LED3_FLASH GPIOD->ODR  ^= GPIO_Pin_14

#define LED4_ON    GPIOD->BSRRL = GPIO_Pin_15
#define LED4_OFF   GPIOD->BSRRH = GPIO_Pin_15
#define LED4_FLASH GPIOD->ODR  ^= GPIO_Pin_15

/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
    Configuration_Init();
    LED1_ON;LED2_ON;LED3_ON;LED4_ON;
    IWDG_Configuration();
    while(1)
    {
        Key_Read();
        //ADC_Process();
        UART_Process();
        //Other_Process();
        if (true==Flag.flag)
        {
            Flag.flag = false;
            LED1_FLASH;LED2_FLASH;
        }
        if (true==Flag.key_has)
        {
            Flag.key_has = false;
            LED3_FLASH;
        }
        /* Î¹¹· */
        IWDG_ReloadCounter();
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
