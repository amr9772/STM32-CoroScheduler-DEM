#include "my_stm32f10_systick_lib.h"
#include "uart_stream.h"


SYSTICK::SYSTICK(uint32_t freq) : Xtal_Freq(freq){
}

void SYSTICK::LIB_SYSTICK_Delay(uint32_t sec){
    // Since 72,000,000 exceeds 24-bit max, use millisecond delay in a loop
    #ifdef PROTEUS_SIMULATION
    sec *= 300;
    #endif
    for(uint32_t i = 0; i < sec; i++) {
        LIB_SYSTICK_DelayMs(1000); 
    }
}

void SYSTICK::LIB_SYSTICK_DelayMs(uint32_t milli){

	uint32_t i;
	
		/*Configure SysTick*/
	SysTick->LOAD =  (Xtal_Freq/1000) -1;
	SysTick->VAL =0; /*clear current value register*/
	SysTick->CTRL = 5; /*Enable systick, no interrupt*/
	
		for(i=0;i<milli;i++){
		
		/*Wait until the COUNTFLAG is set*/
	  while((SysTick->CTRL &0x10000)== 0){}
	}
	
	SysTick->CTRL =0;  /*Stop the timer (En =0)*/
	
}

CoroType SYSTICK::NonBlocking_SYSTICK_DelayMs(uint32_t milli){

	uint32_t i;
	
		/*Configure SysTick*/
	SysTick->LOAD =  (Xtal_Freq/1000) -1;
	SysTick->VAL =0; /*clear current value register*/
	SysTick->CTRL = 5; /*Enable systick, no interrupt*/
	//uart_cout << "HELLO" << uart_endl;
		for(i=0;i<milli;i++){
		//uart_cout << i << uart_endl;
		/*Wait until the COUNTFLAG is set*/
	  while((SysTick->CTRL &0x10000)== 0){}
		co_yield 0;
	}
	
	SysTick->CTRL =0;  /*Stop the timer (En =0)*/
	co_return;
}

CoroType SYSTICK::NonBlockingSYSTICK_Delay(uint32_t sec){
    // Since 72,000,000 exceeds 24-bit max, use millisecond delay in a loop
    #ifdef PROTEUS_SIMULATION
    sec *= 300;
    #endif
    uint32_t milli = 1000;
    for(uint32_t j = 0; j < sec; j++) {
        uint32_t i;
	
          /*Configure SysTick*/
        SysTick->LOAD =  (Xtal_Freq/1000) -1;
        SysTick->VAL =0; /*clear current value register*/
        SysTick->CTRL = 5; /*Enable systick, no interrupt*/
        //uart_cout << "HELLO" << uart_endl;
          for(i=0;i<milli;i++){
          //uart_cout << i << uart_endl;
          /*Wait until the COUNTFLAG is set*/
          while((SysTick->CTRL &0x10000)== 0){}
          co_yield 0;
        }
        
        SysTick->CTRL =0;  /*Stop the timer (En =0)*/
      co_return;  
    }
    //co_return;
}