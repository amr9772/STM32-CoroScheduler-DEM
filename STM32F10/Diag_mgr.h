#ifndef Diag_MGR_H
#define Diag_MGR_H

#include "DEM.h"
#include "CoroType.h"
#include "my_stm32f10_adc_lib.h"
#include "LED.h"
#include "PushButton.h"
#include "TemperatureSensor.h"
#include "my_stm32f10_systick_lib.h"

extern uint16_t TemperatureSensor_value;

extern DEM_event* Temprature_Above_Threshold;
extern DEM_event* Temprature_Below_Threshold;
extern DEM_event* Temprature_Invalid_value;
extern DEM_event* Temprature_DTC;

void Temprature_Above_Threshold_test();
void Temprature_Below_Threshold_test();
void Temprature_Invalid_value_test();
void Temprature_DTC_check();

CoroType Diag_Mgr();


#endif