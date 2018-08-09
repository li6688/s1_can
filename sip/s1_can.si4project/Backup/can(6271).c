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

#include "stm32f4xx.h"
#include "main.h"
#include "uart_printf.h"
#include "uart.h"

CanTxMsg TxMessage;
CanRxMsg RxMessage;


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
    TxMessage.Data[0] = 2;
    
    /* Wait until one of the mailboxes is empty */
    while((CAN_GetFlagStatus(CAN1, CAN_FLAG_RQCP0) !=RESET) || \
          (CAN_GetFlagStatus(CAN1, CAN_FLAG_RQCP1) !=RESET) || \
          (CAN_GetFlagStatus(CAN1, CAN_FLAG_RQCP2) !=RESET));
    CAN_Transmit(CAN1, &TxMessage);
}


/**
  * @brief  This function handles CAN1 RX0 request.
  * @param  None
  * @retval None
  */
void CAN1_RX0_IRQHandler(void)
{
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

    if ((RxMessage.StdId == 0x321)&&(RxMessage.IDE == CAN_ID_STD) && (RxMessage.DLC == 1))
    {
        UART_PutChar(RxMessage.Data[0]+'0');
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
    CAN_InitStructure.CAN_ABOM = DISABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;//CAN_Mode_Normal;
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

    Init_RxMes(&RxMessage);
    
    
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
	
}
/* ************ ****** ************ THE FILE END  ************ ****** ************ */				
