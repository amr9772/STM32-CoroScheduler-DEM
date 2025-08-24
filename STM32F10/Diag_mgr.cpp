#include "Diag_mgr.h"
#include "DEM.h"


void Temprature_Above_Threshold_test(){
    if(TemperatureSensor_value>100 && !Temprature_Invalid_value->get_event_status()){
      Temprature_Above_Threshold->set_event_status(PRE_FAILED);
    }
    else{
      Temprature_Above_Threshold->set_event_status(PRE_PASSED);
    }
}
void Temprature_Below_Threshold_test(){
      if(TemperatureSensor_value<27 && !Temprature_Invalid_value->get_event_status()){
        Temprature_Below_Threshold->set_event_status(PRE_FAILED);
      }
      else{
        Temprature_Below_Threshold->set_event_status(PRE_PASSED);
      }
}
void Temprature_Invalid_value_test(){
      if(TemperatureSensor_value>150){
        Temprature_Invalid_value->set_event_status(PRE_FAILED);
      }
      else{
        Temprature_Invalid_value->set_event_status(PRE_PASSED);
      }
}
void Temprature_DTC_check(){  
  if(Temprature_Above_Threshold->get_event_status()
    || Temprature_Below_Threshold->get_event_status()
    || Temprature_Invalid_value->get_event_status()
    ){
      Temprature_DTC->set_event_status(FAILED);
    }
  else{
    Temprature_DTC->set_event_status(PASSED);
  }
}


CoroType Diag_Mgr(){
  while (1)
  {
    Temprature_Above_Threshold_test();
    co_yield 1;
    Temprature_Below_Threshold_test();
    co_yield 2;
    Temprature_Invalid_value_test();
    co_yield 3;
    Temprature_DTC_check();
    co_yield 4;
  }
  
}