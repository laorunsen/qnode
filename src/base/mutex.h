/*
 * Copyright (C) codedump
 */

#ifndef __QNODE_BASE_MUTEX_H__
#define __QNODE_BASE_MUTEX_H__

#include <pthread.h>
#include "base/base.h"

class Mutex {
public:
  Mutex() {
    pthread_mutexattr_init(&attr_);
    pthread_mutexattr_settype(&attr_, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex_, &attr_);
  }
  inline ~Mutex() {
    pthread_mutex_destroy(&mutex_);
    pthread_mutexattr_destroy(&attr_);
  }

  inline void Lock() {
    pthread_mutex_lock(&mutex_);
  }

  inline bool TryLock() {
    return pthread_mutex_trylock(&mutex_) != EBUSY;
  }

  inline void UnLock() {
    pthread_mutex_unlock(&mutex_);
  }

private:
  pthread_mutex_t mutex_;
  pthread_mutexattr_t attr_;

  DISALLOW_COPY_AND_ASSIGN(Mutex);
};

#endif // __QNODE_BASE_MUTEX_H__
