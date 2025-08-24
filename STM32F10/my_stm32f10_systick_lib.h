#ifndef __MY_STM32F10_SYSTICK_LIB
#define __MY_STM32F10_SYSTICK_LIB

#include  "my_stm32f10_systick_driver.h"
#include "CoroType.h"
#define PROTEUS_SIMULATION

class SYSTICK{
	
	public:
			SYSTICK(uint32_t freq);
			void LIB_SYSTICK_Delay(uint32_t sec);
			void LIB_SYSTICK_DelayMs(uint32_t milli);
      CoroType NonBlocking_SYSTICK_DelayMs(uint32_t milli);
      CoroType  NonBlockingSYSTICK_Delay(uint32_t sec);
  private:
      uint32_t Xtal_Freq{};

};

#endif