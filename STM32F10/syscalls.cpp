#include "my_stm32f10_uart_lib.h"
#include <unistd.h> // For ssize_t
#include <errno.h>

// Forward declaration of the UART object
extern UART UART2;

// Must be C-linkage to match the standard library function signature
extern "C" {
    ssize_t _write(int file, const char* ptr, int len) {
        // You can check the file descriptor if needed,
        // but for a single UART, you can write all data regardless.
        if (file == STDOUT_FILENO || file == STDERR_FILENO) {
            // Write the string using your UART library's function
            for (int i = 0; i < len; ++i) {
                UART2.Write(ptr[i]);
            }
            return len;
        }
        return -1; // Indicate error
    }
}



extern "C" {
    ssize_t _read(int file, char* ptr, int len) {
        if (file == STDIN_FILENO) {
            // Read a single character from the UART
            char ch = UART2.Read();
            
            // Put the character into the buffer
            *ptr = ch;
            
            return 1; // Return the number of characters read
        }
        return -1; // Indicate error
    }
}

extern "C" char __HeapLimit;
extern "C" char end;

static char *heap_end = 0;

extern "C" {
    // ... existing _write and _read functions ...

    // Function to handle dynamic memory allocation for the C library
    void* _sbrk(int incr) {
        char *prev_heap_end;
        char *limit = &__HeapLimit;

        if (heap_end == 0) {
            heap_end = &end;
        }
        
        prev_heap_end = heap_end;

        if (heap_end + incr > limit) {
            errno = ENOMEM;
            return (void*)-1;
        }

        heap_end += incr;
        return (void*)prev_heap_end;
    }
  }