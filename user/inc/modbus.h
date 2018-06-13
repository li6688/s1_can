/**
  ******************************************************************************
  * @file    modbus.h
  * @author  LL
  * @version V0.0.0
  * @date    2017-05-11
  * @brief   头文件
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

#define MODBUS_DEVICEID 0x21 //设备ID
#define MODBUS_DEVICEID_MAX 247 //0xF7 设备ID最大值

#define MODBUS_BASE_ADDR 0x7000
#define MODBUS_REGISTERBASE_ADDR 0x00
#define MODBUS_DEVICEID_ADDR (0x00+MODBUS_REGISTERBASE_ADDR+MODBUS_BASE_ADDR) //两个字节,占用一个两字节的寄存器
#define MODBUS_UARTBAUDRATE_ADDR (0x02+MODBUS_REGISTERBASE_ADDR+MODBUS_BASE_ADDR) //四个字节，占用两个两字节的寄存器
//#define MODBUS_SAMPLETIME_ADDR (0x06+MODBUS_REGISTERBASE_ADDR+MODBUS_BASE_ADDR) //2个字节


#define MODBUS_COILBASE_ADDR 0x10
//#define MODBUS_BATTYPE_ADDR (0x00+MODBUS_COILBASE_ADDR+MODBUS_BASE_ADDR)//电池类型，字节为0蓄电池13.8V，字节为0xFF为锂电池12.6V

/*
寄存器使用地址定义,每个寄存器两个字节
0地址设备ID,第一个字节有效
1地址串口波特率占用两个地址1和2，四个字节
3格雷码
*/
#define MODBUS_REGISTER_MAX 0x0008 //寄存器的个数

extern Modbus_StructType *pModbus_DataPort1;

extern void Modbus_DataDecode_RTU(uint8_t *data,  uint16_t len);
extern void Modbus_Init(void);
extern void Modbus_Test(uint8_t data);
#endif /* __MODBUS_H__ */
