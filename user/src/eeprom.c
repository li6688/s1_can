/**
  ******************************************************************************
  * @file    eeprom.c
  * @author  LL
  * @version V0.0.0
  * @date    2017-05-19
  * @brief   eeprom��д����
  ******************************************************************************
  ******************************************************************************
***/

#define __EEPROM_C__
#include "config.h"


/*******************************************************************************
* Function Name  : Eeprom_ReadOneByte 
* Description    : ��eeprom�ж�һ���ֽڵ�����
 * ����������
 * 1. ��NMVREGS��1������eeprom��
 * 2. �������ַд��NVMADRH:NVMADRL�Ĵ����ԡ�
 * 3. ��NVMCON1�Ĵ�����RDλ��1��������������
 * 4. ��NVMDATH:NVMDATL�Ĵ������ж������ݡ�
* Input          : 
 * addr:��ַ(��Χ0-0xFF)
 *      ���ʹ��ֱ�ӵ�ַ���ַ��Χ0xF000-0xF0FF
 * data:���������� 
* Output         : None
* Return         : None
* Date           : 2017-05-19
* Author         : LL
*******************************************************************************/
static void Eeprom_ReadOneByte(uint16_t addr, uint8_t *data)
{
    //����ʹ��ֱ�ӵ�ַ��������������䣬�Լ��ٴ�����
    //if (addr>0xFF) return;//��ַ������Χ
    //addr += 0x7000;//eeprom����ʼ��ַ��0x7000

	NVMADRL = (addr & 0xFF);//���÷��ʵĵ�ַ
    NVMADRH = ((addr>>8) & 0xFF);
    NVMCON1bits.NVMREGS = 1;//����Ϊ����eeprom
    NVMCON1bits.RD = 1;//����������
    NOP();//���ڸ���ϵͳ������Ҫ
	NOP();
    
    *data = NVMDATL;
}

/*******************************************************************************
* Function Name  : Eeprom_WriteOneByte 
* Description    : ��eeprom��д��һ���ֽڵ�����
 * ���ݵ�д������
 * 1. �� NVMCON1 �Ĵ����� NVMREGS �� WREN λ��1��
 * 2. �������ַ����ַ+7000h��д��NVMADRH:NVMADRL�Ĵ����ԡ�
 * 3. ִ�н������С�
 * 4.ʹ��NVMDATA д����EEPROM �ֽڡ�
 * 5.�����ѯWRλ��ȷ��д������ʱ���
* Input          : 
 * addr:��ַ(��Χ0-0xFF)
 *      ���ʹ��ֱ�ӵ�ַ���ַ��Χ0xF000-0xF0FF.
 * data:д������� 
* Output         : None
* Return         : None
* Date           : 2017-05-19
* Author         : LL
*******************************************************************************/
static void Eeprom_WriteOneByte(uint16_t addr, uint8_t data)
{
	uint8_t GIEBitValue = INTCONbits.GIE;
	
    //����ʹ��ֱ�ӵ�ַ��������������䣬�Լ��ٴ�����
    //if (addr>0xFF) return;//��ַ������Χ
    //addr += 0x7000;//eeprom����ʼ��ַ��0x7000

	NVMADRL = (addr & 0xFF);//���÷��ʵĵ�ַ
    NVMADRH = ((addr>>8) & 0xFF);
	NVMDATL = data;
    //NVMDATH = 0;
    NVMCON1bits.NVMREGS = 1;//����Ϊ����eeprom
    NVMCON1bits.WREN = 1;
    INTCONbits.GIE = 0;//�ر��жϡ��������У����ɱ��ж�
    NVMCON2 = 0x55;//����
    NVMCON2 = 0xAA;
    NVMCON1bits.WR = 1;
	INTCONbits.GIE = GIEBitValue; //�ظ��ж�״̬
    
    while(0!=NVMCON1bits.WR);//�ȴ�д���
    
}

/*******************************************************************************
* Function Name  : Eeprom_Read 
* Description    : ��eeprom�ж�����
* Input          : addr:��ַ
*                  buf:д������� 
*                  len:���ݳ���
* Output         : None
* Return         : None
* Date           : 2017-05-19
* Author         : LL
*******************************************************************************/
void Eeprom_Read(uint16_t addr, uint8_t *buf, uint16_t len)
{
    uint16_t i;
	
    for(i=0; i<len; i++)
    {
        Eeprom_ReadOneByte(addr+i,buf+i);
    }
}

/*******************************************************************************
* Function Name  : Eeprom_Write 
* Description    : ��eeprom��д����
* Input          : addr:��ַ
*                  buf:д������� 
*                  len:���ݳ��� 
* Output         : None
* Return         : None
* Date           : 2017-05-19
* Author         : LL
*******************************************************************************/
void Eeprom_Write(uint16_t addr, uint8_t *buf, uint16_t len)
{
    uint16_t i;
    
    for(i=0; i<len; i++)
    {
        Eeprom_WriteOneByte(addr+i,buf[i]);
    }
}

/*******************************************************************************
* Function Name  : Eeprom_Test 
* Description    : ����
* Input          : None
* Output         : None
* Return         : None
* Date           : 2017-05-19
* Author         : LL
*******************************************************************************/
void Eeprom_Test(void)
{
	uint8_t i;
	uint8_t buf[10];
	for(i=0;i<5;i++)
	{
		buf[i] = i+'0';
	}
	Eeprom_Write(0,buf,i);
	for(i=0;i<5;i++)
	{
		buf[i] = 0;
	}
    Eeprom_Read(0,buf,i);
	DEBUG_PRINTFMASSAGESHEX(buf,i);
}

/* ************ ****** ************ THE FILE END  ************ ****** ************ */