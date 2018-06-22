/**
  ******************************************************************************
  * @file    spi.h
  * @author  LL
  * @version V0.0.0
  * @date    2014-10-20
  * @brief   ͷ�ļ�
  ******************************************************************************
  ******************************************************************************
***/

#ifndef __SPI_H__
#define __SPI_H__


#ifdef __cplusplus
 extern "C" {
#endif

#define CS1_PORT  GPIOB
#define CS1_BIT   GPIO_Pin_12
#define CS1_SET   CS1_PORT->BSRR |= CS1_BIT;//�ߵ�ƽ
#define CS1_CLR   CS1_PORT->BRR  |= CS1_BIT;//�͵�ƽ

#define CS2_PORT  GPIOA
#define CS2_BIT   GPIO_Pin_10
#define CS2_SET   CS2_PORT->BSRR |= CS2_BIT;//�ߵ�ƽ
#define CS2_CLR   CS2_PORT->BRR  |= CS2_BIT;//�͵�ƽ


extern void SPI1_Init(void);
extern uint8_t SPI1_ReadWriteByte(uint8_t TxData);
extern void SPI2_Init(void);
extern uint8_t SPI2_ReadWriteByte(uint8_t TxData);

#ifdef __cplusplus
}
#endif


#endif /* __SPI_H__ */

/* ************ ****** ************ THE FILE END ************ ****** ************ */
