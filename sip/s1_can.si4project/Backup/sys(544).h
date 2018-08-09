/**
  ******************************************************************************
  * @file    sys.h
  * @author  LL
  * @version V0.0.0
  * @date    2014-04-23
  * @brief   
  ******************************************************************************
  ******************************************************************************
***/

#ifndef __SYS_H__
#define __SYS_H__

#ifdef __cplusplus
  extern "C" {
#endif
    
#define SYSTIME_10MS  1
#define SYSTIME_100MS 10*SYSTIME_10MS
#define SYSTIME_500MS 50*SYSTIME_10MS
#define SYSTIME_1S    100*SYSTIME_10MS
#define SYSTIME_2S    2*SYSTIME_1S
#define SYSTIME_5S    5*SYSTIME_1S
#define SYSTIME_10S   10*SYSTIME_1S
#define SYSTIME_20S   20*SYSTIME_1S
#define SYSTIME_30S   30*SYSTIME_1S
#define SYSTIME_50S   50*SYSTIME_1S
#define SYSTIME_1M    60*SYSTIME_1S
#define SYSTIME_2M    2*SYSTIME_1M

#define HEXTOASCIITAB_EN 1 //有些编译器会对没有使用的全局变量警告，不使用的时候禁止，可以去掉警告

typedef enum{OFF=0, ON = !OFF}OnOffSatus;
typedef struct
{
	bool cursta;//当前状态
	bool presta;//前一状态
}Status_StructType;
typedef struct
{
	bool flag;
	bool key_scan;//按键扫描
	bool key_has;//有按键
	Status_StructType Start;
  
}Flag_StructType;


typedef struct
{
    uint32_t  ResetSource;//复位源
	uint8_t  DeviceID;//设备地址
	uint16_t GrayCode;//格雷码
	uint8_t  FMVersion[30];
	uint16_t PortConnectTime;//端口连接计时
	uint32_t UartBaudRate;//串口波特率
	
}GlobalVariable_StructType;
#if HEXTOASCIITAB_EN
extern unsigned char HexToAsciiTab[];
#endif

extern  Flag_StructType Flag;
extern  GlobalVariable_StructType GlobalVar;

extern void NumberToAscii(uint8_t *buf, uint32_t number, uint8_t len);
extern void NumberToAsciiHEX(uint8_t *buf, uint32_t number, uint8_t len);

#ifdef __cplusplus
  }
#endif

#endif /* __SYS_H__ */

/* ************ ****** ************ THE FILE END  ************ ****** ************ */
