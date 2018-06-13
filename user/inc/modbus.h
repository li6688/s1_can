/**
  ******************************************************************************
  * @file    modbus.h
  * @author  LL
  * @version V0.0.0
  * @date    2017-05-11
  * @brief   ͷ�ļ�
  ******************************************************************************
  ******************************************************************************
***/

#ifndef __MODBUS_H__
#define __MODBUS_H__

#define DATACODEBUFF_MAXSIZE 32

typedef struct
{
    uint8_t step;
    uint16_t len;
    uint16_t cnt;
    //uint8_t crc16_h;
	//uint8_t crc16_l;
	uint8_t DataCodeBuff[DATACODEBUFF_MAXSIZE];
} Modbus_StructType;

#define MODBUS_DEVICEID 0x21 //�豸ID
#define MODBUS_DEVICEID_MAX 247 //0xF7 �豸ID���ֵ

#define MODBUS_BASE_ADDR 0x7000
#define MODBUS_REGISTERBASE_ADDR 0x00
#define MODBUS_DEVICEID_ADDR (0x00+MODBUS_REGISTERBASE_ADDR+MODBUS_BASE_ADDR) //�����ֽ�,ռ��һ�����ֽڵļĴ���
#define MODBUS_UARTBAUDRATE_ADDR (0x02+MODBUS_REGISTERBASE_ADDR+MODBUS_BASE_ADDR) //�ĸ��ֽڣ�ռ���������ֽڵļĴ���
//#define MODBUS_SAMPLETIME_ADDR (0x06+MODBUS_REGISTERBASE_ADDR+MODBUS_BASE_ADDR) //2���ֽ�


#define MODBUS_COILBASE_ADDR 0x10
//#define MODBUS_BATTYPE_ADDR (0x00+MODBUS_COILBASE_ADDR+MODBUS_BASE_ADDR)//������ͣ��ֽ�Ϊ0����13.8V���ֽ�Ϊ0xFFΪ﮵��12.6V

/*
�Ĵ���ʹ�õ�ַ����,ÿ���Ĵ��������ֽ�
0��ַ�豸ID,��һ���ֽ���Ч
1��ַ���ڲ�����ռ��������ַ1��2���ĸ��ֽ�
3������
*/
#define MODBUS_REGISTER_MAX 0x0008 //�Ĵ����ĸ���

extern Modbus_StructType *pModbus_DataPort1;

extern void Modbus_DataDecode_RTU(uint8_t *data,  uint16_t len);
extern void Modbus_Init(void);
extern void Modbus_Test(uint8_t data);
#endif /* __MODBUS_H__ */
