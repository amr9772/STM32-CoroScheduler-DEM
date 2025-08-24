Project Name
STM32-CoroScheduler: A Lightweight, Cooperative Multitasking Framework for ARM Cortex-M

Project Description
This repository contains a simple, bare-metal cooperative multitasking scheduler designed for the STM32F103 series microcontroller, leveraging C++20 coroutines. The project demonstrates how to build a flexible and non-blocking embedded application without the complexity of a full-fledged Real-Time Operating System (RTOS).

The core of the project is a coroutine-based scheduler that manages multiple tasks, including an ADC-based temperature sensor reader, LED blinking routines, and a push-button event handler. It features a custom Diagnostic Event Manager (DEM) for robust fault handling and debouncing, providing a foundation for reliable embedded systems. This framework is ideal for resource-constrained environments where responsiveness and efficient CPU utilization are critical.

Key Features
Cooperative Multitasking: Manages multiple tasks using co_await and co_yield for non-blocking execution.

Diagnostic Event Manager (DEM): Implements a robust debouncing mechanism to handle and log system events and faults.

Peripheral Drivers: Includes low-level drivers for GPIO, UART, ADC, and SYSTICK timer.

Hardware Abstraction: The code is structured with a clean object-oriented approach, abstracting hardware details into C++ classes.

STM32F103C8 Support: Written for the STM32F103 "Blue Pill" board but easily portable to other ARM Cortex-M devices.

Directory Structure
Core: Contains the core scheduler and coroutine definitions.

VTINYOS.h: The cooperative scheduler.

CoroType.h: The C++20 coroutine type wrapper.

Drivers: Peripheral drivers and hardware abstraction classes.

my_stm32f10_gpio_lib.h/cpp: GPIO pin and port management.

my_stm32f10_uart_lib.h/cpp: UART communication driver.

my_stm32f10_adc_lib.h/cpp: ADC driver and channel management.

my_stm32f10_systick_lib.h/cpp: SYSTICK timer-based delay.

Tasks: Application-level tasks implemented as coroutines.

task.h/cpp: Coroutine tasks for blinking LEDs, reading temperature, etc.

Diag_mgr.h/cpp: Diagnostic manager and event-related tasks.

Utilities: Helper classes and modules.

DEM.h/cpp: The Diagnostic Event Manager and DEM_event classes.

uart_stream.h/cpp: A custom cout-like stream for UART.
