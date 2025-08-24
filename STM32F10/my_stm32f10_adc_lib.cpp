// my_stm32f10_adc_lib.cpp

#include "my_stm32f10_adc_lib.h" // Assuming the header file is named this way

// Constructor
// The constructor now calls pre_init() for initial setup
// and then init() and configureChannel() for a default setup of ADC_CHANNEL_0.
// This provides a ready-to-use ADC instance for a common use case.
ADC_Driver::ADC_Driver(GPIO* PORT,ADC_Type* adc_instance)
    : _adc(adc_instance),Port(PORT){
    
    // Perform essential, one-time power-up and clock configuration
    pre_init();
    
    // Perform general ADC initialization (e.g., single conversion, right alignment)
    // These are common default settings, but can be changed later with init()
    init(false, false, true); 
    // Configure a default channel (PA0, rank 1, default sample time)
    // This makes the ADC immediately usable for reading PA0 after construction
    //configureChannel(Channel, 1, ADC_SAMPLETIME_28_5_CYCLES); 
    // Calibrate the ADC for optimal accuracy
    calibrate();
}

// pre_init() function: Handles essential, one-time ADC clock and power-up configuration.
// This function is typically called once to enable the ADC peripheral clock,
// reset it, set the prescaler, and enable the ADC module itself.
void ADC_Driver::pre_init(){
    // Enable clock for ADC1 (assuming _adc points to ADC1)
    // Bit 9 in APB2ENR controls the ADC1 clock.
    RCC->APB2ENR |= (1UL << 9); 

    // Reset ADC1 peripheral
    // Toggling the reset bit ensures the ADC is in a known default state.
    RCC->APB2RSTR |= (1UL << 9);  // Set ADC1RST bit
    RCC->APB2RSTR &= ~(1UL << 9); // Clear ADC1RST bit

    // Configure ADC clock prescaler (PCLK2 / N)
    // The ADC clock must not exceed 14 MHz for STM32F10x.
    // Assuming PCLK2 is 48MHz (as per your current Proteus setup),
    // a prescaler of 4 (48/4 = 12MHz) is suitable.
    // 0b01 in ADCPRE bits (15:14) sets PCLK2/4.
    RCC->CFGR &= ~(0x3UL << 14); // Clear ADCPRE bits
    RCC->CFGR |= (0x1UL << 14);  // Set ADCPRE to PCLK2/4 (for 48MHz -> 12MHz ADC clock)

    // Enable ADC (ADON bit in CR2)
    // Setting ADON to 1 enables the ADC. The first time ADON is set,
    // the ADC powers up. Subsequent sets start a conversion if not in continuous mode.
    _adc->CR2 |= (1UL << 0); 
}


// init() function: Configures general ADC operating modes.
// This can be called after construction to change modes like continuous, scan, or alignment.
// @param continuous_mode: true for continuous conversion, false for single-shot.
// @param scan_mode: true to enable scan mode for multiple channels in sequence.
// @param data_align_right: true for right-aligned 12-bit data, false for left-aligned.
void ADC_Driver::init(bool continuous_mode, bool scan_mode, bool data_align_right) {
    // Continuous conversion mode (CONT bit in CR2)
    if (continuous_mode) {
        _adc->CR2 |= (1UL << 1); 
    } else {
        _adc->CR2 &= ~(1UL << 1); 
    }

    // Scan mode (SCAN bit in CR1)
    if (scan_mode) {
        _adc->CR1 |= (1UL << 8); 
    } else {
        _adc->CR1 &= ~(1UL << 8); 
    }

    // Data alignment (ALIGN bit in CR2)
    // 0: Right alignment (default, 12-bit value in bits 11:0 of DR)
    // 1: Left alignment (12-bit value in bits 15:4 of DR)
    if (data_align_right) {
        _adc->CR2 &= ~(1UL << 11); 
    } else {
        _adc->CR2 |= (1UL << 11); 
    }

    // Ensure ADC is ON after configuration (important after any resets or power-downs)
    _adc->CR2 |= (1UL << 0); 
}

// calibrate() function: Performs ADC self-calibration for improved accuracy.
// Calibration should be run after ADC power-up and before any conversions.
void ADC_Driver::calibrate() {
    // Reset calibration registers
    // Setting RSTCAL bit to 1 initiates a reset of calibration values.
    _adc->CR2 |= (1UL << 3);  
    // Wait until RSTCAL bit is cleared by hardware, indicating reset is complete.
    while ((_adc->CR2 & (1UL << 3)) != 0); 

    // Start calibration
    // Setting CAL bit to 1 starts the self-calibration process.
    _adc->CR2 |= (1UL << 2);  
    // Wait until CAL bit is cleared by hardware, indicating calibration is complete.
    while ((_adc->CR2 & (1UL << 2)) != 0); 
}


ADC_Channel ADC_Driver::Creat_Channel(ADC_Channel_TypeDef channel_,ADC_SampleTime_TypeDef SampleTime_){
  return ADC_Channel(_adc,SampleTime_,Port,channel_,1);
}
/******************************************************************************************************/

ADC_Channel::ADC_Channel(ADC_Type* ADC_, ADC_SampleTime_TypeDef SampleTime_,GPIO* Port_,ADC_Channel_TypeDef channel_, uint8_t rank)
  : _adc(ADC_),sample_time(SampleTime_),Port(Port_),channel(channel_)
{
    // Configure the corresponding GPIO pin for analog input
      Port->CreatePin(channel, GPIO_MODE_INPUT_ANALOG, GPIO_NOPULL, 0, 0);

      _adc->SMPR2 &= ~((0x7UL) << (channel * 3)); // Clear existing sample time bits
      _adc->SMPR2 |= ((uint32_t)sample_time << (channel * 3)); // Set new sample time
    
    // Set rank in the regular sequence
    // The regular sequence registers (SQR1, SQR2, SQR3) define the order of conversions.
    // Each channel in the sequence occupies 5 bits.
    // Corrected: sqr_reg must be volatile uint32_t*
    volatile uint32_t* sqr_reg = nullptr; 
    uint8_t bit_offset = 0;

    if (rank >= 1 && rank <= 6) { // SQ1 to SQ6 are in SQR3
        sqr_reg = &_adc->SQR3;
        bit_offset = (rank - 1) * 5;
    } else if (rank >= 7 && rank <= 12) { // SQ7 to SQ12 are in SQR2
        sqr_reg = &_adc->SQR2;
        bit_offset = (rank - 7) * 5;
    } else if (rank >= 13 && rank <= 16) { // SQ13 to SQ16 are in SQR1
        sqr_reg = &_adc->SQR1;
        bit_offset = (rank - 13) * 5;
    } else {
        // Handle error: Invalid rank (e.g., via assert or return an error code)
        // For simplicity, we just return here.
        return;
    }

    *sqr_reg &= ~((0x1FUL) << bit_offset); // Clear existing channel bits for this rank
    *sqr_reg |= ((uint32_t)channel << bit_offset); // Set new channel for the rank

    // Update L (number of conversions in regular sequence) in SQR1
    // L bits (23:20) of SQR1 define (number of conversions - 1). Max 16 conversions (L=15).
    uint8_t current_l_value = (uint8_t)((_adc->SQR1 >> 20) & 0xF); // Read current L value
    if (rank > (current_l_value + 1)) { // If the new rank extends the sequence length
        _adc->SQR1 &= ~(0xFUL << 20);       // Clear L bits
        _adc->SQR1 |= ((uint32_t)(rank - 1) << 20); // Set new L based on the highest rank
    }

}

uint16_t ADC_Channel::readChannel(){
  // IMPORTANT: This function assumes non-continuous mode for single reads.
    // If the ADC is in continuous mode, this approach might conflict.
    // Ensure the ADC is in single conversion mode (init(false, ...)) for this to work robustly.

    // Set sequence length to 1 conversion (L = 0, bits 23:20 of SQR1)
    // This ensures only the current channel is converted, not a longer sequence.
    _adc->SQR1 &= ~(0xFUL << 20); 

    // Set the first (and only) conversion in the regular sequence to the desired channel
    _adc->SQR3 &= ~(0x1FUL << 0);    // Clear SQR3_SQ1 (first conversion slot)
    _adc->SQR3 |= ((uint32_t)channel << 0); // Set desired channel as the first conversion

    // Start regular conversion by setting the SWSTART bit (Software Start)
    _adc->CR2 |= (1UL << 22); 

    // Wait for EOC (End Of Conversion) flag
    // EOC bit (Bit 1) in SR register is set when a regular channel conversion is complete.
    while (!(_adc->SR & (1UL << 1))); 

    // Read the conversion result from the Data Register (DR)
    // Reading DR automatically clears the EOC flag for regular conversions.
    uint16_t adc_value = (uint16_t)(_adc->DR & 0xFFFF);

    return adc_value;
}