#ifndef		__MY_STM32F10_GPIO_LIB_H__
#define		__MY_STM32F10_GPIO_LIB_H__

#include  "my_stm32f10_gpio_driver.h"
#include  <utility>



// GPIO Modes for Init struct (F1 CNF and MODE bits) (kept as is)
#define GPIO_MODE_INPUT             0x00U  // Input mode (MODE=00)
#define GPIO_MODE_INPUT_ANALOG      0x00U  // Analog mode (CNF=00, MODE=00)
#define GPIO_MODE_INPUT_FLOATING    0x04U  // Floating input (CNF=01, MODE=00)
#define GPIO_MODE_INPUT_PUPD        0x08U  // Input with pull-up/pull-down (CNF=10, MODE=00)

// Output modes are defined by combining CNF and Speed
#define GPIO_MODE_OUTPUT_PP_GENERIC 0x00U // General Purpose Output Push-Pull (CNF=00)
#define GPIO_MODE_OUTPUT_OD_GENERIC 0x01U // General Purpose Output Open-Drain (CNF=01)
#define GPIO_MODE_AF_PP_GENERIC     0x02U // Alternate Function Push-Pull (CNF=10)
#define GPIO_MODE_AF_OD_GENERIC     0x03U // Alternate Function Open-Drain (CNF=11)

// Speed definitions for output modes
#define GPIO_SPEED_LOW              0x02U  // 2 MHz (MODE=10)
#define GPIO_SPEED_MEDIUM           0x01U  // 10 MHz (MODE=01)
#define GPIO_SPEED_HIGH             0x03U  // 50 MHz (MODE=11)

// Pull-up/Pull-down definitions for GPIO_InitTypeDef.Pull
#define GPIO_NOPULL                 0x00U
#define GPIO_PULLUP                 0x01U
#define GPIO_PULLDOWN               0x02U

// GPIO Pin State Enumeration
typedef enum
{
    GPIO_PIN_RESET = 0,
    GPIO_PIN_SET
}GPIO_PinState;

// PORT_NameType enum
typedef enum{
	PORTA = 1,
	PORTB,
	PORTC,
	PORTD,
}PORT_NameType;
/*********************************GPIO PIN CLASS**************************************/
// GPIO_PIN class - self-contained for configuration and control
class GPIO_PIN { //Composition relationship with the GPIO class
  public:
      // Constructor: Now takes the port base address and configures the pin directly.
      GPIO_PIN() : GPIOx(nullptr), Number(0), Pin(0), Mode(0), Pull(0), Speed(0), Alternate(0) {};
      GPIO_PIN(volatile GPIO_TypeDef* PORT_Base, uint8_t Pin_Number, uint32_t Pin_Mode,
              uint32_t Pin_Pull, uint32_t Pin_Speed, uint32_t Pin_Alternate);
      GPIO_PIN(const GPIO_PIN&) = delete;
      GPIO_PIN& operator=(const GPIO_PIN&) = delete;
      GPIO_PIN& operator=(GPIO_PIN&& other) noexcept;
      // Public methods for pin control
      GPIO_PinState ReadPin();
      void WritePin(GPIO_PinState PinState);
      void TogglePin();
      // Getters 
      uint8_t Get_pin_number() const;
  private:
      volatile GPIO_TypeDef* GPIOx; // Pointer to the actual hardware registers for this pin's port
      uint8_t Number;               // The pin number (0-15)
      uint32_t Pin;                 // The bitmask for this pin (e.g., 1U << 14)
      uint32_t Mode;                // Pin Mode
      uint32_t Pull;                // Pull-up/Pull-down setting
      uint32_t Speed;               // Output Speed
      uint32_t Alternate;           // Alternate Function
};


/*********************************GPIO CLASS******************************************/
// GPIO class - only responsible for enabling the port clock and providing the base address
class GPIO{
  public:
      // Constructor: Enables port clock and sets GPIOx pointer
      GPIO(PORT_NameType GPIO_PortName);
      // CreatePin method
      // and its role is primarily to provide the necessary parameters to GPIO_PIN's constructor.
      GPIO_PIN& CreatePin(uint8_t Pin_Number, uint32_t Pin_Mode, uint32_t Pin_Pull,
                         uint32_t Pin_Speed, uint32_t Pin_Alternate=0);
      // Operators Overloads
      GPIO_PIN& operator [] (uint8_t index);
      // Getters 
      uint8_t Get_num_of_used_pins() const;
      volatile  GPIO_TypeDef* get_GPIOx() const;  
  protected:
      void RCC_GPIOA_CLK_ENABLE(void);
      void RCC_GPIOB_CLK_ENABLE(void);
      void RCC_GPIOC_CLK_ENABLE(void);
      void RCC_GPIOD_CLK_ENABLE(void);
  private:
      volatile  GPIO_TypeDef* GPIOx;
      GPIO_PIN  Pins[16];  // Composition
      uint8_t   Pins_Used_count;
       
};



#endif
