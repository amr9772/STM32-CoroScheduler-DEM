#ifndef __UART_STREAM_H__
#define __UART_STREAM_H__

#include "my_stm32f10_uart_lib.h"
#include "utils.h"
#include <concepts>
#include <type_traits>

template <typename T>
concept  printable_number = std::is_integral_v<T>;


class UART_Stream {
private:
    UART& m_uart;

public:
    UART_Stream(UART& uart);

    // Stream insertion operator overloads
    UART_Stream& operator<<(const char* str);
    UART_Stream& operator<<(char ch);
    template<typename T>
    requires printable_number<T>
    UART_Stream& operator<<(T num);
    //UART_Stream& operator<<(uint16_t num);

    UART_Stream& operator>>(char& ch);
    UART_Stream& operator>>(char* str);
};

template<typename T>
requires printable_number<T>
UART_Stream& UART_Stream::operator<<(T num) {
    char buffer[12];
    m_uart.Write_String(int_to_string(num, buffer));
    return *this;
}


// Global instance of the stream declared here.
extern UART_Stream uart_cout;
// Define the input stream instance
extern UART_Stream uart_cin;

#define uart_endl "\r\n"

#endif