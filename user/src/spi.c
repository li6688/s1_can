/**
  ******************************************************************************
  * @file    spi.c
  * @author  LL
  * @version V0.0.0
  * @date    2014-04-23
  * @brief   SPI²Ù×÷º¯Êý
  ******************************************************************************
  ******************************************************************************
***/

#include <stdint.h>
#include "stm32f10x.h"
#include "main.h"
#include "spi.h"

void SPI1_Init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;

    /*!< SPI configuration */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    /*!< Enable the SPI  */
    SPI_Cmd(SPI1, ENABLE);
    SPI1_ReadWriteByte(0xFF);
}

uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{		
	uint8_t retry;
    
    retry = 0;
    /*!< Loop while DR register in not emplty */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
    {
        retry++;
		if(retry > 200) return 0;
    }

    /*!< Send byte through the SPI peripheral */
    SPI_I2S_SendData(SPI1, TxData);

    retry = 0;
    /*!< Wait to receive a byte */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
    {
        retry++;
		if(retry > 200) return 0;
    }

    /*!< Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI1);	    
}

void SPI2_Init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;

    /*!< SPI configuration */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);

    /*!< Enable the SPI  */
    SPI_Cmd(SPI2, ENABLE);
    SPI2_ReadWriteByte(0xFF);
}

uint8_t SPI2_ReadWriteByte(uint8_t TxData)
{		
	uint8_t retry;	

    retry = 0;
    /*!< Loop while DR register in not emplty */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
    {
        retry++;
		if(retry > 200) return 0;
    }

    /*!< Send byte through the SPI peripheral */
    SPI_I2S_SendData(SPI2, TxData);

    retry = 0;
    /*!< Wait to receive a byte */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
    {
        retry++;
		if(retry > 200) return 0;
    }

    /*!< Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI2);	    
}


/* ************ ****** ************ THE FILE END ************ ****** ************ */
