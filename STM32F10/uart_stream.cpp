#include "uart_stream.h"
#include "my_stm32f10_uart_lib.h" // Include this if needed


UART_Stream::UART_Stream(UART& uart) : m_uart(uart) {}

UART_Stream& UART_Stream::operator<<(const char* str) {
    m_uart.Write_String(str);
    return *this;
}

UART_Stream& UART_Stream::operator<<(char ch) {
    m_uart.Write(ch);
    return *this;
}



/*
UART_Stream& UART_Stream::operator<<(uint16_t num) { 
    char buffer[12];
    m_uart.Write_String(int_to_string(num, buffer));
    return *this;
}
*/
// Stream extraction overloads for input
UART_Stream& UART_Stream::operator>>(char& ch) {
    ch = m_uart.Read();
    return *this;
}

UART_Stream& UART_Stream::operator>>(char* str) {
    // You'll need to read a string from UART.
    // The max_len for the buffer should be handled by the caller.
    // Assuming a max buffer size of 64 characters for this example.
    m_uart.Read_String(str, 64);
    return *this;
}