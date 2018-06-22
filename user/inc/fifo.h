/**
  ******************************************************************************
  * @file    fifo.h
  * @author  LL
  * @version V0.0.0
  * @date    2016-03-21
  * @brief   
  ******************************************************************************
  ******************************************************************************
***/
#ifndef __FIFO_H__
#define __FIFO_H__

 
#define _DI()
#define _EI()

#define TX_SIZE 128
#define RX_SIZE 56

#define  DATATYPE unsigned char
#define  DATATYPESIZE unsigned int

typedef struct
{
    DATATYPE *pBuf;
    DATATYPE *Tail;
    DATATYPE *Head;
    DATATYPESIZE len;
}FiFo_StructType;

extern DATATYPE Tx_Buffer[];
extern FiFo_StructType *pTx;

extern DATATYPE Rx_Buffer[];
extern FiFo_StructType *pRx;

extern DATATYPESIZE FIFO_GetCount(FiFo_StructType *FiFo);
extern unsigned char FIFO_PutOne(FiFo_StructType *FiFo, DATATYPE Data);
extern unsigned char FIFO_GetOne(FiFo_StructType *FiFo, DATATYPE *pData);
extern void FIFO_Init(FiFo_StructType *FiFo, DATATYPESIZE Size, DATATYPE *Buf);

#endif
// -------------------------------- END OF FILE --------------------------------
