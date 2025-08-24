/********************************************************************************
 * File: main.cpp
 * Author: Amr Elsayed
 * Date: August 24, 2025
 * Description: Main application file for an STM32F103 microcontroller,
 * demonstrating a cooperative multitasking scheduler
 * using C++20 coroutines. Manages various tasks including
 * LED blinking, button monitoring, serial communication,
 * and ADC temperature reading.
 **********************************************************************************/

#include "my_stm32f10_uart_lib.h"
#include "my_stm32f10_gpio_lib.h"
#include "my_stm32f10_systick_lib.h"
#include "my_stm32f10_adc_lib.h"
#include "utils.h"
#include "uart_stream.h"
#include "CoroType.h"
#include "VTiny_Os.h"
#include "task.h"
#include "LED.h"
#include "PushButton.h"
#include "TemperatureSensor.h"
#include "DEM.h"
#include "Diag_mgr.h"
/***********************************************GPIO**********************************************************************************************/
GPIO PortA(PORTA);
GPIO PortB(PORTB);
GPIO_PIN& PA12 = PortA.CreatePin(12, GPIO_MODE_OUTPUT_PP_GENERIC, GPIO_NOPULL, GPIO_SPEED_HIGH, 0);
GPIO_PIN& PA13 = PortA.CreatePin(13, GPIO_MODE_OUTPUT_PP_GENERIC, GPIO_NOPULL, GPIO_SPEED_HIGH, 0);
GPIO_PIN& PA14 = PortA.CreatePin(14, GPIO_MODE_OUTPUT_PP_GENERIC, GPIO_NOPULL, GPIO_SPEED_HIGH, 0);
GPIO_PIN& PA15 = PortA.CreatePin(15, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_HIGH, 0);
GPIO_PIN& PB0 =  PortB.CreatePin(0, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_HIGH, 0);
GPIO_PIN& PB15 =  PortB.CreatePin(15, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_HIGH, 0);

/***********************************************Timer**********************************************************************************************/
SYSTICK delay(48000000);
/***********************************************UART**********************************************************************************************/
UART UART2(RX_AND_TX,_115200,&PortA);
UART_Stream uart_cout(UART2);
UART_Stream uart_cin(UART2);
/***********************************************ADC**********************************************************************************************/
ADC_Driver my_adc(&PortA,ADC1);
ADC_Channel AD0 = my_adc.Creat_Channel(ADC_CHANNEL_0);
ADC_Channel AD6 = my_adc.Creat_Channel(ADC_CHANNEL_6);
/***********************************************Devices*******************************************************************************************/
LED    led_DTC(&PA13, "Red");
LED    led_Green(&PA14, "Green");
LED    led_Yellow(&PA12, "Yellow");
Button Diag_button(&PB15);
TemperatureSensor T_sens(&AD6);
/***********************************************Global_variables*******************************************************************************************/
uint16_t TemperatureSensor_value{};
/***********************************************External_variables*******************************************************************************************/
extern DEM<4> Diag;
/***********************************************Main*******************************************************************************************/
int main() {

  CoroutineScheduler<6> Os; // Scheduler for N tasks using Class Template  
  /*Tasks are defined in task.h, Diag.Mgr.h and 
    implemented in task.cpp, Diag.Mgr.cpp
    adding Tasks to the Scheduler*/
  Os.add_task(Temperature_read_task()); //Reading Sensors Values 
  Os.add_task(DTC_LED_Alarm());    // Toggle LED as alarm for DTC
  Os.add_task(blinking_led_task2());  // Blinking LED task 1 
  Os.add_task(blinking_led_task1());  // Blinking LED task 2  
  Os.add_task(Get_diag_info()); // Push Button to reveal the Diagnostic events info 
  Os.add_task(Diag_Mgr()); // Diagnotic events monitoring
  /*Hello World!*/
  uart_cout << "Hello World !" << uart_endl; //std::cout << "Hello World !" << std::endl;

  /*Run The Scheduler*/
  Os.run(); //infinite <---- Using Coroutines C++20


  while(1) {
                //empty
                //unreachable
          }

  return 0;    //unreachable

}