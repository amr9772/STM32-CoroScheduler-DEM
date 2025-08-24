#include "my_stm32f10_uart_lib.h"


UART::UART(UART_ComType comtype_, UART_BaudrateType baudrate_, GPIO* PORT)
:comtype(comtype_),baudrate(baudrate_),UART_GPIO(PORT)
{
    // Enable GPIOA clock <----- Enabled by GPIO PORT 
    // Enable USART2 clock
    RCC_USART2_CLK_ENABLE(); // Enable USART2 clock (bit 17 for USART2)

    USART2->CR1 = UART2_CR1_DIS; //Disable UART temporarily 
    

    switch (comtype) {
        case TX_ONLY:{
            // Configure PA2 as Alternate Function Output (TX)
            // F1 uses CRL and CRH registers for pin configuration
            // PA2 is in CRL (pins 0-7), so we set CNF[1] and MODE[1]
            // CNF[1] bits 9:8, MODE[1] bits 8:9
            // Bits for PA2 in CRL are 11:8. CNF2 = bits 11:10, MODE2 = bits 9:8
            // Alternate Function Push-Pull: CNF=10, MODE=10
            UART_GPIO->CreatePin(2,GPIO_MODE_AF_PP_GENERIC,GPIO_NOPULL,GPIO_SPEED_LOW,1);
            USART2->CR1 = CR1_CNF1_TX;
            break;
            }
        case RX_ONLY:{
            // Configure PA3 as Floating Input (RX)
            // CNF3 = bits 15:14, MODE3 = bits 13:12 in CRL
            // Floating Input: CNF=01, MODE=00
            UART_GPIO->CreatePin(3,GPIO_MODE_INPUT_FLOATING,GPIO_NOPULL,GPIO_SPEED_LOW,1);
            USART2->CR1 = CR1_CNF1_RX;     
            break;
        }
        case RX_AND_TX:{
            // Configure PA2 (TX) and PA3 (RX)
            UART_GPIO->CreatePin(2,GPIO_MODE_AF_PP_GENERIC,GPIO_NOPULL,GPIO_SPEED_LOW,1);
            UART_GPIO->CreatePin(3,GPIO_MODE_INPUT_FLOATING,GPIO_NOPULL,GPIO_SPEED_LOW,1);
            USART2->CR1 |= CR1_CNF1_TX | CR1_CNF1_RX;            
            break;
        }
        default:
            break;
    }

    switch (baudrate) {
        case _115200:
            USART2->BRR = BRR_CNF1_115200;
            break;

        case _9600:
            USART2->BRR = BRR_CNF1_9600;
            break;

        default:
            break;
    }

    USART2->CR2 = CR2_CNF1;
    USART2->CR3 = CR3_CNF1;
    USART2->CR1 |= UART2_CR1_EN1;
}

char UART::Read(void) {
    // Check the RXNE (Receive data register not empty) flag in the SR
    while (!(USART2->SR & (1UL << 5))) {}
    return (char)USART2->DR;
}

void UART::Write(char ch) {
    // Check the TXE (Transmit data register empty) flag in the SR
    while (!(USART2->SR & (1 << 7))) {}
    USART2->DR = (ch & 0xFF);
}

void UART::Write(int dec) {
    // Check the TXE (Transmit data register empty) flag in the SR
    char buffer[10];
    char* number = int_to_string(dec,buffer);
    Write_String(number);
}

void UART::Write_String(const char* str) {
    while (*str != '\0') {
        Write(*str);
        str++;
    }
}

void UART::Read_String(char* buffer, int max_len) {
    int i = 0;
    char received_char;
    while (i < max_len - 1) {
        received_char = Read();
        if (received_char == '\n' || received_char == '\r') {
            break;
        }
        buffer[i] = received_char;
        i++;
    }
    buffer[i] = '\0'; // Null-terminate the string
}

void UART::RCC_USART2_CLK_ENABLE(void){RCC->APB1ENR |= (1 << 17);}