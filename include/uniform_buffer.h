#pragma once

#include "buffer.h"
#include <memory>
#include <vector>
#include <array>

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

    template<typename T, size_t N>
    void data(std::array<T, N> const& container, GLenum usage) {
      _buffer.data(container, usage, GL_UNIFORM_BUFFER);
    }
  
    void data(GLsizei size, GLenum usage) {
      _buffer.data(size, usage, GL_UNIFORM_BUFFER);
    }
  
    template<typename Container>
    void subdata(size_t offset, size_t count, Container const& container) {
      _buffer.subdata(offset, count, container, GL_UNIFORM_BUFFER);
    }

    void bind(GLuint binding);

  private:
    Buffer _buffer;

};




}

