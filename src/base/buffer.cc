/*
 * Copyright (C) codedump
 */

#include <string.h>
#include "base/buffer.h"

size_t BufferList::Read(char* to, size_t n) {
  size_t orig = n;
  size_t read_size;

  while (n > 0 && totalSize() > 0) {
    read_size = readableSize();
    if (n > read_size) {
      memcpy(to, readPoint(), read_size);
      readAdvance(read_size);
      to += read_size;
      n  -= read_size;
    } else {
      memcpy(to, readPoint(), n);
      readAdvance(n);
      break;
    }
  }

out:  
  return orig - n;
}

void BufferList::Write(const char* from, size_t n) {
  size_t write_size;

  while (n > 0) {
    write_size = writeableSize();

    if (n > write_size) {
      memcpy(writePoint(), from, write_size);
      writeAdvance(write_size);
      from += write_size;
      n    -= write_size;
    } else {
      memcpy(writePoint(), from, n);
      break;
    }
  }
}

void BufferList::readAdvance(size_t n) {
  read_inx_ += n;
  if (read_inx_ == kBufferSize) {
    Buffer* buffer = buffer_list_.front();
    buffer_list_.pop_front();
    read_inx_ = 0;
    obj_list_->Free(buffer);
  }
}

void BufferList::writeAdvance(size_t n) {
  write_inx_ += n;
  if (write_inx_ == kBufferSize) {
    buffer_list_.push_back(obj_list_->Get());
    write_inx_ = 0;
  }
}

size_t BufferList::readableSize() const {
  if (buffer_list_.size() == 1) {
    return write_inx_ - read_inx_;
  }

  return kBufferSize = read_inx_;
}

size_t BufferList::writeableSize() const {
  return kBufferSize - write_inx_;
}