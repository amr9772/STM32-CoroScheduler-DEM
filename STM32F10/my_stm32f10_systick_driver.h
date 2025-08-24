#ifndef __MY_STM32F10_SYSTICK_DRIVER_H
#define	__MY_STM32F10_SYSTICK_DRIVER_H
#include <stdint.h>

#define 		SCB_BASE			(0xE000E000UL)  /*System Control Bse Address*/


#define 		SysTick_Offset		0x010UL
#define 		SysTick_BASE		(SCB_BASE +  SysTick_Offset)



typedef struct{

volatile  uint32_t CTRL;   
volatile	uint32_t LOAD;    
volatile uint32_t VAL;    
volatile	uint32_t CALIB;	
	
}SysTick_Type;


#define SysTick ((SysTick_Type *)SysTick_BASE) /*SysTick configuration structure*/
#endif