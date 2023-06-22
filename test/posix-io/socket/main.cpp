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

#include "posix-io/file_descriptors_manager.h"
#include "posix-io/io.h"
#include "posix-io/TPool.h"
#include "posix-io/socket.h"
#include "posix-io/net_interface.h"
#include "posix-io/net_stack.h"
#include <cmsis-plus/diag/trace.h>

#include "posix/sys/socket.h"

#include <cerrno>
#include <cassert>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include "utime.h"

#if defined(__ARM_EABI__)
#include "posix-io/redefinitions.h"
#endif

// ----------------------------------------------------------------------------

enum class Cmds
  : unsigned int
    { UNKNOWN,
  NOTSET,
  CLOSE,
  READ,
  WRITE,
  WRITEV,
  IOCTL,
  FCNTL,
  SOCKET,
  ACCEPT,
  BIND,
  CONNECT,
  GETPEERNAME,
  GETSOCKNAME,
  GETSOCKOPT,
  LISTEN,
  RECV,
  RECVFROM,
  RECVMSG,
  SEND,
  SENDMSG,
  SENDTO,
  SETSOCKOPT,
  SHUTDOWN,
  SOCKATMARK
};

// Test class, all methods store the input in local variables,
// to be checked later.

class TestSocket : public os::posix::socket
{
public:

  TestSocket ();

  // Methods used for test purposes only.

  void
  clear (void);

  Cmds
  getCmd (void);

  int
  getMode (void);

  const char*
  path (void);

  unsigned int
  getNumber1 (void);

  unsigned int
  getNumber2 (void);

  unsigned int
  getNumber3 (void);

  void*
  getPtr1 (void);

  void*
  getPtr2 (void);

  void*
  getPtr3 (void);

protected:

  // Implementations.

  virtual int
  do_socket (int domain, int type, int protocol) override;

  virtual int
  do_close (void) override;

  virtual ssize_t
  do_read (void* buf, std::size_t nbyte) override;

  virtual ssize_t
  do_write (const void* buf, std::size_t nbyte) override;

  virtual ssize_t
  do_writev (const struct iovec* iov, int iovcnt) override;

  virtual int
  do_vfcntl (int cmd, va_list args) override;

  int
  do_accept (socket* sock, struct sockaddr* address, socklen_t* address_len)
      override;

  int
  do_bind (const struct sockaddr* address, socklen_t address_len) override;

  int
  do_connect (const struct sockaddr* address, socklen_t address_len) override;

  int
  do_getpeername (struct sockaddr* address, socklen_t* address_len) override;

  int
  do_getsockname (struct sockaddr* address, socklen_t* address_len) override;

  int
  do_getsockopt (int level, int option_name, void* option_value,
                 socklen_t* option_len) override;

  int
  do_listen (int backlog) override;

  ssize_t
  do_recv (void* buffer, size_t length, int flags) override;

  ssize_t
  do_recvfrom (void* buffer, size_t length, int flags, struct sockaddr* address,
               socklen_t* address_len) override;

  ssize_t
  do_recvmsg (struct msghdr* message, int flags) override;

  ssize_t
  do_send (const void* buffer, size_t length, int flags) override;

  ssize_t
  do_sendmsg (const struct msghdr* message, int flags) override;

  ssize_t
  do_sendto (const void* message, size_t length, int flags,
             const struct sockaddr* dest_addr, socklen_t dest_len) override;

  int
  do_setsockopt (int level, int option_name, const void* option_value,
                 socklen_t option_len) override;

  int
  do_shutdown (int how) override;

  int
  do_sockatmark (void) override;

private:

  uint32_t fSomething;
  const char* fPath;
  int fMode;
  unsigned int fNumber1;
  unsigned int fNumber2;
  unsigned int fNumber3;
  void* fPtr1;
  void* fPtr2;
  void* fPtr3;
  Cmds fCmd;

};

TestSocket::TestSocket ()
{
  clear ();
}

void
TestSocket::clear (void)
{
  fCmd = Cmds::NOTSET;
  fPath = nullptr;
  fMode = -1;
  fSomething = 1;
  fNumber1 = 1;
  fNumber2 = 1;
  fNumber3 = 1;
  fPtr1 = nullptr;
  fPtr2 = nullptr;
  fPtr3 = nullptr;
}

inline Cmds
TestSocket::getCmd (void)
{
  return fCmd;
}

inline unsigned int
TestSocket::getNumber1 (void)
{
  return fNumber1;
}

inline unsigned int
TestSocket::getNumber2 (void)
{
  return fNumber2;
}

inline unsigned int
TestSocket::getNumber3 (void)
{
  return fNumber3;
}

inline int
TestSocket::getMode (void)
{
  return fMode;
}

inline const char*
TestSocket::path (void)
{
  return fPath;
}

inline void*
TestSocket::getPtr1 (void)
{
  return fPtr1;
}

inline void*
TestSocket::getPtr2 (void)
{
  return fPtr2;
}

inline void*
TestSocket::getPtr3 (void)
{
  return fPtr3;
}

int
TestSocket::do_close (void)
{
  fCmd = Cmds::CLOSE;
  return 0; // Always return success
}

ssize_t
TestSocket::do_read (void *buf, std::size_t nbyte)
{
  fCmd = Cmds::READ;
  fPtr1 = buf;
  fNumber1 = nbyte;
  return 0;
}

ssize_t
TestSocket::do_write (const void* buf, std::size_t nbyte)
{
  fCmd = Cmds::WRITE;
  fPtr1 = (void*) buf;
  fNumber1 = nbyte;
  return 0;
}

ssize_t
TestSocket::do_writev (const struct iovec* iov, int iovcnt)
{
  fCmd = Cmds::WRITEV;
  fPtr1 = (void*) iov;
  fNumber1 = iovcnt;
  return 0;
}

int
TestSocket::do_vfcntl (int cmd, std::va_list args)
{
  fCmd = Cmds::FCNTL;
  fNumber1 = cmd;
  fMode = va_arg(args, int);
  return 0;
}

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

int
TestSocket::do_socket (int domain, int type, int protocol)
{
  fCmd = Cmds::SOCKET;
  fNumber1 = domain;
  fNumber2 = type;
  fNumber3 = protocol;

  return 0;
}

int
TestSocket::do_accept (socket* sock, struct sockaddr* address,
                       socklen_t* address_len)
{
  fCmd = Cmds::SOCKET;
  fPtr1 = address;
  fPtr2 = address_len;

  return 0;
}

int
TestSocket::do_bind (const struct sockaddr* address, socklen_t address_len)
{
  fCmd = Cmds::BIND;
  fPtr1 = (void*) address;
  fNumber1 = address_len;

  return 0;
}

int
TestSocket::do_connect (const struct sockaddr* address, socklen_t address_len)
{
  fCmd = Cmds::CONNECT;
  fPtr1 = (void*) address;
  fNumber1 = address_len;

  return 0;
}

int
TestSocket::do_getpeername (struct sockaddr* address, socklen_t* address_len)
{
  fCmd = Cmds::GETPEERNAME;
  fPtr1 = (void*) address;
  fPtr2 = address_len;

  return 0;
}

int
TestSocket::do_getsockname (struct sockaddr* address, socklen_t* address_len)
{
  fCmd = Cmds::GETSOCKNAME;
  fPtr1 = (void*) address;
  fPtr2 = address_len;

  return 0;
}

int
TestSocket::do_getsockopt (int level, int option_name, void* option_value,
                           socklen_t* option_len)
{
  fCmd = Cmds::GETSOCKOPT;
  fNumber1 = level;
  fNumber2 = option_name;
  fPtr1 = option_value;
  fPtr2 = option_len;

  return 0;
}

int
TestSocket::do_listen (int backlog)
{
  fCmd = Cmds::LISTEN;
  fNumber1 = backlog;

  return 0;
}

ssize_t
TestSocket::do_recv (void* buffer, size_t length, int flags)
{
  fCmd = Cmds::RECV;
  fPtr1 = buffer;
  fNumber1 = length;
  fNumber2 = flags;

  return length / 2;
}

ssize_t
TestSocket::do_recvfrom (void* buffer, size_t length, int flags,
                         struct sockaddr* address, socklen_t* address_len)
{
  fCmd = Cmds::RECVFROM;
  fPtr1 = buffer;
  fNumber1 = length;
  fNumber2 = flags;
  fPtr2 = address;
  fPtr3 = address_len;

  return length / 2;
}

ssize_t
TestSocket::do_recvmsg (struct msghdr* message, int flags)
{
  fCmd = Cmds::RECVMSG;
  fPtr1 = message;
  fNumber1 = flags;

  return flags / 2;
}

ssize_t
TestSocket::do_send (const void* buffer, size_t length, int flags)
{
  fCmd = Cmds::SEND;
  fPtr1 = (void*) buffer;
  fNumber1 = length;
  fNumber2 = flags;

  return length / 2;
}

ssize_t
TestSocket::do_sendmsg (const struct msghdr* message, int flags)
{
  fCmd = Cmds::SENDMSG;
  fPtr1 = (void*) message;
  fNumber1 = flags;

  return flags / 2;
}

ssize_t
TestSocket::do_sendto (const void* message, size_t length, int flags,
                       const struct sockaddr* dest_addr, socklen_t dest_len)
{
  fCmd = Cmds::SENDTO;
  fPtr1 = (void*) message;
  fNumber1 = length;
  fNumber2 = flags;
  fPtr2 = (void*) dest_addr;
  fNumber3 = dest_len;

  return length / 2;
}

int
TestSocket::do_setsockopt (int level, int option_name, const void* option_value,
                           socklen_t option_len)
{
  fCmd = Cmds::SETSOCKOPT;
  fNumber1 = level;
  fNumber2 = option_name;
  fPtr1 = (void*) option_value;
  fNumber3 = option_len;

  return 0;
}

int
TestSocket::do_shutdown (int how)
{
  fCmd = Cmds::SHUTDOWN;
  fNumber1 = how;

  return 0;
}

#pragma GCC diagnostic pop

int
TestSocket::do_sockatmark (void)
{
  fCmd = Cmds::SOCKATMARK;

  return 0;
}

// ----------------------------------------------------------------------------

class TestNetInterface : public os::posix::net_interface
{
public:
  TestNetInterface () = default;
};

// ----------------------------------------------------------------------------

using TestSocketPool = os::posix::pool_typed<TestSocket>;

constexpr std::size_t SOCKETS_POOL_ARRAY_SIZE = 2;

// Pool of file objects, used in common by all filesystems.
TestSocketPool socketsPool
  { SOCKETS_POOL_ARRAY_SIZE };

TestNetInterface if0;

os::posix::net_stack net
  { &socketsPool };

// Static manager
os::posix::file_descriptors_manager dm
  { 5 };

// ----------------------------------------------------------------------------

int
main (int argc __attribute__((unused)), char* argv[] __attribute__((unused)))
{
    {
      // C API

      // Test SOCKET.
      errno = -2;
      int fd;
      assert(((fd = __posix_socket (123, 234, 345)) == 3) && (errno == 0));

      os::posix::io* io = os::posix::file_descriptors_manager::io (fd);
      assert(io != nullptr);

      assert(io->get_type () == static_cast<posix::io::type_t>(os::posix::io::type::socket));

      TestSocket* tsock = static_cast<TestSocket*> (io);
      assert(socketsPool.object (0) == tsock);
      assert(socketsPool.in_use (0) == true);

      // Check SOCKET params.
      assert(tsock->getCmd () == Cmds::SOCKET);
      assert(tsock->getNumber1 () == 123);
      assert(tsock->getNumber2 () == 234);
      assert(tsock->getNumber3 () == 345);

      int fd2;
      struct sockaddr addr1;
      socklen_t len1;
      assert(((fd2 = __posix_accept(fd, &addr1, &len1)) == 4) && (errno == 0));

      os::posix::io* io2 = os::posix::file_descriptors_manager::io (fd2);
      assert(io2 != nullptr);

      assert(io2->get_type () == static_cast<posix::io::type_t>(os::posix::io::type::socket));

      TestSocket* tsock2 = static_cast<TestSocket*> (io2);
      assert(socketsPool.object (1) == tsock2);
      assert(socketsPool.in_use (1) == true);

      assert(tsock->getPtr1 () == &addr1);
      assert(tsock->getPtr2 () == &len1);

      // Test second socket CLOSE.
      errno = -2;
      tsock->clear ();
      int ret = __posix_close (fd2);
      assert((ret == 0) && (errno == 0));
      assert(tsock2->getCmd () == Cmds::CLOSE);

      // Must no longer be in the pool
      assert(socketsPool.in_use (1) == false);

      // Test BIND
      errno = -2;
      tsock->clear ();
      assert(((ret = __posix_bind(fd, &addr1, 123)) == 0) && (errno == 0));

      assert(tsock->getCmd () == Cmds::BIND);
      assert(tsock->getPtr1 () == &addr1);
      assert(tsock->getNumber1 () == 123);

      // Test CONNECT
      errno = -2;
      tsock->clear ();
      assert(((ret = __posix_connect(fd, &addr1, 234)) == 0) && (errno == 0));

      assert(tsock->getCmd () == Cmds::CONNECT);
      assert(tsock->getPtr1 () == &addr1);
      assert(tsock->getNumber1 () == 234);

      // Test GETPEERNAME
      errno = -2;
      tsock->clear ();
      assert(
          ((ret = __posix_getpeername(fd, &addr1, &len1)) == 0) && (errno == 0));

      assert(tsock->getCmd () == Cmds::GETPEERNAME);
      assert(tsock->getPtr1 () == &addr1);
      assert(tsock->getPtr2 () == &len1);

      // Test GETSOCKNAME
      errno = -2;
      tsock->clear ();
      assert(
          ((ret = __posix_getsockname(fd, &addr1, &len1)) == 0) && (errno == 0));

      assert(tsock->getCmd () == Cmds::GETSOCKNAME);
      assert(tsock->getPtr1 () == &addr1);
      assert(tsock->getPtr2 () == &len1);

      // Test GETSOCKOPT
      errno = -2;
      tsock->clear ();
      char opt[2];
      assert(
          ((ret = __posix_getsockopt(fd, 123, 234, (void*)&opt, &len1)) == 0) && (errno == 0));

      assert(tsock->getCmd () == Cmds::GETSOCKOPT);
      assert(tsock->getNumber1 () == 123);
      assert(tsock->getNumber2 () == 234);
      assert(tsock->getPtr1 () == &opt);
      assert(tsock->getPtr2 () == &len1);

      // Test RECV
      errno = -2;
      tsock->clear ();
      char buf[2];
      assert(
          ((ret = __posix_recv(fd, &buf, 234, 345)) == (234/2)) && (errno == 0));

      assert(tsock->getCmd () == Cmds::RECV);
      assert(tsock->getPtr1 () == &buf);
      assert(tsock->getNumber1 () == 234);
      assert(tsock->getNumber2 () == 345);

      // Test RECVFROM
      errno = -2;
      tsock->clear ();
      assert(
          ((ret = __posix_recvfrom(fd, &buf, 234, 345, &addr1, &len1)) == (234/2)) && (errno == 0));

      assert(tsock->getCmd () == Cmds::RECVFROM);
      assert(tsock->getPtr1 () == &buf);
      assert(tsock->getNumber1 () == 234);
      assert(tsock->getNumber2 () == 345);
      assert(tsock->getPtr2 () == &addr1);
      assert(tsock->getPtr3 () == &len1);

      // Test RECVMSG
      errno = -2;
      tsock->clear ();
      assert(
          ((ret = __posix_recvmsg(fd, (struct msghdr*)&buf, 234)) == (234/2)) && (errno == 0));

      assert(tsock->getCmd () == Cmds::RECVMSG);
      assert(tsock->getPtr1 () == &buf);
      assert(tsock->getNumber1 () == 234);

      // Test SEND
      errno = -2;
      tsock->clear ();
      assert(
          ((ret = __posix_send(fd, &buf, 234, 345)) == (234/2)) && (errno == 0));

      assert(tsock->getCmd () == Cmds::SEND);
      assert(tsock->getPtr1 () == &buf);
      assert(tsock->getNumber1 () == 234);
      assert(tsock->getNumber2 () == 345);

      // Test SENDMSG
      errno = -2;
      tsock->clear ();
      assert(
          ((ret = __posix_sendmsg(fd, (struct msghdr*)&buf, 234)) == (234/2)) && (errno == 0));

      assert(tsock->getCmd () == Cmds::SENDMSG);
      assert(tsock->getPtr1 () == &buf);
      assert(tsock->getNumber1 () == 234);

      // Test SENDTO
      errno = -2;
      tsock->clear ();
      assert(
          ((ret = __posix_sendto(fd, &buf, 234, 345, &addr1, 456)) == (234/2)) && (errno == 0));

      assert(tsock->getCmd () == Cmds::SENDTO);
      assert(tsock->getPtr1 () == &buf);
      assert(tsock->getNumber1 () == 234);
      assert(tsock->getNumber2 () == 345);
      assert(tsock->getPtr2 () == &addr1);
      assert(tsock->getNumber3 () == 456);

      // Test SETSOCKOPT
      errno = -2;
      tsock->clear ();
      assert(
          ((ret = __posix_setsockopt(fd, 123, 234, (void*)&opt, 345)) == 0) && (errno == 0));

      assert(tsock->getCmd () == Cmds::SETSOCKOPT);
      assert(tsock->getNumber1 () == 123);
      assert(tsock->getNumber2 () == 234);
      assert(tsock->getPtr1 () == &opt);
      assert(tsock->getNumber3 () == 345);

      // Test SHUTDOWN
      errno = -2;
      tsock->clear ();
      assert(((ret = __posix_shutdown(fd, 123)) == 0) && (errno == 0));

      assert(tsock->getCmd () == Cmds::SHUTDOWN);
      assert(tsock->getNumber1 () == 123);

      // Test SOCKATMARK
      errno = -2;
      tsock->clear ();
      assert(((ret = __posix_sockatmark(fd)) == 0) && (errno == 0));

      assert(tsock->getCmd () == Cmds::SOCKATMARK);

      // Test CLOSE.
      errno = -2;
      tsock->clear ();
      ret = __posix_close (fd);
      assert((ret == 0) && (errno == 0));
      assert(tsock->getCmd () == Cmds::CLOSE);

      // Must no longer be in the pool
      assert(socketsPool.in_use (0) == false);
    }

    {
      // C++ API

      // Test SOCKET.
      errno = -2;
      os::posix::socket* sock;
      assert(
          ((sock = os::posix::socket(123, 234, 345)) != nullptr) && errno == 0);
      assert(sock->get_type () == static_cast<posix::io::type_t>(os::posix::io::type::socket));

      assert(sock->file_descriptor () > 0);

      TestSocket* tsock = static_cast<TestSocket*> (sock);
      assert(socketsPool.object (0) == tsock);
      assert(socketsPool.in_use (0) == true);

      // Check SOCKET params.
      assert(tsock->getCmd () == Cmds::SOCKET);
      assert(tsock->getNumber1 () == 123);
      assert(tsock->getNumber2 () == 234);
      assert(tsock->getNumber3 () == 345);

      struct sockaddr addr1;
      socklen_t len1;
      os::posix::socket* sock2;
      assert(
          ((sock2 = sock->accept(&addr1, &len1)) != nullptr) && (errno == 0));

      assert(sock2->get_type () == static_cast<posix::io::type_t>(os::posix::io::type::socket));

      assert(sock2->file_descriptor () > 0);

      TestSocket* tsock2 = static_cast<TestSocket*> (sock2);
      assert(socketsPool.object (1) == tsock2);
      assert(socketsPool.in_use (1) == true);

      assert(tsock->getPtr1 () == &addr1);
      assert(tsock->getPtr2 () == &len1);

      // Test second socket CLOSE.
      errno = -2;
      tsock->clear ();
      int ret = sock2->close ();
      assert((ret == 0) && (errno == 0));
      assert(tsock2->getCmd () == Cmds::CLOSE);

      // Must no longer be in the pool
      assert(socketsPool.in_use (1) == false);

      // Test CLOSE.
      errno = -2;
      ret = sock->close ();
      assert((ret == 0) && (errno == 0));
      assert(tsock->getCmd () == Cmds::CLOSE);

      // Must no longer be in the pool
      assert(socketsPool.in_use (0) == false);
    }

  trace_puts ("'test-socket-debug' succeeded.");

  // Success!
  return 0;
}

// ----------------------------------------------------------------------------

