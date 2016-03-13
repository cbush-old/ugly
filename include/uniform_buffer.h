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
    template<typename T>
    void data(std::vector<T> const& container, GLenum usage) {
      _buffer.data(container, usage, GL_UNIFORM_BUFFER);
    }

    void bind(GLuint binding);

  private:
    Buffer _buffer;

};




}

