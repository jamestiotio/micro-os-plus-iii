/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2018 Liviu Ionescu.
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

#include <cmsis-plus/posix-io/device-block-partition.h>

#include <cmsis-plus/diag/trace.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ========================================================================

    device_block_partition::device_block_partition (device_block_impl& impl,
                                                    const char* name) :
        device_block
          { impl, name }
    {
      trace::printf ("device_block_partition::%s(\"%s\")=@%p\n", __func__,
                     name_, this);
    }

    device_block_partition::~device_block_partition ()
    {
      trace::printf ("device_block_partition::%s() @%p %s\n", __func__, this,
                     name_);
    }

    // ------------------------------------------------------------------------

    void
    device_block_partition::configure (blknum_t offset, blknum_t nblocks)
    {
      trace::printf ("device_block_partition::%s(%u,%u) @%p\n", __func__,
                     offset, nblocks, this);

      impl ().configure (offset, nblocks);
    }

    // ========================================================================

    device_block_partition_impl::device_block_partition_impl (
        device_block_partition& self, device_block& parent) :
        device_block_impl
          { self }, //
        parent_ (parent)
    {
      trace::printf ("device_block_partition_impl::%s()=@%p\n", __func__, this);
    }

    device_block_partition_impl::~device_block_partition_impl ()
    {
      trace::printf ("device_block_partition_impl::%s() @%p\n", __func__, this);
    }

    // ----------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    int
    device_block_partition_impl::do_vioctl (int request, std::va_list args)
    {
      errno = ENOSYS;
      return -1;
    }

#pragma GCC diagnostic pop

    void
    device_block_partition_impl::configure (blknum_t offset, blknum_t nblocks)
    {
      trace::printf ("device_block_partition_impl::%s(%u,%u) @%p\n", __func__,
                     offset, nblocks, this);

      partition_offset_blocks_ = offset;
      assert(nblocks > 0);
      num_blocks_ = nblocks;

      // Inherit from parent.
      block_logical_size_bytes_ = parent_.block_logical_size_bytes ();
      block_physical_size_bytes_ = parent_.block_physical_size_bytes ();
    }

    int
    device_block_partition_impl::do_vopen (const char* path, int oflag,
                                           std::va_list args)
    {
      trace::printf ("device_block_partition_impl::%s(%d) @%p\n", __func__,
                     oflag, this);

      return parent_.vopen (path, oflag, args);
    }

    ssize_t
    device_block_partition_impl::do_read_block (void* buf, blknum_t blknum,
                                                std::size_t nblocks)
    {
      trace::printf ("device_block_partition_impl::%s(0x%X, %u, %u) @%p\n",
                     __func__, buf, blknum, nblocks, this);

      return parent_.read_block (buf, blknum + partition_offset_blocks_,
                                 nblocks);
    }

    ssize_t
    device_block_partition_impl::do_write_block (const void* buf,
                                                 blknum_t blknum,
                                                 std::size_t nblocks)
    {
      trace::printf ("device_block_partition_impl::%s(0x%X, %u, %u) @%p\n",
                     __func__, buf, blknum, nblocks, this);

      return parent_.write_block (buf, blknum + partition_offset_blocks_,
                                  nblocks);
    }

    void
    device_block_partition_impl::do_sync (void)
    {
      trace::printf ("device_block_partition_impl::%s() @%p\n", __func__, this);

      return parent_.sync ();
    }

    int
    device_block_partition_impl::do_close (void)
    {
      trace::printf ("device_block_partition_impl::%s() @%p\n", __func__, this);

      return parent_.close ();
    }

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
