#pragma once

#include "buffer.h"
#include <memory>

namespace gl {


class UniformBuffer {
  public:
    UniformBuffer();

  public:
    ~UniformBuffer();
    UniformBuffer(UniformBuffer const&) = delete;
    UniformBuffer& operator=(UniformBuffer const&) = delete;

  public:
    template<typename Container>
    void data(Container const& container, GLenum usage) {
      _buffer.data(container, usage, GL_UNIFORM_BUFFER);
    }
  
    template<typename Container>
    void subdata(size_t offset, size_t count, Container const& container) {
      _buffer.subdata(offset, count, container);
    }

    void bind(GLuint binding);

  private:
    Buffer _buffer;

};




}

