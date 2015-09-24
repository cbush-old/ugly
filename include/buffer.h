#ifndef UGLY_BUFFER_H
#define UGLY_BUFFER_H

#include "gl_type.h"
#include <vector>

namespace gl {


class Buffer {
  public:
    Buffer();

  public:
    virtual ~Buffer() =0;
    Buffer(Buffer const&) = delete;
    Buffer& operator=(Buffer const&) = delete;

  public:
    template<typename T>
    void data(std::vector<T> const&);

  private:
    class Buffer_impl* _impl;

};



} // namespace gl

#endif
