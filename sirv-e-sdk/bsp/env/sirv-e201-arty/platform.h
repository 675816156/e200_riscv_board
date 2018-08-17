// See LICENSE for license details.

#ifndef _SIFIVE_PLATFORM_H
#define _SIFIVE_PLATFORM_H

#include <stdint.h>
#include "sifive/const.h"
#include "sifive/devices/aon.h"
#include "sifive/devices/clint.h"
#include "sifive/devices/gpio.h"
#include "sifive/devices/plic.h"
#include "sifive/devices/pwm.h"
#include "sifive/devices/spi.h"
#include "sifive/devices/uart.h"
#include "sifive/devices/i2c.h"


// Some things missing from the official encoding.h
#define MCAUSE_INT         0x80000000
#define MCAUSE_CAUSE       0x7FFFFFFF

/****************************************************************************
 * Platform definitions
 *****************************************************************************/
#define TRAPVEC_TABLE_CTRL_ADDR _AC(0x00001010,UL)
#define CLINT_CTRL_ADDR         _AC(0x02000000,UL)
#define PLIC_CTRL_ADDR          _AC(0x0C000000,UL)
#define AON_CTRL_ADDR           _AC(0x10000000,UL)
#define GPIO_CTRL_ADDR          _AC(0x10012000,UL)
#define UART0_CTRL_ADDR         _AC(0x10013000,UL)
#define SPI0_CTRL_ADDR          _AC(0x10014000,UL)
#define PWM0_CTRL_ADDR          _AC(0x10015000,UL)
#define UART1_CTRL_ADDR         _AC(0x10023000,UL)
#define SPI1_CTRL_ADDR          _AC(0x10024000,UL)
#define PWM1_CTRL_ADDR          _AC(0x10025000,UL)
#define SPI2_CTRL_ADDR          _AC(0x10034000,UL)
#define PWM2_CTRL_ADDR          _AC(0x10035000,UL)
#define I2C_CTRL_ADDR           _AC(0x10042000,UL)
#define SPI0_MMAP_ADDR          _AC(0x20000000,UL)
#define MEM_CTRL_ADDR           _AC(0x80000000,UL)

// Helper functions
#define _REG8(p, i)             (*(volatile uint8_t *) ((p) + (i)))
#define _REG32(p, i)            (*(volatile uint32_t *) ((p) + (i)))
#define _REG32P(p, i)           ((volatile uint32_t *) ((p) + (i)))
#define AON_REG(offset)         _REG32(AON_CTRL_ADDR, offset)
#define CLINT_REG(offset)       _REG32(CLINT_CTRL_ADDR, offset)
#define GPIO_REG(offset)        _REG32(GPIO_CTRL_ADDR, offset)
#define OTP_REG(offset)         _REG32(OTP_CTRL_ADDR, offset)
#define PLIC_REG(offset)        _REG32(PLIC_CTRL_ADDR, offset)
#define PRCI_REG(offset)        _REG32(PRCI_CTRL_ADDR, offset)
#define PWM0_REG(offset)        _REG32(PWM0_CTRL_ADDR, offset)
#define PWM1_REG(offset)        _REG32(PWM1_CTRL_ADDR, offset)
#define PWM2_REG(offset)        _REG32(PWM2_CTRL_ADDR, offset)
#define SPI0_REG(offset)        _REG32(SPI0_CTRL_ADDR, offset)
#define SPI1_REG(offset)        _REG32(SPI1_CTRL_ADDR, offset)
#define SPI2_REG(offset)        _REG32(SPI2_CTRL_ADDR, offset)
#define UART0_REG(offset)       _REG32(UART0_CTRL_ADDR, offset)
#define UART1_REG(offset)       _REG32(UART1_CTRL_ADDR, offset)
#define I2C_REG(offset)         _REG8(I2C_CTRL_ADDR, offset)

// Interrupt Numbers
#define INT_RESERVED    0
#define INT_WDOGCMP     1
#define INT_RTCCMP      2
#define INT_UART0_BASE  3
#define INT_UART1_BASE  4
#define INT_SPI0_BASE   5
#define INT_SPI1_BASE   6
#define INT_SPI2_BASE   7
#define INT_GPIO_BASE   8
#define INT_PWM0_BASE   40
#define INT_PWM1_BASE   44
#define INT_PWM2_BASE   48
#define INT_I2C_BASE    52

// IOF Mappings
#define IOF0_UART0_MASK         _AC(0x00030000, UL)

#define NUM_GPIO 5

#define PLIC_NUM_INTERRUPTS 52
#define PLIC_NUM_PRIORITIES 7

//the sch is error
#define RED_LED_OFFSET   1
#define BLUE_LED_OFFSET  2
#define GREEN_LED_OFFSET 3
#define UART0_RX_OFFSET  16
#define UART0_TX_OFFSET  17

/*
// These are *PIN* numbers, not
// GPIO Offset Numbers.
#define PIN_SPI1_SCK    (13u)
#define PIN_SPI1_MISO   (12u)
#define PIN_SPI1_MOSI   (11u)
#define PIN_SPI1_SS0    (10u)
#define PIN_SPI1_SS1    (14u) 
#define PIN_SPI1_SS2    (15u)
#define PIN_SPI1_SS3    (16u)

#define SS_PIN_TO_CS_ID(x) \
  ((x==PIN_SPI1_SS0 ? 0 :                \
    (x==PIN_SPI1_SS1 ? 1 :               \
     (x==PIN_SPI1_SS2 ? 2 :              \
      (x==PIN_SPI1_SS3 ? 3 :             \
       -1))))) 
*/

#ifdef HAS_BOARD_BUTTONS
#undef HAS_BOARD_BUTTONS
#endif

// These buttons are present only on the Freedom E300 Arty Dev Kit.
#ifdef HAS_BOARD_BUTTONS
#define BUTTON_0_OFFSET 15
#define BUTTON_1_OFFSET 30
#define BUTTON_2_OFFSET 31

#define INT_DEVICE_BUTTON_0 (INT_GPIO_BASE + BUTTON_0_OFFSET)
#define INT_DEVICE_BUTTON_1 (INT_GPIO_BASE + BUTTON_1_OFFSET)
#define INT_DEVICE_BUTTON_2 (INT_GPIO_BASE + BUTTON_2_OFFSET)

#endif

#define HAS_HFXOSC 1
#define HAS_LFROSC_BYPASS 1

#define RTC_FREQ 32768

void write_hex(int fd, unsigned long int hex);

unsigned long get_timer_freq(void);
uint64_t get_timer_value(void);
uint64_t get_instret_value(void);
uint64_t get_cycle_value(void);

#endif /* _SIFIVE_PLATFORM_H */
