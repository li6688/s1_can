/**
  ******************************************************************************
  * @file    config.h
  * @author  LL
  * @version V0.0.0
  * @date    2016-09-19
  * @brief   ≈‰÷√Õ∑Œƒº˛
  ******************************************************************************
  ******************************************************************************
***/

#ifndef __CONFIG_H__
#define	__CONFIG_H__

#include <xc.h>
#include <stdint.h>

#include "sys.h"

#ifdef __MAIN_C__
#include "uart.h"
#include "uart_printf.h"
#include "timer.h"
#include "misc.h"
#include "eeprom.h"
#include "ad7790.h"
#endif /* __MAIN_C__ */

#ifdef __TIMER_C__
#include "timer.h"
#include "uart.h"
#endif /* __TIMER_C__*/

#ifdef __UART_C__
#include "uart.h"
//#include "fifo.h"
#include "uart_printf.h"
#include "crc16.h"
#include "modbus.h"
#endif /* __UART_C__ */

#ifdef __UART_PRINTF_C__
#include "uart_printf.h"
#include "uart.h"
#endif /* __UART_PRINTF_C__ */

#ifdef __MODBUS_C__
#include "uart_printf.h"
#include "uart.h"
#include "modbus.h"
#include "crc16.h"
#include "eeprom.h"
#include "misc.h"
#endif /* __MODBUS_C__ */

#ifdef __INTERRUPT_C__
#include "modbus.h"
#include "misc.h"
#endif /* __INTERRUPT_C__ */

#ifdef __CRC16_C__
#include "crc16.h"
#endif /* __CRC16_C__ */

#ifdef __ADC_C__
#include "adc.h"
#include "uart_printf.h"
#endif /* __ADC_C__ */

#ifdef __SPI_C__
#include "spi.h"
#endif

#ifdef __LED_C__
#include "led.h"
#endif /* __LED_C__ */

#ifdef __KEY_C__
#include "key.h"
#include "timer.h"
#include "uart.h"
#endif /* __KEY_C__ */

#ifdef __MISC_C__
#include "timer.h"
#include "misc.h"
#include "uart_printf.h"
#include "eeprom.h"
#include "modbus.h"
#include "uart.h"
#include "ad7790.h"
#endif /* __MISC_C__ */

#ifdef __TEMPERATURE_C__
#include "uart_printf.h"
#include "MLX90614.h"
#include "adc.h"
#include "temperature.h"
#include "misc.h"
#include "protocol.h"
#endif /* __TEMPERATURE_C__ */

#ifdef __MOTOR_C__
#include "motor.h"
#endif /* __MOTOR_C__ */

#ifdef __ENERGY_C__
#include "energy.h"
#include "misc.h"
#include "protocol.h"
#include "uart_printf.h"
#endif /* __MOTOR_C__ */


#ifdef __POWERMANAGEMENT_C__
#include "adc.h"
#include "misc.h"
#include "uart.h"
#include "PowerManagement.h"
#endif /* __POWERMANAGEMENT_C__ */

#ifdef __RV3049_C__
#include "spi.h"
#include "rv3049.h"
#include "uart.h"
#endif /* __RV3049_C__ */

#ifdef __MCP980X_C__
#include "uart.h"
#endif /* __MCP980X_C__ */

#ifdef __BFM25L16_C__
#include "fm25l16b.h"
#include "uart.h"
#include "spi.h"
#endif /* __BFM25L16_C__ */

#ifdef __FLASHUPDATA_C__
#include "flashupdata.h"
#include "uart.h"
#include "uart_printf.h"
#endif /* __FALSHUPDATA_C__ */

#ifdef __EEPROM_C__
#include "uart_printf.h"
#include "eeprom.h"
#endif /* __EEPROM_C__ */

#endif /* __CONFIG_H__ */

/* ************ ****** ************ THE FILE END  ************ ****** ************ */


