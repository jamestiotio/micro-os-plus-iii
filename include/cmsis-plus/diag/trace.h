/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
 *
 * µOS++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 3.
 *
 * µOS++ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CMSIS_PLUS_DIAG_TRACE_H_
#define CMSIS_PLUS_DIAG_TRACE_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)
#include <cstdint>
#include <cstddef>
#include <cstdarg>
#else
#include <stdint.h>
#include <stdarg.h>
#endif

#include <sys/types.h>

// ----------------------------------------------------------------------------

// The trace device is an independent output channel, intended for diagnostic
// purposes.
//
// The API is simple, and mimics the standard C output calls:
// - os::cmsis::trace::printf()/trace_printf()
// - os::cmsis::trace::puts()/trace_puts()
// - os::cmsis::trace::putchar()/trace_putchar();
//
// The implementation is done in:
// - os::cmsis::trace::initialize()
// - os::cmsis::trace::write()
// If these functions are not defined in another place, there are
// weak definitions that simply discard the trace output.
//
// Trace support is enabled by adding the TRACE macro definition.
//
// When TRACE is not defined, all functions are inlined to empty bodies.
// This has the advantage that the trace calls do not need to be conditionally
// compiled with #ifdef TRACE/#endif

#if defined(TRACE)

#if defined(__cplusplus)

// This is a very annoying issue, some very old libraries still
// define putchar() as a macro. This is abusive, since it prevents
// the use of putchar() in other name spaces.
#if defined(putchar)
#undef putchar
#endif

namespace os
{
  namespace cmsis
  {
    namespace trace
    {
      // ----------------------------------------------------------------------

      void
      initialize (void);

      ssize_t
      write (const void* buf, std::size_t nbyte);

      // ----------------------------------------------------------------------

      int
      printf (const char* format, ...);

      int
      vprintf (const char* format, std::va_list args);

      int
      puts (const char* s);

      int
      putchar (int c);

      void
      dumpArgs (int argc, char* argv[]);

    } /* namespace trace */
  } /* namespace cmsis */
} /* namespace os */

#endif /* defined(__cplusplus) */

#if defined(__cplusplus)
extern "C"
{
#endif

  // ----- Implementation -----
  // These two functions must be implemented for a specific trace channel.

  /**
   * Called from startup.
   */
  void
  __initialize_trace (void);

  ssize_t
  trace_write (const void* buf, size_t nbyte);

  // ----- Portable -----

  int
  trace_printf (const char* format, ...);

  int
  trace_vprintf (const char* format, va_list args);

  int
  trace_puts (const char* s);

  int
  trace_putchar (int c);

  void
  trace_dump_args (int argc, char* argv[]);

#if defined(__cplusplus)
}
#endif

#else /* !defined(TRACE) */

// Empty definitions when trace is not defined

#if defined(__cplusplus)

namespace os
  {
    namespace cmsis
      {
        namespace trace
          {
            // ----------------------------------------------------------------

            inline void
            initialize (void);

            inline ssize_t
            write (const void* buf, std::size_t nbyte);

            // ----------------------------------------------------------------

            inline int
            printf (const char* format, ...);

            inline int
            vprintf (const char* format, std::va_list args);

            inline int
            puts (const char* s);

            inline int
            putchar (int c);

            inline void
            dumpArgs (int argc, char* argv[]);

            // ----------------------------------------------------------------

            inline void __attribute__((always_inline))
            initialize (void)
              {
              }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

            inline ssize_t __attribute__((always_inline))
            write (const void* buf, std::size_t nbyte)
              {
                return (ssize_t)nbyte;
              }

            inline int __attribute__((always_inline))
            printf (const char* format, ...)
              {
                return 0;
              }

            inline int __attribute__((always_inline))
            vprintf (const char* format, std::va_list args)
              {
                return 0;
              }

            inline int __attribute__((always_inline))
            puts (const char* s)
              {
                return 0;
              }

            inline int __attribute__((always_inline))
            putchar (int c)
              {
                return c;
              }

            inline void __attribute__((always_inline))
            dumpArgs (int argc, char* argv[])
              {
              }

#pragma GCC diagnostic pop

          } /* namespace trace */
      } /* namespace cmsis */
  } /* namespace os */

#endif /* defined(__cplusplus) */

#if defined(__cplusplus)
extern "C"
  {
#endif

    inline void
    __initialize_trace (void);

    // Implementation dependent
    inline ssize_t
    trace_write (const void* buf, size_t nbyte);

    inline int
    trace_printf (const char* format, ...);

    inline int
    trace_vprintf (const char* format, va_list args);

    inline int
    trace_puts (const char* s);

    inline int
    trace_putchar (int c);

    inline void
    trace_dump_args (int argc, char* argv[]);

#if defined(__cplusplus)
  }
#endif

inline void
__attribute__((always_inline))
__initialize_trace (void)
  {
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

inline ssize_t
__attribute__((always_inline))
trace_write (const void* buf, size_t nbyte)
  {
    return (ssize_t)nbyte;
  }

inline int
__attribute__((always_inline))
trace_printf (const char* format, ...)
  {
    return 0;
  }

inline int
__attribute__((always_inline))
trace_vprintf (const char* format, va_list args)
  {
    return 0;
  }

inline int
__attribute__((always_inline))
trace_puts (const char* s)
  {
    return 0;
  }

inline int
__attribute__((always_inline))
trace_putchar (int c)
  {
    return c;
  }

inline void
__attribute__((always_inline))
trace_dump_args (int argc, char* argv[])
  {
  }

#pragma GCC diagnostic pop

#endif /* defined(TRACE) */

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_DIAG_TRACE_H_ */