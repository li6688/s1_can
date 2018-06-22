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

#define HEXTOASCIITAB_EN 1 //��Щ���������û��ʹ�õ�ȫ�ֱ������棬��ʹ�õ�ʱ���ֹ������ȥ������

typedef enum{OFF=0, ON = !OFF}OnOffSatus;
typedef struct
{
	bool cursta;//��ǰ״̬
	bool presta;//ǰһ״̬
}Status_StructType;
typedef struct
{
	bool flag;
	bool key_scan;//����ɨ��
	Status_StructType Start;
  
}Flag_StructType;


typedef struct
{
    uint32_t  ResetSource;//��λԴ
	uint8_t  DeviceID;//�豸��ַ
	uint16_t GrayCode;//������
	uint8_t  FMVersion[30];
	uint16_t PortConnectTime;//�˿����Ӽ�ʱ
	uint32_t UartBaudRate;//���ڲ�����
	
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
