#ifndef Button_H
#define Button_H
#include "my_stm32f10_gpio_lib.h"
#include "uart_stream.h"

class Button{
  friend inline UART_Stream& operator << (UART_Stream& print,Button btn);
  public:
    Button() = default;
    Button(GPIO_PIN* pin_) : pin(pin_){
      status = pin->ReadPin();
    }
    bool Read(){
      status = pin->ReadPin();
      return status;
    }
    bool operator() (){
      status = pin->ReadPin();
      return status;
    }
    
  private:
    GPIO_PIN* pin = nullptr;
    bool status{};
};


inline UART_Stream& operator << (UART_Stream& print,Button btn){
print << "Button: "<<  " status: " << btn() << " pin: " << btn.pin->Get_pin_number();
return print;
}




#endif