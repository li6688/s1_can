/**
  ******************************************************************************
  * @file    key.c
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
#include "can.h"

#define KEY_NULL     0x00    //空
#define	KEY_S1       0x08    //KEY1
#define	KEY_S2       0x04    //KEY2
#define	KEY_S3       0x02    //KEY3
#define	KEY_S4       0x01    //KEY4

#define KEY_ENTER   KEY_S4
#define	KEY_ENTERHOLD   0x80|KEY_ENTER

enum
{
	CKSTEP_NULL,
	CKSTEP_PRESS,
	CKSTEP_HOLD,
	CKSTEP_HOLDRPT,
	CKSTEP_RLS
};

#define KEY_T_1S 100
#define KEY_T_3S 3*KEY_T_1S

/*
如果按键按下一次有多次动作，是因为按键抖动引起，可适当增加按键的扫描时间
和增加去抖动处理。
*/

/*******************************************************************/
//全局变量声明区 

/********************************************************************/
//私有变量声明区

struct key_data
{
    uint8_t closed; 
    uint8_t changed;
    uint8_t data;
    uint8_t step;
    uint32_t count;	 				  			
}keys;

/*******************************************************************************
* Function Name  : Key_Enter_Serve
* Description    : 确定按键
* Input          : None
* Output         : None
* Return         : None
* Date           : 2014-10-18
* Author         : LL
*******************************************************************************/
void Key_Enter_Serve(void)
{
 
}
//********************************************************************************
//按键处理
//********************************************************************************
void Key_All_Serve(void)
{
    switch(keys.closed)
    {
        case KEY_ENTER:
            Flag.key_has = true;
            CAN1_Send();
            break;
        case KEY_ENTERHOLD:
            break;
        default:
            break;	
    }
  
    keys.closed=0;									
}

//********************************************************************************
//key scan		
//********************************************************************************
void Key_Read(void)	
{ 
    uint8_t temp;
    bool ref=false;
    if(false == Flag.key_scan) return;

    temp = ((GPIOA->IDR >> 0) & 0x0001);	 				

    switch(keys.step)
    {
        case CKSTEP_NULL:
            if (KEY_NULL != temp)
            {
                keys.data = temp;
                keys.step = CKSTEP_PRESS;
            }
            break;	
        case CKSTEP_PRESS:
            if (temp != keys.data)
            {
                keys.step = CKSTEP_NULL;
                break;
            }    
            keys.step = CKSTEP_HOLD;
            keys.count = 0;  
            break;
        case CKSTEP_HOLD: 
            if (temp != keys.data)
            {
                keys.step = CKSTEP_RLS;
            }
            keys.count++;
            if (keys.count>KEY_T_3S)
            {
                keys.closed = 0x80|keys.data;
                ref = true;
                keys.step = CKSTEP_RLS; 
            }  
            break;
        case CKSTEP_HOLDRPT:
            break;
        case CKSTEP_RLS:
            if (temp == KEY_NULL)
            {
                if (keys.count<KEY_T_1S)
                { 
                    keys.closed = keys.data;
                    ref = true;
                }
                keys.step = CKSTEP_NULL;     
            }
            break;
        default:
            keys.step = CKSTEP_NULL;
            break;   
    }

    if (ref)
        Key_All_Serve();

    Flag.key_scan=false;
}

/* ************ ****** ************ THE FILE END  ************ ****** ************ */
