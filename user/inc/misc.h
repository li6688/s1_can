/**
  ******************************************************************************
  * @file    misc.h
  * @author  LL
  * @version V0.0.0
  * @date    2016-06-20
  * @brief   头文件 
  ******************************************************************************
  ******************************************************************************
***/

#ifndef __MISC_H__
#define __MISC_H__

//开关量控制编号
#define DEVICES1_NUM 1
#define DEVICES2_NUM 2
#define DEVICES3_NUM 3
#define DEVICES4_NUM 4

#define POWERSUPPLY_NUM  DEVICES1_NUM //模拟部分供电控制

//模拟通道编号
#define CH0_NUM 0
#define CH1_NUM 1
#define CH2_NUM 2
#define CH3_NUM 3
#define CH4_NUM 4
#define CH5_NUM 5
#define CH6_NUM 6
#define CH7_NUM 7

#define AN1_NUM  CH0_NUM
#define AN2_NUM  CH1_NUM 
#define AN3_NUM  CH2_NUM 
#define AN4_NUM  CH3_NUM 


//输入检测信号编号
#define GPIO1_NUM 1

#define LOGICSTA_NUM GPIO1_NUM//逻辑状态

//电平高低
#define LEVEL_HIGH 1
#define LEVEL_LOW 0


#define LOWPOWER_EN 1 //低功耗使能，1开启，0关闭

extern void Misc_DeviceOnOff(uint8_t devicenum, OnOffSatus sta);
extern uint8_t Misc_ReadCheckPin(uint8_t pinnum);
extern uint16_t Misc_GrayToBin(uint16_t gray);
extern uint16_t Misc_ReadGray(void);
extern void Misc_SetParameter();
extern void Misc_Init(void);
extern void Misc_Process(void);


#endif /* UART0_H_ */
