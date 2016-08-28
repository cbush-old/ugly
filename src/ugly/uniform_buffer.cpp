#include "uniform_buffer.h"

using namespace gl;

UniformBuffer::UniformBuffer() {}

UniformBuffer::~UniformBuffer() {}

void UniformBuffer::bind(GLuint binding) const {
  GL_CALL(glBindBufferBase(
    GL_UNIFORM_BUFFER,
    binding,
    _buffer.name()
  ));
}

