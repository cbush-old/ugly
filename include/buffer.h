#ifndef UGLY_BUFFER_H
#define UGLY_BUFFER_H

#include "gl_type.h"
#include <vector>

namespace gl {


class Buffer {
  public:
    Buffer();

  public:
    virtual ~Buffer();
    Buffer(Buffer const&) = delete;
    Buffer& operator=(Buffer const&) = delete;

  public:
    template<typename T>
    void data(std::vector<T> const&, GLenum usage);

    GLuint name() const;

  private:
    class Buffer_impl* _impl;

};




} // namespace gl

#endif
