/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#ifndef POSIX_MQUEUE_H_
#define POSIX_MQUEUE_H_

#if !defined(__ARM_EABI__)
#include <mqueue.h>
#else

#include <sys/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

// ----------------------------------------------------------------------------
// Standard definitions from:
// http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/mqueue.h.html
// ----------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Types
  using mq_attr = struct
    {
      long mq_flags;    // Message queue flags.
      long mq_maxmsg;// Maximum number of messages.
      long mq_msgsize;// Maximum message size.
      long mq_curmsgs;// Number of messages currently queued.
    };

  using mqd_t = void*;

  mqd_t
  mq_open (const char* name, int oflag, ...);
  int
  mq_close (mqd_t mqdes);
  int
  mq_unlink (const char* name);

  int
  mq_getattr (mqd_t mqdes, struct mq_attr* attr);
  int
  mq_setattr (mqd_t mqdes, const struct mq_attr* attr, struct mq_attr* attr);

  int
  mq_notify (mqd_t mqdes, const struct sigevent* notification);

  ssize_t
  mq_receive (mqd_t mqdes, char* msg_ptr, size_t msg_len, unsigned* msg_prio);
  ssize_t
  mq_timedreceive (mqd_t mqdes, char* msg_ptr, size_t msg_len,
                   unsigned* msg_prio, const struct timespec* abstime);

  int
  mq_send (mqd_t mqdes, const char* msg_ptr, size_t msg_len, unsigned msg_prio);
  int
  mq_timedsend (mqd_t mqdes, const char* msg_ptr, size_t msg_len,
                unsigned msg_prio, const struct timespec* abstime);

// ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif /* __ARM_EABI__ */

#endif /* POSIX_MQUEUE_H_ */
