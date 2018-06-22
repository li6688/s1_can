/**
  ******************************************************************************
  * @file    i2c.c
  * @author  LL
  * @version V0.0.0
  * @date    2014-04-23
  * @brief   ²Ù×÷º¯Êý
  ******************************************************************************
  ******************************************************************************
***/

#include <stdint.h>
#include "stm32f10x.h"
#include "main.h"
#include "i2c.h"
#include "ds1682.h"

#define WAITE_TIMEOUT 2000

#define I2C_SPEED          300000
#define I2C_SLAVE_ADDRESS7 0xD6


void I2C1_Init(void)
{
    I2C_InitTypeDef  I2C_InitStructure;

    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = I2C_SLAVE_ADDRESS7;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
  
    I2C_Cmd(I2C1, ENABLE);
    I2C_Init(I2C1, &I2C_InitStructure);
}

uint8_t I2C1_Write(uint8_t *pBuffer, uint16_t WriteAddr, uint8_t NumByteToWrite)
{		
    uint8_t i;
	uint16_t retry;	

    retry = 0;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
    {
        retry++;
    	if(retry > WAITE_TIMEOUT) return I2C_TIMEOUT;
    }

    /*!< Send START condition */
    I2C_GenerateSTART(I2C1, ENABLE);
    /*!< Test on EV5 and clear it */
    retry = 0;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    {
        retry++;
    	if(retry > WAITE_TIMEOUT) return I2C_TIMEOUT;
    }

    /*!< Send address for write */
    I2C_Send7bitAddress(I2C1, DS1682_ADDR, I2C_Direction_Transmitter);
    /*!< Test on EV6 and clear it */
    retry = 0;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        retry++;
    	if(retry > WAITE_TIMEOUT) return I2C_TIMEOUT;
    }

    I2C_SendData(I2C1, WriteAddr);
    /*!< Test on EV8 and clear it */
    retry = 0;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        retry++;
    	if(retry > WAITE_TIMEOUT) return I2C_TIMEOUT;
    }
    
    for (i=0; i<NumByteToWrite; i++)
    {
        I2C_SendData(I2C1, pBuffer[i]);
        /*!< Test on EV8 and clear it */
        retry = 0;
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        {
            retry++;
        	if(retry > WAITE_TIMEOUT) return I2C_TIMEOUT;
        } 
    }
    
    /*!< Send STOP condition */
    I2C_GenerateSTOP(I2C1, ENABLE);

    return I2C_OK;	    
}

uint8_t I2C1_Read(uint8_t *pBuffer, uint16_t ReadAddr, uint8_t NumByteToRead)
{		
    uint8_t i;
	uint16_t retry;

    retry = 0;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
    {
        retry++;
    	if(retry > WAITE_TIMEOUT) return I2C_TIMEOUT;
    }

    /*!< Send START condition */
    I2C_GenerateSTART(I2C1, ENABLE);
    /*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
    retry = 0;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    {
        retry++;
    	if(retry > WAITE_TIMEOUT) return I2C_TIMEOUT;
    }

    /*!< Send address for write */
    I2C_Send7bitAddress(I2C1, DS1682_ADDR, I2C_Direction_Transmitter);
    /*!< Test on EV6 and clear it */
    retry = 0;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        retry++;
    	if(retry > WAITE_TIMEOUT) return I2C_TIMEOUT;
    } 
      
    /*!< Send the internal address to read from: Only one byte address */
    I2C_SendData(I2C1, ReadAddr);
    /*!< Test on EV8 and clear it */
    retry = 0;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF) == RESET)
    {
        retry++;
    	if(retry > WAITE_TIMEOUT) return I2C_TIMEOUT;
    } 
      
    /*!< Send STRAT condition a second time */  
    I2C_GenerateSTART(I2C1, ENABLE);
    /*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
    retry = 0;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    {
        retry++;
    	if(retry > WAITE_TIMEOUT) return I2C_TIMEOUT;
    } 

    /*!< Send address for read */
    I2C_Send7bitAddress(I2C1, DS1682_ADDR, I2C_Direction_Receiver);  
      
    /* One Byte Master Reception procedure (POLLING) ---------------------------*/
    if (NumByteToRead == 1)
    {
        /* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
        retry = 0;
        while(I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR) == RESET)
        {
            retry++;
            if(retry > WAITE_TIMEOUT) return I2C_TIMEOUT;
        }     

        /*!< Disable Acknowledgement */
        I2C_AcknowledgeConfig(I2C1, DISABLE);   

        /* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
        (void)I2C1->SR2;

        /*!< Send STOP Condition */
        I2C_GenerateSTOP(I2C1, ENABLE);

        /* Wait for the byte to be received */
        retry = 0;
        while(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET)
        {
            retry++;
            if(retry > WAITE_TIMEOUT) return I2C_TIMEOUT;
        }  

        /*!< Read the byte received */
        *pBuffer = I2C_ReceiveData(I2C1);

        /* Wait to make sure that STOP control bit has been cleared */
        retry = 0;
        while(I2C1->CR1 & I2C_CR1_STOP)
        {
            retry++;
            if(retry > WAITE_TIMEOUT) return I2C_TIMEOUT;
        }  

        /*!< Re-Enable Acknowledgement to be ready for another reception */
        I2C_AcknowledgeConfig(I2C1, ENABLE);
    }
    else if (NumByteToRead == 2)
    {
        I2C_PECPositionConfig(I2C1, I2C_PECPosition_Next);
        I2C_AcknowledgeConfig(I2C1, ENABLE);
        /* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
        retry = 0;
        while(I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR) == RESET)
        {
            retry++;
            if(retry > WAITE_TIMEOUT) return I2C_TIMEOUT;
        } 
        /* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
        (void)I2C1->SR2;

        /*!< Disable Acknowledgement */
        I2C_AcknowledgeConfig(I2C1, DISABLE);   
        
        /* Wait for the byte to be received */
        retry = 0;
        while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF) == RESET)
        {
            retry++;
            if(retry > WAITE_TIMEOUT) return I2C_TIMEOUT;
        }
              
        /*!< Send STOP Condition */
        I2C_GenerateSTOP(I2C1, ENABLE);
        
        *pBuffer++ =  I2C_ReceiveData(I2C1);
        *pBuffer++ =  I2C_ReceiveData(I2C1);
        
        /* Wait to make sure that STOP control bit has been cleared */
        retry = 0;
        while(I2C1->CR1 & I2C_CR1_STOP)
        {
            retry++;
            if(retry > WAITE_TIMEOUT) return I2C_TIMEOUT;
        }  

        /*!< Re-Enable Acknowledgement to be ready for another reception */
        I2C_AcknowledgeConfig(I2C1, ENABLE); 

    }
    else
    {
        /*!< Test on EV6 and clear it */
        retry = 0;
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
        {
            retry++;
            if(retry > WAITE_TIMEOUT) return I2C_TIMEOUT;
        }

        for (i=0; i<NumByteToRead-3; i++)
        {
            /* Wait for the byte to be received */
            retry = 0;
            while(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET)
            {
                retry++;
                if(retry > WAITE_TIMEOUT) return I2C_TIMEOUT;
            }  
            *pBuffer++ =  I2C_ReceiveData(I2C1);
        }
        
        /* Wait for the byte to be received */
        retry = 0;
        while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF) == RESET)
        {
            retry++;
            if(retry > WAITE_TIMEOUT) return I2C_TIMEOUT;
        }
        /*!< Disable Acknowledgement */
        I2C_AcknowledgeConfig(I2C1, DISABLE);
        *pBuffer++ =  I2C_ReceiveData(I2C1);
        
        /*!< Send STOP Condition */
        I2C_GenerateSTOP(I2C1, ENABLE);
        retry = 0;
        while(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET)
        {
            retry++;
            if(retry > WAITE_TIMEOUT) return I2C_TIMEOUT;
        }  
        *pBuffer++ =  I2C_ReceiveData(I2C1);
        
        
        /* Wait to make sure that STOP control bit has been cleared */
        retry = 0;
        while(I2C1->CR1 & I2C_CR1_STOP)
        {
            retry++;
            if(retry > WAITE_TIMEOUT) return I2C_TIMEOUT;
        }  

        /*!< Re-Enable Acknowledgement to be ready for another reception */
        I2C_AcknowledgeConfig(I2C1, ENABLE); 
    }
      
    return I2C_OK;
}

/* ************ ****** ************ THE FILE END ************ ****** ************ */
