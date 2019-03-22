/*
 * Copyright (C) codedump
 */

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "base/assert.h"
#include "base/net.h"
#include "core/acceptor_handler.h"
#include "core/config.h"
#include "core/listener.h"
#include "core/log.h"
#include "core/session.h"

Listener::Listener(const string& addr, int port, Poller* poller,
                   AcceptorHandler *h, SessionFactory *f)
  : addr_(addr),
    port_(port),
    poller_(poller),
    handler_(h),
    factory_(f) {
  int error;      
  fd_ = Listen(addr, port, kBacklog, &error);
  Assert(fd > 0);
  handle_ = poller_->Add(fd_, this, kEventRead);
}

Listener::~Listener() {
  Close(fd_);
}

void
Listener::In() {
  int error;

  while (true) {
    int fd = Accept(fd_, &error);
    if (fd > 0) {
      Session *s = factory_->Create(fd);
      handler_->OnAccept(s);
    } else if (fd == 0) {
      break;
    } else {
      handler_->OnError(error);
      Errorf("accept connection error: %s", strerror(error));
    }
  }
}

void
Listener::Out() {
}

void
Listener::Timeout() {
}

string
Listener::String() {
  char buf[100];
  snprintf(buf, sizeof(buf), "%s-%d", addr_.c_str(), port_);

  return string(buf);
}
