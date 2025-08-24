#ifndef TEMPERATURE_H
#define TEMPERATURE_H
#include "my_stm32f10_gpio_lib.h"
#include "uart_stream.h"
#include "my_stm32f10_adc_lib.h"


class TemperatureSensor{
  friend inline UART_Stream& operator << (UART_Stream& print,TemperatureSensor sensor);
  public:
    TemperatureSensor() = default;
    TemperatureSensor(ADC_Channel* pin_) : pin(pin_){
      adc_val = pin->readChannel();
      if(adc_val == 0) Temperature = -1;
      else Temperature = (adc_val*150/1867);
    }
    uint16_t Read(){
      adc_val = pin->readChannel();
      if(adc_val == 0) Temperature = -1;
      else Temperature = (adc_val*150/1867);
      return Temperature;
    }
    uint16_t operator() (){
      adc_val = pin->readChannel();
      if(adc_val == 0) Temperature = -1;
      else Temperature = (adc_val*150/1867);
      return Temperature;
    }
    
  private:
    ADC_Channel* pin = nullptr;
    uint16_t     adc_val{};
    uint16_t     Temperature{};
};


inline UART_Stream& operator << (UART_Stream& print,TemperatureSensor sensor){
if(sensor()>150) print << "Temperature sensor--> "<<  " Temperature: " << "Invalid";
else print << "Temperature sensor--> "<<  " Temperature: " << sensor();
return print;
}




#endif