#include "task.h"

CoroType blinking_led_task1() {
    while (true) {
        led_Yellow.Toggle();       
        for (int i = 0; i < 100; ++i) { /*Non Blocking Delay --> TBD: use Systick_timer*/ 
            co_yield 0;
        }
    }
}

CoroType blinking_led_task2() {
    while (true) {
        led_Green.Toggle();
        for (int i = 0; i < 5000; ++i) { /*Non Blocking Delay --> TBD: use Systick_timer*/ 
            co_yield 0;
        }
    }
}

CoroType DTC_LED_Alarm() {
    while (true) {
            if(Temprature_DTC->get_event_status()){
              led_DTC.ON();
            }
            else{
              led_DTC.OFF();
            }
            co_yield 0;
        }
    }

CoroType serial_monitor_task() {
    while (true) {
        // Check for incoming UART data (non-blocking way, e.g., co_await for data)
        // ...
          
         char* str;
         uart_cin >> str;
         uart_cout << str << uart_endl;
        
         co_yield 0;
    }
}
CoroType Temperature_read_task() {
    while(1){
    TemperatureSensor_value = T_sens(); /*Read Temperature Sensor value*/
    co_yield 0;
  }
}


CoroType Get_diag_info(){
    while(1){
    if(Diag_button()){
      Diag.Show_info();
      for (int i = 0; i < 1000; ++i) { /*Debouncing*/
            co_yield 0;
        }
    }
    co_yield 0;
  }
}