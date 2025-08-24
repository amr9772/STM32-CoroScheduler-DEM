#ifndef LED_H
#define LED_H
#include "my_stm32f10_gpio_lib.h"
#include "uart_stream.h"

class LED{
  friend inline UART_Stream& operator << (UART_Stream& print,LED led);
  public:
    LED() = default;
    LED(GPIO_PIN* pin_,const char* color_ ) : pin(pin_),color(color_){}
    LED(GPIO_PIN* pin_) : pin(pin_){
      color = "undefined";
    }
    void ON(){
      pin->WritePin(GPIO_PIN_SET);
      status = true;
    }
    void OFF(){
      pin->WritePin(GPIO_PIN_RESET);
      status = false;
    }
    void Toggle(){
      pin->TogglePin();
      status = false;
    }
    
  private:
    GPIO_PIN* pin = nullptr;
    const char* color   = nullptr;
    bool status{};



};


inline UART_Stream& operator << (UART_Stream& print,LED led){
print << "-LED- color: " << led.color <<  " status: " << (led.status?"ON":"OFF") << " pin: " << led.pin->Get_pin_number();
return print;
}




#endif