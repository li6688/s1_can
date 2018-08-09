/**
******************************************************************************
* @file    can.c
* @author  LL
* @version V0.0.0
* @date    2018-06-24
* @brief   本软件使用与STM32F4系列
******************************************************************************
******************************************************************************
***/
#define __CAN_C__

#include <stdbool.h>
#include "stm32f4xx.h"
#include "main.h"
#include "uart_printf.h"
#include "uart.h"
#include "sys.h"

CanTxMsg TxMessage;
CanRxMsg RxMessage1;
CanRxMsg RxMessage2;



/*******************************************************************************
* Function Name  : Init_RxMes 
* Description    : CAN接收消息初始化
* Input          : None
* Output         : None
* Return         : None
* Date           : 2018-06-24
* Author         : LL
*******************************************************************************/
void Init_RxMes(CanRxMsg *RxMessage)
{
    uint8_t ubCounter = 0;

    RxMessage->StdId = 0x00;
    RxMessage->ExtId = 0x00;
    RxMessage->IDE = CAN_ID_STD;
    RxMessage->DLC = 0;
    RxMessage->FMI = 0;
    for (ubCounter = 0; ubCounter < 8; ubCounter++)
    {
        RxMessage->Data[ubCounter] = 0x00;
    }
}

/*******************************************************************************
* Function Name  : CAN1_Send 
* Description    : CAN发送消息
* Input          : None
* Output         : None
* Return         : None
* Date           : 2018-06-24
* Author         : LL
*******************************************************************************/
void CAN1_Send(void)
{
    TxMessage.Data[0] = 1;
    
    /* Wait until one of the mailboxes is empty */
//    while((CAN_GetFlagStatus(CAN1, CAN_FLAG_RQCP0) !=RESET) || \
//          (CAN_GetFlagStatus(CAN1, CAN_FLAG_RQCP1) !=RESET) || \
//          (CAN_GetFlagStatus(CAN1, CAN_FLAG_RQCP2) !=RESET));
    CAN_Transmit(CAN1, &TxMessage);
}

/*******************************************************************************
* Function Name  : CAN2_Send 
* Description    : CAN发送消息
* Input          : None
* Output         : None
* Return         : None
* Date           : 2018-06-24
* Author         : LL
*******************************************************************************/
void CAN2_Send(void)
{
    TxMessage.Data[0] = 2;
    
    /* Wait until one of the mailboxes is empty */
//    while((CAN_GetFlagStatus(CAN1, CAN_FLAG_RQCP0) !=RESET) || \
//          (CAN_GetFlagStatus(CAN1, CAN_FLAG_RQCP1) !=RESET) || \
//          (CAN_GetFlagStatus(CAN1, CAN_FLAG_RQCP2) !=RESET));
    CAN_Transmit(CAN2, &TxMessage);
}



/**
  * @brief  This function handles CAN1 RX0 request.
  * @param  None
  * @retval None
  */
void CAN1_RX0_IRQHandler(void)
{
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage1);
    if ((RxMessage1.StdId == 0x321)&&(RxMessage1.IDE == CAN_ID_STD) && (RxMessage1.DLC == 1))
    {
        Flag.can1_send = true;
    }
}


/*******************************************************************************
* Function Name  : CAN1_Init 
* Description    : 初始化，波特率
* Input          : None
* Output         : None
* Return         : None
* Date           : 2018-06-24
* Author         : LL
*******************************************************************************/
static void CAN1_Init(void)
{
    CAN_InitTypeDef        CAN_InitStructure;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    /* CAN register init */
    CAN_DeInit(CAN1);

    /* CAN cell init */
    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = ENABLE;//DISABLE;
    CAN_InitStructure.CAN_AWUM = ENABLE;//DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;

    /* CAN Baudrate = 175 Kbps (CAN clocked at 42 MHz) */
    CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
    CAN_InitStructure.CAN_Prescaler = 16;
    CAN_Init(CAN1, &CAN_InitStructure);

    /* CAN filter init */
    CAN_FilterInitStructure.CAN_FilterNumber = 0;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);

    /* Transmit Structure preparation */
    TxMessage.StdId = 0x321;
    TxMessage.ExtId = 0x01;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.DLC = 1;

    /* Enable FIFO 0 message pending Interrupt */
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

    Init_RxMes(&RxMessage1);
    
    
}

/**
  * @brief  This function handles CAN2 SEC request.
  * @param  None
  * @retval None
  */
void CAN2_SCE_IRQHandler(void)
{
    Flag.can2_sce = true;

    CAN_ClearITPendingBit(CAN2,CAN_IT_EPV); 
    
}


/**
  * @brief  This function handles CAN2 RX0 request.
  * @param  None
  * @retval None
  */
void CAN2_RX0_IRQHandler(void)
{
    CAN_Receive(CAN2, CAN_FIFO0, &RxMessage2);
    if ((RxMessage2.StdId == 0x321)&&(RxMessage2.IDE == CAN_ID_STD) && (RxMessage2.DLC == 1))
    {
        Flag.can_send = true;
    }
}


/*******************************************************************************
* Function Name  : CAN2_Init 
* Description    : 初始化
* Input          : None
* Output         : None
* Return         : None
* Date           : 2018-06-24
* Author         : LL
*******************************************************************************/
static void CAN2_Init(void)
{
    CAN_InitTypeDef        CAN_InitStructure;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    /* CAN register init */
    CAN_DeInit(CAN2);

    /* CAN cell init */
    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = ENABLE;//DISABLE;
    CAN_InitStructure.CAN_AWUM = ENABLE;//DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;

    /* CAN Baudrate = 175 Kbps (CAN clocked at 42 MHz) */
    CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
    CAN_InitStructure.CAN_Prescaler = 16;
    CAN_Init(CAN2, &CAN_InitStructure);

    /* CAN filter init */
    CAN_FilterInitStructure.CAN_FilterNumber = 14;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;//0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);

    /* Transmit Structure preparation */
    TxMessage.StdId = 0x321;
    TxMessage.ExtId = 0x01;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.DLC = 1;

    /* Enable FIFO 0 message pending Interrupt */
    CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
    CAN_ITConfig(CAN2, CAN_IT_ERR, ENABLE);
    CAN_ITConfig(CAN2, CAN_IT_EPV, ENABLE);

    Init_RxMes(&RxMessage2);
    
    
}


/*******************************************************************************
* Function Name  : CAN_Configuration 
* Description    : CAN初始化
* Input          : None
* Output         : None
* Return         : None
* Date           : 2018-06-24
* Author         : LL
*******************************************************************************/
void CAN_Configuration(void)
{
	CAN1_Init();
    CAN2_Init();
    DEBUG_PRINTFMASSAGES("CAN OK");
}

/*******************************************************************************
* Function Name  : CAN_Process
* Description    : CAN处理
* Input          : None
* Output         : None
* Return         : None
* Date           : 2018-06-24
* Author         : LL
*******************************************************************************/
void CAN_Process(void)
{
    if (Flag.can_send)
    {
        Flag.can_send = false;
        DEBUG_PRINTFMASSAGES("CAN Send");
    }
    if (Flag.can1_send)
    {
        Flag.can1_send = false;
        DEBUG_PRINTFMASSAGES("CAN1 Send");
    }
    if (Flag.can2_sce)
    {
        Flag.can2_sce = false;
        DEBUG_PRINTFMASSAGES("CAN  2 SEC");
    }
	
}
/* ************ ****** ************ THE FILE END  ************ ****** ************ */				
