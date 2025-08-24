#ifndef __MY_STM32F10_UART_DRIVER_H
#define __MY_STM32F10_UART_DRIVER_H

#include  "my_stm32f10_gpio_driver.h"


#define APB1PERIPH_BASE 			(PERIPH_BASE + 0x00000U)
#define USART2_BASE 					(APB1PERIPH_BASE + 0x4400UL)

typedef struct {
    volatile uint16_t SR;   /* USART Status Register */
    uint16_t RESERVED0;
    volatile uint16_t DR;   /* USART Data Register */
    uint16_t RESERVED1;
    volatile uint16_t BRR;  /* USART Baud Rate Register */
    uint16_t RESERVED2;
    volatile uint16_t CR1;  /* USART Control Register 1 */
    uint16_t RESERVED3;
    volatile uint16_t CR2;  /* USART Control Register 2 */
    uint16_t RESERVED4;
    volatile uint16_t CR3;  /* USART Control Register 3 */
    uint16_t RESERVED5;
    volatile uint16_t GTPR; /* USART Guard time and prescaler register */
    uint16_t RESERVED6;
} USART_TypeDef;

#define USART2 ((USART_TypeDef *)USART2_BASE)



#endif