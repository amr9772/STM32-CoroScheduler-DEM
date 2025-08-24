#ifndef __MY_STM32F10_UART_LIB
#define __MY_STM32F10_UART_LIB

#include  "my_stm32f10_uart_driver.h"
#include  "my_stm32f10_gpio_lib.h"
#include  "utils.h"

#define CR1_CNF1_RX 		  0x0004 //Enable Rx, 8-bit data
#define BRR_CNF1_9600 		0x09C4 //9600 baud @ 24 Mhz 
#define BRR_CNF1_115200 	0x0D0  //115200 baud @ 24 Mhz 
#define CR1_CNF1_TX 		  0x0008 //Enable Tx, 8-bit data
#define CR2_CNF1 		      0x0000 //1 stop bit
#define CR3_CNF1 		      0x0000 //no flow control
#define UART2_CR1_EN1 		0x2000 //Enable UART
#define UART2_CR1_DIS 		0x0000 //Disable UART


typedef enum{
 RX_ONLY =0,
 TX_ONLY,
 RX_AND_TX
}UART_ComType;



typedef enum{
 _115200 = 0,
 _9600

}UART_BaudrateType;

class UART{
  public:
    UART(UART_ComType comtype_, UART_BaudrateType baudrate_, GPIO* PORT);
    void Write(char ch);
    void Write(int dec);
    char Read(void);
    // New string-based methods
    void Write_String(const char* str);
    void Read_String(char* buffer, int max_len);
  private:
    void RCC_USART2_CLK_ENABLE(void);
    UART_ComType      comtype;
    UART_BaudrateType baudrate;
    GPIO*             UART_GPIO;
    

};


#endif