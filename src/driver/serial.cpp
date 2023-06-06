/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 * Copyright (c) 2013-2014 ARM Ltd.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * The code is inspired by ARM CMSIS Driver_USART.h file, v2.02,
 * and tries to remain functionally close to the CMSIS specifications.
 */

#include <cmsis-plus/driver/serial.h>
#include <cmsis-plus/diag/trace.h>

#include <cassert>

// ----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

// ----------------------------------------------------------------------------

namespace os
{
  namespace driver
  {
    // ----------------------------------------------------------------------

    Serial::Serial () noexcept
    {
      trace::printf ("%s() %p\n", __func__, this);

      cb_func_ = nullptr;
      cb_object_ = nullptr;

      clean ();
    }

    Serial::~Serial () noexcept
    {
      trace::printf ("%s() %p\n", __func__, this);
    }

    // ----------------------------------------------------------------------

    void
    Serial::clean (void) noexcept
    {
      status_.rx_break = false;
      status_.rx_busy = false;
      status_.rx_framing_error = false;
      status_.rx_overflow = false;
      status_.rx_parity_error = false;
      status_.tx_busy = false;
      status_.tx_underflow = false;

      modem_status_.cts = false;
      modem_status_.dsr = false;
      modem_status_.dcd = false;
      modem_status_.ri = false;
    }

    // ----------------------------------------------------------------------

    void
    Serial::register_callback (signal_event_t cb_func, const void* cb_object) noexcept
    {
      cb_func_ = cb_func;
      cb_object_ = cb_object;
    }

    return_t
    Serial::send (const void* data, std::size_t num) noexcept
    {
      assert (data != nullptr);
      if (num == 0)
        {
          return RETURN_OK;
        }
      return do_send (data, num);
    }

    return_t
    Serial::receive (void* data, std::size_t num) noexcept
    {
      assert (data != nullptr);
      if (num == 0)
        {
          return RETURN_OK;
        }
      return do_receive (data, num);
    }

    return_t
    Serial::transfer (const void* data_out, void* data_in, std::size_t num) noexcept
    {
      assert (data_out != nullptr);
      assert (data_in != nullptr);
      if (num == 0)
        {
          return RETURN_OK;
        }
      return do_transfer (data_out, data_in, num);
    }

  } /* namespace driver */
} /* namespace os */

// ----------------------------------------------------------------------------
