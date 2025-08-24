#ifndef TASKS_H
#define TASKS_H
#include "CoroType.h"
#include "my_stm32f10_adc_lib.h"
#include "LED.h"
#include "PushButton.h"
#include "TemperatureSensor.h"
#include "my_stm32f10_systick_lib.h"
#include "DEM.h"

extern LED led_DTC;
extern LED led_Green;
extern LED led_Yellow;
extern Button Diag_button;
extern TemperatureSensor T_sens;

extern uint16_t TemperatureSensor_value;

extern DEM<4> Diag;
extern DEM_event* Temprature_DTC;

CoroType blinking_led_task1();
CoroType blinking_led_task2();
CoroType DTC_LED_Alarm();
CoroType serial_monitor_task();
CoroType Temperature_read_task();
CoroType Get_diag_info();





#endif