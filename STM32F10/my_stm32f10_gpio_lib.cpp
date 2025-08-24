#include "my_stm32f10_gpio_lib.h"
#include <cassert> 
//#include <utility>
// GPIO Class Constructor: Enables port clock and assigns GPIOx pointer
GPIO::GPIO(PORT_NameType GPIO_PortName) : Pins_Used_count(0){
    switch(GPIO_PortName){
        case PORTA: GPIOx = GPIOA; RCC_GPIOA_CLK_ENABLE(); break;
        case PORTB: GPIOx = GPIOB; RCC_GPIOB_CLK_ENABLE(); break;
        case PORTC: GPIOx = GPIOC; RCC_GPIOC_CLK_ENABLE(); break;
        case PORTD: GPIOx = GPIOD; RCC_GPIOD_CLK_ENABLE(); break;
        default: GPIOx = nullptr; // Handle invalid port name
    }
}
// RCC Clock Enable Function Definitions 
void GPIO::RCC_GPIOA_CLK_ENABLE(void) { RCC->APB2ENR |= GPIOA_EN_BIT; }
void GPIO::RCC_GPIOB_CLK_ENABLE(void) { RCC->APB2ENR |= GPIOB_EN_BIT; }
void GPIO::RCC_GPIOC_CLK_ENABLE(void) { RCC->APB2ENR |= GPIOC_EN_BIT; }
void GPIO::RCC_GPIOD_CLK_ENABLE(void) { RCC->APB2ENR |= GPIOD_EN_BIT; }

// GPIO Class CreatePin Method: Now constructs and returns a GPIO_PIN object by value.
// The actual hardware configuration is moved into the GPIO_PIN constructor.

GPIO_PIN& GPIO::CreatePin(uint8_t Pin_Number, uint32_t Pin_Mode, uint32_t Pin_Pull,
                         uint32_t Pin_Speed, uint32_t Pin_Alternate) {
    // Construct and return refrence to GPIO_PIN object
    GPIO_PIN new_PIN(GPIOx, Pin_Number, Pin_Mode, Pin_Pull, Pin_Speed, Pin_Alternate);
    Pins[Pins_Used_count] = std::move(new_PIN);
    Pins_Used_count ++;
    return Pins[Pins_Used_count-1];
}

GPIO_PIN& GPIO::operator [] (uint8_t index){
    assert(index >= 0 && index < 16);
    return Pins[index];
}

uint8_t GPIO::Get_num_of_used_pins() const{
  return Pins_Used_count;
}
volatile  GPIO_TypeDef* GPIO::get_GPIOx() const{
  return GPIOx;
}

/***********************************************************************************************************/


// GPIO_PIN Class Constructor: Now performs the actual hardware configuration
GPIO_PIN::GPIO_PIN(volatile GPIO_TypeDef* PORT_Base, uint8_t Pin_Number, uint32_t Pin_Mode, uint32_t Pin_Pull,
                   uint32_t Pin_Speed, uint32_t Pin_Alternate)
    : GPIOx(PORT_Base), Number(Pin_Number), Pin(1U << Pin_Number),
      Mode(Pin_Mode), Pull(Pin_Pull), Speed(Pin_Speed), Alternate(Pin_Alternate) {

    if (GPIOx == nullptr || Pin_Number >= 16) {
        // PIN --> Invalid
        return; 
    }

    uint32_t temp_config = 0x0U;
    uint32_t config_reg_offset;

    // --- Hardware configuration logic (GPIO_PIN constructor) ---
    if (Number < 8U) { // Pins 0-7 use CRL register
        config_reg_offset = Number * 4U;
        GPIOx->CRL &= ~((uint32_t)(0xFU) << config_reg_offset); // Clear existing bits

        if (Mode == GPIO_MODE_OUTPUT_PP_GENERIC ||
            Mode == GPIO_MODE_OUTPUT_OD_GENERIC ||
            Mode == GPIO_MODE_AF_PP_GENERIC ||
            Mode == GPIO_MODE_AF_OD_GENERIC) {
            temp_config = (Mode << 2U) | Speed; // Combine CNF (mode) and MODE (speed)
        } else { // Input modes
            temp_config = Mode;
            if (Mode == GPIO_MODE_INPUT_PUPD) {
                if (Pull == GPIO_PULLUP) {
                    GPIOx->ODR |= Pin; // Set ODR bit for pull-up
                } else if (Pull == GPIO_PULLDOWN) {
                    GPIOx->ODR &= ~Pin; // Clear ODR bit for pull-down
                }
            }
        }
        GPIOx->CRL |= (temp_config << config_reg_offset); // Apply new config
    } else { // Pins 8-15 use CRH register
        config_reg_offset = (Number - 8U) * 4U;
        GPIOx->CRH &= ~((uint32_t)(0xFU) << config_reg_offset); // Clear existing bits

        if (Mode == GPIO_MODE_OUTPUT_PP_GENERIC ||
            Mode == GPIO_MODE_OUTPUT_OD_GENERIC ||
            Mode == GPIO_MODE_AF_PP_GENERIC ||
            Mode == GPIO_MODE_AF_OD_GENERIC) {
            temp_config = (Mode << 2U) | Speed; // Combine CNF (mode) and MODE (speed)
        } else { // Input modes
            temp_config = Mode;
            if (Mode == GPIO_MODE_INPUT_PUPD) {
                if (Pull == GPIO_PULLUP) {
                    GPIOx->ODR |= Pin; // Set ODR bit for pull-up
                } else if (Pull == GPIO_PULLDOWN) {
                    GPIOx->ODR &= ~Pin; // Clear ODR bit for pull-down
                }
            }
        }
        GPIOx->CRH |= (temp_config << config_reg_offset); // Apply new config
    }
    // --- End of hardware configuration ---
 
}

GPIO_PIN& GPIO_PIN::operator=(GPIO_PIN&& other) noexcept {
    if (this != &other) {
        GPIOx     = other.GPIOx;
        Number    = other.Number;
        Pin       = other.Pin;
        Mode      = other.Mode;
        Pull      = other.Pull;
        Speed     = other.Speed;
        Alternate = other.Alternate;

        // Leave the moved-from object in a valid but empty state
        other.GPIOx = nullptr;
        other.Number = 0;
        other.Pin = 0;
    }
    return *this;
}

// GPIO_PIN methods (using the now-correctly-set volatile GPIOx pointer from constructor)
GPIO_PinState GPIO_PIN::ReadPin() {
    if (GPIOx == nullptr) return GPIO_PIN_RESET; // Guard against uninitialized pointer
    return (GPIOx->IDR & Pin) ? GPIO_PIN_SET : GPIO_PIN_RESET;
}

void GPIO_PIN::WritePin(GPIO_PinState PinState) {
    if (GPIOx == nullptr) return; // Guard against uninitialized pointer
    if (PinState != GPIO_PIN_RESET) {
        GPIOx->BSRR = Pin; // Set pin atomically
    } else {
        GPIOx->BSRR = (uint32_t)Pin << 16; // Reset pin atomically
    }
}

void GPIO_PIN::TogglePin() {
    if (GPIOx == nullptr) return; // Guard against uninitialized pointer
    GPIOx->ODR ^= Pin; 
}

uint8_t GPIO_PIN::Get_pin_number() const{
  return Number;
}