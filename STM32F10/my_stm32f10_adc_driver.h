#ifndef __MY_STM32F10_ADC_DRIVER_H__
#define __MY_STM32F10_ADC_DRIVER_H__

#include  "my_stm32f10_gpio_driver.h"
#include <stdint.h>

#define ADC1_BASE             (APB2PERIPH_BASE + 0x2400UL) // ADC1

// ADC Registers Structure
typedef struct {
    volatile uint32_t SR;        // Status Register
    volatile uint32_t CR1;       // Control Register 1
    volatile uint32_t CR2;       // Control Register 2
    volatile uint32_t SMPR1;     // Sample Time Register 1 (channels 10-17)
    volatile uint32_t SMPR2;     // Sample Time Register 2 (channels 0-9)
    volatile uint32_t JOFR1;     // Injected Channel Data Offset Register 1
    volatile uint32_t JOFR2;     // Injected Channel Data Offset Register 2
    volatile uint32_t JOFR3;     // Injected Channel Data Offset Register 3
    volatile uint32_t JOFR4;     // Injected Channel Data Offset Register 4
    volatile uint32_t HTR;       // High Threshold Register
    volatile uint32_t LTR;       // Low Threshold Register
    volatile uint32_t SQR1;      // Regular Sequence Register 1 (conversions 13-16)
    volatile uint32_t SQR2;      // Regular Sequence Register 2 (conversions 7-12)
    volatile uint32_t SQR3;      // Regular Sequence Register 3 (conversions 1-6)
    volatile uint32_t JSQR;      // Injected Sequence Register
    volatile uint32_t JDR1;      // Injected Data Register 1
    volatile uint32_t JDR2;      // Injected Data Register 2
    volatile uint32_t JDR3;      // Injected Data Register 3
    volatile uint32_t JDR4;      // Injected Data Register 4
    volatile uint32_t DR;        // Regular Data Register
} ADC_Type;

#define ADC1 ((ADC_Type *)ADC1_BASE)



#endif
