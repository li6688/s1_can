/**
  ******************************************************************************
  * @file    misc.c
  * @author  LL
  * @version V0.0.0
  * @date    2016-06-20
  * @brief
  ******************************************************************************
  ******************************************************************************
***/
#define __MISC_C__

#include "config.h"

//RC3
#define DEVICES1_TRI TRISCbits.TRISC3
#define DEVICES1_PORT LATCbits.LATC3
#define DEVICES1_ANS  ANSELCbits.ANSC3
#define DEVICES1_OUT  {DEVICES1_ANS = 0; DEVICES1_TRI = 0; NOP();}
#define DEVICES1_SET  {DEVICES1_PORT = 1;}
#define DEVICES1_CLR  {DEVICES1_PORT = 0;}
//RC3
#define DEVICES2_TRI TRISCbits.TRISC3
#define DEVICES2_PORT LATCbits.LATC3
#define DEVICES2_ANS  ANSELCbits.ANSC3
#define DEVICES2_OUT  {DEVICES2_ANS = 0; DEVICES2_TRI = 0; NOP();}
#define DEVICES2_SET  {DEVICES2_PORT = 1;}
#define DEVICES2_CLR  {DEVICES2_PORT = 0;}
//RC3
#define DEVICES3_TRI TRISCbits.TRISC3
#define DEVICES3_PORT LATCbits.LATC3
#define DEVICES3_ANS  ANSELCbits.ANSC3
#define DEVICES3_OUT  {DEVICES3_ANS = 0; DEVICES3_TRI = 0; NOP();}
#define DEVICES3_SET  {DEVICES3_PORT = 1;}
#define DEVICES3_CLR  {DEVICES3_PORT = 0;}

//RA5 
#define GPIO1_TRI TRISAbits.TRISA5
#define GPIO1_PORT PORTAbits.RA5
#define GPIO1_ANS ANSELAbits.ANSA5
#define GPIO1_WPU WPUAbits.WPUA5
#define GPIO1_IN {GPIO1_ANS = 0; GPIO1_TRI = 1; GPIO1_WPU = 1; NOP();}
#define GPIO1_PIN (GPIO1_PORT)
//RA4 
#define GPIO2_TRI TRISAbits.TRISA4
#define GPIO2_PORT PORTAbits.RA4
#define GPIO2_ANS ANSELAbits.ANSA4
#define GPIO2_WPU WPUAbits.WPUA4
#define GPIO2_IN {GPIO2_ANS = 0; GPIO2_TRI = 1; GPIO2_WPU = 1; NOP();}
#define GPIO2_PIN (GPIO2_PORT)
//RC5 
#define GPIO3_TRI TRISCbits.TRISC5
#define GPIO3_PORT PORTCbits.RC5
#define GPIO3_ANS ANSELCbits.ANSC5
#define GPIO3_WPU WPUCbits.WPUC5
#define GPIO3_IN {GPIO3_ANS = 0; GPIO3_TRI = 1; GPIO3_WPU = 1; NOP();}
#define GPIO3_PIN (GPIO3_PORT)
//RC4
#define GPIO4_TRI TRISCbits.TRISC4
#define GPIO4_PORT PORTCbits.RC4
#define GPIO4_ANS ANSELCbits.ANSC4
#define GPIO4_WPU WPUAbits.WPUA5
#define GPIO4_IN {GPIO4_ANS = 0; GPIO4_TRI = 1; GPIO4_WPU = 1; NOP();}
#define GPIO4_PIN (GPIO4_PORT)
//RC3 
#define GPIO5_TRI TRISCbits.TRISC3
#define GPIO5_PORT PORTCbits.RC3
#define GPIO5_ANS ANSELCbits.ANSC3
#define GPIO5_WPU WPUCbits.WPUC3
#define GPIO5_IN {GPIO5_ANS = 0; GPIO5_TRI = 1; GPIO5_WPU = 1; NOP();}
#define GPIO5_PIN (GPIO5_PORT)
//RC6 
#define GPIO6_TRI TRISCbits.TRISC6
#define GPIO6_PORT PORTCbits.RC6
#define GPIO6_ANS ANSELCbits.ANSC6
#define GPIO6_WPU WPUCbits.WPUC6
#define GPIO6_IN {GPIO6_ANS = 0; GPIO6_TRI = 1; GPIO6_WPU = 1; NOP();}
#define GPIO6_PIN (GPIO6_PORT)
//RC7 
#define GPIO7_TRI TRISCbits.TRISC7
#define GPIO7_PORT PORTCbits.RC7
#define GPIO7_ANS ANSELCbits.ANSC7
#define GPIO7_WPU WPUCbits.WPUC7
#define GPIO7_IN {GPIO7_ANS = 0; GPIO7_TRI = 1; GPIO7_WPU = 1; NOP();}
#define GPIO7_PIN (GPIO7_PORT)
//RB7 
#define GPIO8_TRI TRISBbits.TRISB7
#define GPIO8_PORT PORTBbits.RB7
#define GPIO8_ANS ANSELBbits.ANSB7
#define GPIO8_WPU WPUBbits.WPUB7
#define GPIO8_IN {GPIO8_ANS = 0; GPIO8_TRI = 1; GPIO8_WPU = 1; NOP();}
#define GPIO8_PIN (GPIO8_PORT)
//RC2 
#define GPIO9_TRI TRISCbits.TRISC2
#define GPIO9_PORT PORTCbits.RC2
#define GPIO9_ANS ANSELCbits.ANSC2
#define GPIO9_WPU WPUCbits.WPUC2
#define GPIO9_IN {GPIO9_ANS = 0; GPIO9_TRI = 1; GPIO9_WPU = 1; NOP();}
#define GPIO9_PIN (GPIO9_PORT)
//RC1 
#define GPIO10_TRI TRISCbits.TRISC1
#define GPIO10_PORT PORTCbits.RC1
#define GPIO10_ANS ANSELCbits.ANSC1
#define GPIO10_WPU WPUCbits.WPUC1
#define GPIO10_IN {GPIO10_ANS = 0; GPIO10_TRI = 1; GPIO10_WPU = 1; NOP();}
#define GPIO10_PIN (GPIO10_PORT)
//RC0 
#define GPIO11_TRI TRISCbits.TRISC0
#define GPIO11_PORT PORTCbits.RC0
#define GPIO11_ANS ANSELCbits.ANSC0
#define GPIO11_WPU WPUCbits.WPUC0
#define GPIO11_IN {GPIO11_ANS = 0; GPIO11_TRI = 1; GPIO11_WPU = 1; NOP();}
#define GPIO11_PIN (GPIO11_PORT)
//RA2 
#define GPIO12_TRI TRISAbits.TRISA2
#define GPIO12_PORT PORTAbits.RA2
#define GPIO12_ANS ANSELAbits.ANSA2
#define GPIO12_WPU WPUAbits.WPUA2
#define GPIO12_IN {GPIO12_ANS = 0; GPIO12_TRI = 1; GPIO12_WPU = 1; NOP();}
#define GPIO12_PIN (GPIO12_PORT)
//RA1 
#define GPIO13_TRI TRISAbits.TRISA1
#define GPIO13_PORT PORTAbits.RA1
#define GPIO13_ANS ANSELAbits.ANSA1
#define GPIO13_WPU WPUAbits.WPUA1
#define GPIO13_IN {GPIO13_ANS = 0; GPIO13_TRI = 1; GPIO13_WPU = 1; NOP();}
#define GPIO13_PIN (GPIO13_PORT)

#define DI1_PIN GPIO1_PIN
#define DI2_PIN GPIO2_PIN
#define DI3_PIN GPIO3_PIN
#define DI4_PIN GPIO4_PIN
#define DI5_PIN GPIO5_PIN
#define DI6_PIN GPIO6_PIN
#define DI7_PIN GPIO7_PIN
#define DI8_PIN GPIO8_PIN
#define DI9_PIN GPIO9_PIN
#define DI10_PIN GPIO10_PIN
#define DI11_PIN GPIO11_PIN
#define DI12_PIN GPIO12_PIN
#define DI13_PIN GPIO13_PIN

#define MISC_FWVERSION_EN 0
#define MISC_GRAY_BITS 0 //格雷码位数 1--13位，0--12位

/*******************************************************************************
* Function Name  : Misc_GraySample
* Description    : 采样格雷码
* Input          : None
* Output         : None
* Return         : None
* Date           : 2017-08-10
* Author         : LL
*******************************************************************************/
static void Misc_GraySample(void)
{
	uint16_t temp16;
	if (Flag.Gray_Scan)
	{
		Flag.Gray_Scan = FALSE;
		temp16 = Misc_ReadGray();
		GlobalVar.GrayCode = Misc_GrayToBin(temp16);
	}
}

/*******************************************************************************
* Function Name  : Misc_RunStatus
* Description    : 运行状态处理
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-06-21
* Author         : LL
*******************************************************************************/
static void Misc_RunStatus(void)
{
#if LOWPOWER_EN
    if ((TRUE == Flag.Start.cursta) && (Flag.Start.cursta != Flag.Start.presta))
    {
        Flag.Start.presta = Flag.Start.cursta;
	
    }

	if ((FALSE == Flag.Start.cursta) && (Flag.Start.cursta != Flag.Start.presta))
    {
        Flag.Start.presta = Flag.Start.cursta;
		
		DEBUG_PRINTFMASSAGES("in sleep");
		
		//WDTCONbits.WDTPS = 0x10;//64S,设置看门狗唤醒时间，配置为设置休眠时看门狗禁止
		do
		{
			BAUD1CONbits.WUE = 1;
			CPUDOZEbits.IDLEN = 0;//使能串口在休眠状态下唤醒
			asm("sleep");
		}while(1==STATUSbits.nPD);
		DEBUG_PRINTFMASSAGES("out sleep");
		
		Flag.Start.cursta = TRUE;
		WDTCONbits.WDTPS = 0x09;//512ms
    }  
#endif

}

/*******************************************************************************
* Function Name  : Misc_PortInit
* Description    : 初始化IO口，把需要的IO初始化到需要的状态
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-03-21
* Author         : LL
*******************************************************************************/
static void Misc_PortInit(void)
{
	GPIO1_IN;GPIO2_IN;GPIO3_IN;GPIO4_IN;GPIO5_IN;
	GPIO6_IN;GPIO7_IN;GPIO8_IN;GPIO9_IN;GPIO10_IN;
	GPIO11_IN;GPIO12_IN;GPIO13_IN;
}

#if MISC_FWVERSION_EN
/*******************************************************************************
* Function Name  : Misc_FWVersionInit
* Description    : 固件版本号初始化
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-03-21
* Author         : LL
*******************************************************************************/
static void Misc_FWVersionInit(void)
{
    uint8_t i;
    uint8_t gang = '-';
    uint8_t *verstr = (uint8_t *)"V0.1";
    uint8_t *datestr = (uint8_t *)__DATE__;
    uint8_t *timestr = (uint8_t *)__TIME__;

    for (i = 0; i < 4; i++)
    {
        GlobalVar.FMVersion[i] = verstr[i];
    }
    GlobalVar.FMVersion[4] = gang;
    for (i = 0; i < 11; i++)
    {
        GlobalVar.FMVersion[i + 5] = datestr[i];
    }
    GlobalVar.FMVersion[16] = gang;
    for (i = 0; i < 9; i++)
    {
        GlobalVar.FMVersion[i + 17] = timestr[i];
    }
}
#endif /* MISC_FWVERSION_EN */

/*******************************************************************************
* Function Name  : Misc_DeviceOnOff
* Description    : 开关量处理
* Input          : devicenum:设备编号
                   sta:状态，NO打开 OFF关闭
* Output         : None
* Return         : None
* Date           : 2016-06-20
* Author         : LL
*******************************************************************************/
void Misc_DeviceOnOff(uint8_t devicenum, OnOffSatus sta)
{
    switch(devicenum)
    {
        case DEVICES1_NUM:
            if (ON == sta) 
            {
                DEVICES1_CLR;
            }
            else 
            {
                DEVICES1_SET;
            }
            break;
        case DEVICES2_NUM:
			if (ON == sta) 
            {
                DEVICES2_SET;
            }
            else 
            {
                DEVICES2_CLR;
            }
            break;
        case DEVICES3_NUM:
            if (ON == sta)
            {
                DEVICES3_SET;
            }
            else 
            {
                DEVICES3_CLR;
            }
            break; 
        default:
            break;
    }
}

/*******************************************************************************
* Function Name  : Misc_ReadCheckPin
* Description    : 读检测脚电平
* Input          : pinnum:引脚编号
* Output         : None
* Return         : ret:引脚状态
* Date           : 2016-06-20
* Author         : LL
*******************************************************************************/
uint8_t Misc_ReadCheckPin(uint8_t pinnum)
{
	uint8_t ret;
    switch(pinnum)
    {
        case GPIO1_NUM:
            if (0!=GPIO1_PIN) ret = LEVEL_HIGH;
            else ret = LEVEL_LOW;
            break;
        default:
            break;
    }
	return ret;
}

/*******************************************************************************
* Function Name  : Misc_GrayToBin
* Description    : 格雷码转换为二进制码
* Input          : None
* Output         : None
* Return         : 二进制编码
* Date           : 2017-08-03
* Author         : LL
*******************************************************************************/
uint16_t Misc_GrayToBin(uint16_t gray)
{
  //gray^=gray>>16;
  gray^=gray>>8;
  gray^=gray>>4;
  gray^=gray>>2;
  gray^=gray>>1;
  return gray;
}

/*******************************************************************************
* Function Name  : Misc_ReadGray
* Description    : 读编码
* Input          : None
* Output         : None
* Return         : 编码
* Date           : 2017-08-03
* Author         : LL
*******************************************************************************/
uint16_t Misc_ReadGray(void)
{
	uint16_t c;

	c=0;
    if (0!=DI1_PIN) c = 1;
	if (0!=DI2_PIN) c |= 1<<1;
	if (0!=DI3_PIN) c |= 1<<2;
	if (0!=DI4_PIN) c |= 1<<3;
	if (0!=DI5_PIN) c |= 1<<4;
	if (0!=DI6_PIN) c |= 1<<5;
	if (0!=DI7_PIN) c |= 1<<6;
	if (0!=DI8_PIN) c |= 1<<7;
	if (0!=DI9_PIN) c |= 1<<8;
	if (0!=DI10_PIN) c |= 1<<9;
	if (0!=DI11_PIN) c |= 1<<10;
	if (0!=DI12_PIN) c |= 1<<11;
    #if MISC_GRAY_BITS //第十三位使能选择
	if (0!=DI13_PIN) c |= 1<<12;
	#endif
	DEBUG_PRINTFNUMBERBCD(Misc_GrayToBin(c));
	return c;
}


/*******************************************************************************
* Function Name  : Misc_SetParameter
* Description    : 设置一协议系统参数，主要是MODBUS地址和波特率

这个函数要在初始化串口之前调用?

* Input          : None
* Output         : None
* Return         : None
* Date           : 2017-06-30
* Author         : LL
*******************************************************************************/
void Misc_SetParameter(void)
{
	uint8_t id;
	uint32_t baud;
	uint8_t buf[4];

	//设备ID初始化
	Eeprom_Read(MODBUS_DEVICEID_ADDR,buf,1);
    id = buf[0];
    if ((0 == id) || (id > MODBUS_DEVICEID_MAX))//ID不合法
    {
        id = MODBUS_DEVICEID;//设置为默认ID
        Eeprom_Write(MODBUS_DEVICEID_ADDR, &id, 1);
    }
	GlobalVar.DeviceID =id;

	//波特率初始化
    Eeprom_Read(MODBUS_UARTBAUDRATE_ADDR,buf,4);
    baud = buf[0];
    baud = (baud<<8) | buf[1];
    baud = (baud<<8) | buf[2];
    baud = (baud<<8) | buf[3];
    switch (baud)
    {
        case UART_BAUDRATE1200:
        case UART_BAUDRATE2400:
        case UART_BAUDRATE4800:
        case UART_BAUDRATE9600:
        case UART_BAUDRATE19200:
        case UART_BAUDRATE57600:
        case UART_BAUDRATE115200:
            break;
        default:
            baud = UART_BAUDRATE;
            buf[0] = (baud>>24) & 0xFF;
            buf[1] = (baud>>16) & 0xFF;
            buf[2] = (baud>>8) & 0xFF;
            buf[3] = baud & 0xFF;
            Eeprom_Write(MODBUS_DEVICEID_ADDR,buf,4);
            break;
    }
    GlobalVar.UartBaudRate = baud;
	
}

/*******************************************************************************
* Function Name  : Misc_Init
* Description    : 不确定项初始化
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-06-20
* Author         : LL
*******************************************************************************/
void Misc_Init(void)
{
	
    Misc_PortInit();
#if MISC_FWVERSION_EN
    Misc_FWVersionInit();
#endif
	Flag.Start.cursta = TRUE;
	Flag.Start.presta = FALSE;
}

/*******************************************************************************
* Function Name  : Misc_Process
* Description    : 不确定项的处理
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-06-20
* Author         : LL
*******************************************************************************/
void Misc_Process(void)
{
    Misc_RunStatus();
	Misc_GraySample();
}
/* ************ ****** ************ THE FILE END  ************ ****** ************ */
