/*
 * uart0.h
 *
 * Created: 2016/3/16 16:14:16
 *  Author: Administrator
 */ 


#ifndef __UART_H_
#define __UART_H_

//波特率定义
#define UART_BAUDRATE1200 1200
#define UART_BAUDRATE2400 2400
#define UART_BAUDRATE4800 4800
#define UART_BAUDRATE9600 9600
#define UART_BAUDRATE19200 19200
#define UART_BAUDRATE57600 57600
#define UART_BAUDRATE115200 115200

#define UART_BAUDRATE 9600
     
extern void UART_PutChar(uint8_t data);
extern void UART_PrintfString(uint8_t *buf);
extern void UART_PrintfNormal(uint8_t *buf, uint16_t len);
extern void UART_Init(void);
extern void USART_Process(void);


#endif /* UART0_H_ */