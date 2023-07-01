/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus/)
 * Copyright (c) 2022-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

// ----------------------------------------------------------------------------

// clang-format off

// ----------------------------------------------------------------------------

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include "cmsis_device.h"

#include <cmsis-plus/cortexm/exception-handlers.h>

// ----------------------------------------------------------------------------

void __attribute__((weak))
Default_Handler(void);

// ----------------------------------------------------------------------------

extern uint32_t __stack;

typedef void
(*handler_ptr_t)(void);

// ----------------------------------------------------------------------------

void __attribute__ ((weak, alias("Default_Handler")))
TIMER_IRQ_0_Handler(void);

void __attribute__ ((weak, alias("Default_Handler")))
TIMER_IRQ_1_Handler(void);

void __attribute__ ((weak, alias("Default_Handler")))
TIMER_IRQ_2_Handler(void);

void __attribute__ ((weak, alias("Default_Handler")))
TIMER_IRQ_3_Handler(void);

void __attribute__ ((weak, alias("Default_Handler")))
PWM_IRQ_WRAP_Handler(void);

void __attribute__ ((weak, alias("Default_Handler")))
USBCTRL_IRQ_Handler(void);

void __attribute__ ((weak, alias("Default_Handler")))
XIP_IRQ_Handler(void);

void __attribute__ ((weak, alias("Default_Handler")))
PIO0_IRQ_0_Handler(void);

void __attribute__ ((weak, alias("Default_Handler")))
PIO0_IRQ_1_Handler(void);

void __attribute__ ((weak, alias("Default_Handler")))
PIO1_IRQ_0_Handler(void);

void __attribute__ ((weak, alias("Default_Handler")))
PIO1_IRQ_1_Handler(void);

void __attribute__ ((weak, alias("Default_Handler")))
DMA_IRQ_0_Handler(void);

void __attribute__ ((weak, alias("Default_Handler")))
DMA_IRQ_1_Handler(void);

void __attribute__ ((weak, alias("Default_Handler")))
IO_IRQ_BANK0_Handler(void);

void __attribute__ ((weak, alias("Default_Handler")))
IO_IRQ_QSPI_Handler(void);

void __attribute__ ((weak, alias("Default_Handler")))
SIO_IRQ_PROC0_Handler(void);

void __attribute__ ((weak, alias("Default_Handler")))
SIO_IRQ_PROC1_Handler(void);

void __attribute__ ((weak, alias("Default_Handler")))
CLOCKS_IRQ_Handler(void);

void __attribute__ ((weak, alias("Default_Handler")))
SPI0_IRQ_Handler(void);

void __attribute__ ((weak, alias("Default_Handler")))
SPI1_IRQ_Handler(void);

void __attribute__ ((weak, alias("Default_Handler")))
UART0_IRQ_Handler(void);

void __attribute__ ((weak, alias("Default_Handler")))
UART1_IRQ_Handler(void);

void __attribute__ ((weak, alias("Default_Handler")))
ADC_IRQ_FIFO_Handler(void);

void __attribute__ ((weak, alias("Default_Handler")))
I2C0_IRQ_Handler(void);

void __attribute__ ((weak, alias("Default_Handler")))
I2C1_IRQ_Handler(void);

void __attribute__ ((weak, alias("Default_Handler")))
RTC_IRQ_Handler(void);

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Wpedantic"

/**
 * The table of interrupt handlers. It has an explicit section name
 * and relies on the linker script to place it at the correct location
 * in memory, usually at the very beginning of the flash bank.
 */
__attribute__ ((section(".isr_vector"),used))
handler_ptr_t _interrupt_vectors[] =
  {
    // Cortex-M Core Handlers
    (handler_ptr_t) &__stack,          // MSP
    Reset_Handler,                     // The reset handler

    NMI_Handler,                       // The NMI handler
    HardFault_Handler,                 // The hard fault handler

#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
    MemManage_Handler,                 // The MPU fault handler
    BusFault_Handler,                  // The bus fault handler
    UsageFault_Handler,                // The usage fault handler
#else
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
#endif
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    SVC_Handler,                       // SVCall handler
#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
    DebugMon_Handler,                  // Debug monitor handler
#else
    0,                                 // Reserved
#endif
    0,                                 // Reserved
    PendSV_Handler,                    // The PendSV handler
    SysTick_Handler,                   // The SysTick handler

    // ------------------------------------------------------------------------
    // External Interrupts
    TIMER_IRQ_0_Handler, // 0
    TIMER_IRQ_1_Handler,
    TIMER_IRQ_2_Handler,
    TIMER_IRQ_3_Handler,
    PWM_IRQ_WRAP_Handler,
    USBCTRL_IRQ_Handler,
    XIP_IRQ_Handler,
    PIO0_IRQ_0_Handler,
    PIO0_IRQ_1_Handler,
    PIO1_IRQ_0_Handler,
    PIO1_IRQ_1_Handler,
    DMA_IRQ_0_Handler,
    DMA_IRQ_1_Handler,
    IO_IRQ_BANK0_Handler,
    IO_IRQ_QSPI_Handler,
    SIO_IRQ_PROC0_Handler,
    SIO_IRQ_PROC1_Handler,
    CLOCKS_IRQ_Handler,
    SPI0_IRQ_Handler,
    SPI1_IRQ_Handler,
    UART0_IRQ_Handler,
    UART1_IRQ_Handler,
    ADC_IRQ_FIFO_Handler,
    I2C0_IRQ_Handler,
    I2C1_IRQ_Handler,
    RTC_IRQ_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler // 31
};

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

/**
 * The processor ends up here if an unexpected interrupt occurs or a
 * specific handler is not present in the application code.
 * When in DEBUG, trigger a debug exception to clearly notify
 * the user of the exception and help identify the cause.
 */
void __attribute__ ((section(".after_vectors")))
Default_Handler(void)
{
#if defined(DEBUG)
  __DEBUG_BKPT();
#endif
  while (1)
    {
      __WFI();
    }
}

// ----------------------------------------------------------------------------
