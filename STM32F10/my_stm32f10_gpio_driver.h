#ifndef __MY_STM32F10_GPIO_DRIVER_H__
#define __MY_STM32F10_GPIO_DRIVER_H__

#include <stdint.h>

// Base addresses for STM32F103C4
#define PERIPH_BASE         (0x40000000U)
#define APB2PERIPH_BASE     (PERIPH_BASE + 0x10000U)
#define AHBPERIPH_BASE      (PERIPH_BASE + 0x20000U)

// RCC Base for STM32F103C4
#define RCC_BASE            (AHBPERIPH_BASE + 0x1000U)

// GPIO Port Bases for STM32F103C4
#define GPIOA_BASE          (APB2PERIPH_BASE + 0x0800U)
#define GPIOB_BASE          (APB2PERIPH_BASE + 0x0C00U)
#define GPIOC_BASE          (APB2PERIPH_BASE + 0x1000U)
#define GPIOD_BASE          (APB2PERIPH_BASE + 0x1400U)


// GPIO Register Definition for STM32F1 series
// IMPORTANT: All members of hardware register structs MUST be volatile
struct GPIO_TypeDef
{
    volatile uint32_t CRL;
    volatile uint32_t CRH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
};

// RCC Register Definition for STM32F1 series
// IMPORTANT: All members of hardware register structs MUST be volatile
struct RCC_TypeDef
{
    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB1RSTR;
    volatile uint32_t AHBENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t APB1ENR;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
};

// Macro definitions for accessing peripheral registers as structs
// IMPORTANT: The pointers themselves should also be volatile
#define RCC                 ((volatile RCC_TypeDef  *)RCC_BASE)
#define GPIOA               ((volatile GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB               ((volatile GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC               ((volatile GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD               ((volatile GPIO_TypeDef *)GPIOD_BASE)

// Clock enable bits for GPIOs in RCC_APB2ENR for STM32F1
#define GPIOA_EN_BIT        (1 << 2)
#define GPIOB_EN_BIT        (1 << 3)
#define GPIOC_EN_BIT        (1 << 4)
#define GPIOD_EN_BIT        (1 << 5)

#endif
