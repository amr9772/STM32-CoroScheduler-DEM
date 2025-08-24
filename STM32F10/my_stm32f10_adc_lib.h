#ifndef		__MY_STM32F10_ADC_LIB_H__
#define		__MY_STM32F10_ADC_LIB_H__

#include  "my_stm32f10_adc_driver.h"
#include  "my_stm32f10_gpio_lib.h"
// ADC Channel Definitions (for STM32F10x, PA0 to PA7 for ADC1)
typedef enum {
    ADC_CHANNEL_0 = 0,  // PA0
    ADC_CHANNEL_1 = 1,  // PA1
    ADC_CHANNEL_2 = 2,  // PA2
    ADC_CHANNEL_3 = 3,  // PA3
    ADC_CHANNEL_4 = 4,  // PA4
    ADC_CHANNEL_5 = 5,  // PA5
    ADC_CHANNEL_6 = 6,  // PA6
    ADC_CHANNEL_7 = 7,  // PA7
} ADC_Channel_TypeDef;

// ADC Sample Time Definitions
typedef enum {
    ADC_SAMPLETIME_1_5_CYCLES   = 0b000,
    ADC_SAMPLETIME_7_5_CYCLES   = 0b001,
    ADC_SAMPLETIME_13_5_CYCLES  = 0b010,
    ADC_SAMPLETIME_28_5_CYCLES  = 0b011,
    ADC_SAMPLETIME_41_5_CYCLES  = 0b100,
    ADC_SAMPLETIME_55_5_CYCLES  = 0b101,
    ADC_SAMPLETIME_71_5_CYCLES  = 0b110,
    ADC_SAMPLETIME_239_5_CYCLES = 0b111
} ADC_SampleTime_TypeDef;
/**************************************************************************************************/
class ADC_Channel {
public:
    ADC_Channel() = default;
    ADC_Channel(ADC_Type* ADC_, ADC_SampleTime_TypeDef SampleTime_,GPIO* Port_,ADC_Channel_TypeDef channel_, uint8_t rank);
    uint16_t readChannel();
private:
    ADC_Channel_TypeDef channel;
    ADC_Type* _adc = nullptr;   
    ADC_SampleTime_TypeDef sample_time;
    GPIO*     Port = nullptr;
};
/**************************************************************************************************/
// ADC Class Definition
class ADC_Driver {
public:
    ADC_Driver(GPIO* PORT,ADC_Type* adc_instance);
    ADC_Channel Creat_Channel(ADC_Channel_TypeDef channel_,ADC_SampleTime_TypeDef SampleTime_ = ADC_SAMPLETIME_1_5_CYCLES);

private:
    ADC_Type* _adc;     
    GPIO*     Port;
    void pre_init();
    void calibrate();
    void init(bool continuous_mode, bool scan_mode, bool data_align_right);
};

#endif
