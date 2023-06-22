/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#ifndef CMSIS_PLUS_CORTEXM_EXCEPTION_HANDLERS_H_
#define CMSIS_PLUS_CORTEXM_EXCEPTION_HANDLERS_H_

#include <stdint.h>

#if defined(DEBUG)
#define __DEBUG_BKPT()  __asm__ volatile ("bkpt 0")
#endif

// ----------------------------------------------------------------------------

#if defined(__cplusplus)
extern "C"
{
#endif

  extern void
  Reset_Handler (void);

  extern void
  NMI_Handler (void);

  extern void
  HardFault_Handler (void);

#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)

  extern void
  MemManage_Handler (void);

  extern void
  BusFault_Handler (void);

  extern void
  UsageFault_Handler (void);

  extern void
  DebugMon_Handler (void);

#endif

  extern void
  SVC_Handler (void);

  extern void
  PendSV_Handler (void);

  extern void
  SysTick_Handler (void);

  // Exception Stack Frame for the Cortex-M3 and Cortex-M4 processor.
  typedef struct exception_stack_frame_s
  {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t psr;
#if  defined(__ARM_ARCH_7EM__)
    uint32_t s[16];
#endif
  } exception_stack_frame_t;

#if defined(TRACE)
#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
  void
  dump_exception_stack (exception_stack_frame_t* frame, uint32_t cfsr,
                        uint32_t mmfar, uint32_t bfar, uint32_t lr);
#endif // defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
#if defined(__ARM_ARCH_6M__)
  void
  dump_exception_stack (exception_stack_frame_t* frame, uint32_t lr);
#endif // defined(__ARM_ARCH_6M__)
#endif // defined(TRACE)

  void
  HardFault_Handler_C (exception_stack_frame_t* frame, uint32_t lr);

#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)

  void
  UsageFault_Handler_C (exception_stack_frame_t* frame, uint32_t lr);

  void
  BusFault_Handler_C (exception_stack_frame_t* frame, uint32_t lr);

#endif // defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)

#if defined(__cplusplus)
}
#endif

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_CORTEXM_EXCEPTION_HANDLERS_H_ */
