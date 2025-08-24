#include "utils.h"

char* int_to_string(int32_t num, char* buffer) {
    int i = 0;
    int length = 0;
    bool is_negative = false;

    // Handle 0 separately
    if (num == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return buffer;
    }

    // Handle negative numbers
    if (num < 0) {
        is_negative = true;
        num = -num;
    }
    
    // Calculate the length of the number
    int temp = num;
    while (temp != 0) {
        temp /= 10;
        length++;
    }

    // Add room for the negative sign and null terminator
    if (is_negative) {
        length++;
    }
    
    // Set the null terminator at the end of the buffer
    buffer[length] = '\0';
    i = length - 1;

    // Process digits from right to left
    while (num != 0) {
        int rem = num % 10;
        buffer[i--] = rem + '0';
        num = num / 10;
    }

    // Add the negative sign at the beginning
    if (is_negative) {
        buffer[0] = '-';
    }

    return buffer;
}