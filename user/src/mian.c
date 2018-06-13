
#define __MAIN_C__

#include "config.h"

// PIC16F18344 Configuration Bit Settings
// 'C' source line config statements
// CONFIG1
#pragma config FEXTOSC = OFF//HS     // FEXTOSC External Oscillator mode Selection bits (HS (crystal oscillator) above 4 MHz)
#pragma config RSTOSC = HFINT32   // Power-up default value for COSC bits (EXTOSC operating per FEXTOSC bits)
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled; I/O or oscillator function on OSC2)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config MCLRE = ON       // Master Clear Enable bit (MCLR/VPP pin function is MCLR; Weak pull-up enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config WDTE = SLEEP        // Watchdog Timer Enable bits (WDT enabled, SWDTEN is ignored)
#pragma config LPBOREN = OFF    // Low-power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bits (Brown-out Reset enabled, SBOREN bit ignored)
#pragma config BORV = HIGH      // Brown-out Reset Voltage selection bit (Brown-out voltage (Vbor) set to 2.7V)
#pragma config PPS1WAY = OFF    // PPSLOCK bit One-Way Set Enable bit (The PPSLOCK bit can be set and cleared repeatedly (subject to the unlock sequence))
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will cause a Reset)
#pragma config DEBUG = OFF//ON       // Debugger enable bit (Background debugger enabled)

// CONFIG3
#pragma config WRT = OFF        // User NVM self-write protection bits (Write protection off)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low Voltage programming enabled. MCLR/VPP pin function is MCLR. MCLRE configuration bit is ignored.)

// CONFIG4
#pragma config CP = OFF         // User NVM Program Memory Code Protection bit (User NVM code protection disabled)
#pragma config CPD = OFF        // Data NVM Memory Code Protection bit (Data NVM code protection disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define FWDT_RELOAD asm ("CLRWDT")

static void SystemCLK_Init(void)
{
    OSCCON1bits.NDIV = 2;//设置系统分频比为2--4 0--1
    //打盹模式配置
    #if 0
    CPUDOZEbits.DOZE = 2;//打盹模式外设指令周期比2--1:8
    CPUDOZEbits.ROI = 1;//发生中断时全速运行
	CPUDOZEbits.DOE = 1;//退出中断时降速运行
    CPUDOZEbits.DOZEN = 1;//使能打盹模式
	#endif
}
//锁定PPS设置
void PPSLock_Set(void)
{
	// Lock Registers
    PPSLOCK = 0x55; // 第一个锁定代码
    PPSLOCK = 0xAA; // 第二个锁定代码
    PPSLOCKbits.PPSLOCKED = 0x01; // 锁定 PPS
}
//解锁PPS设置
void PPSUnlock_Set(void)
{
	// Unlock Registers
	PPSLOCK = 0x55; // 第一个解锁代码
    PPSLOCK = 0xAA; // 第二个解锁代码
    PPSLOCKbits.PPSLOCKED = 0x00; // 解锁 PPS
}
static void WDT_Init(void)
{
	WDTCONbits.WDTPS = 0x0A;//0x09--512mS,0x0A--1S,0x0B--2S,0x11--128S
}

int main()
{
    GIE = 0;
    SystemCLK_Init();
	PPSUnlock_Set();
	Misc_SetParameter();
    Misc_Init();
	UART_Init();
    Timer_Init();
	PPSLock_Set();
	WDT_Init();
	PEIE = 1;
    GIE = 1;
    
    while(1)
    {
        USART_Process();
        Misc_Process();
        FWDT_RELOAD;
    }
}