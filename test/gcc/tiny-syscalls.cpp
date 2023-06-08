/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
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

#include <cstdint>
#include <unistd.h>
#include <sys/stat.h>

extern "C"
{
  void __attribute__((weak))
  _start (void);

  void *
  _sbrk (int incr);

  void
  _exit (int code);

  int
  _kill (pid_t pid, int sig);

  pid_t
  _getpid (void);

  ssize_t
  _write (int fildes, const void *buf, size_t nbyte);

  int
  _close (int fildes);

  int
  _fstat (int fildes, struct stat *buf);

  int
  _isatty (int fildes);

  off_t
  _lseek (int fildes, off_t offset, int whence);

  ssize_t
  _read (int fildes, void *buf, size_t nbyte);

  extern int main (int, const char **);

}

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

void
_exit (int code)
{
  for (;;)
    ;
}

void *
_sbrk (int incr)
{
  return nullptr;
}

int
_kill (pid_t pid, int sig)
{
  return 0;
}

pid_t
_getpid (void)
{
  return 0;
}

ssize_t
_write (int fildes, const void *buf, size_t nbyte)
{
  return 0;
}

int
_close (int fildes)
{
  return 0;
}

int
_fstat (int fildes, struct stat *buf)
{
  return 0;
}

int
_isatty (int fildes)
{
  return 0;
}

off_t
_lseek (int fildes, off_t offset, int whence)
{
  return 0;
}

ssize_t
_read (int fildes, void *buf, size_t nbyte)
{
  return 0;
}

void
_start (void)
{
  main (1, nullptr);
}

#pragma GCC diagnostic pop
