#include "buffer.h"
#include "generated_object.h"

namespace gl {

class Buffer_impl : public GeneratedObject<glGenBuffers, glDeleteBuffers> {
  public:
    ~Buffer_impl() {}

  public:
    template<typename T>
    inline void data(std::vector<T> const&, GLenum usage);

};


template<typename T>
inline void Buffer_impl::data(std::vector<T> const& v, GLenum usage) {
  // 4.5: see glNamedBufferData
  GL_CALL(glBindBuffer(GL_COPY_WRITE_BUFFER, name()));
  GL_CALL(glBufferData(
    GL_COPY_WRITE_BUFFER,
    v.size() * sizeof(T),
    v.data(),
    usage
  ));
  GL_CALL(glBindBuffer(GL_COPY_WRITE_BUFFER, 0));
}


Buffer::Buffer()
  : _impl(new Buffer_impl)
  {}

Buffer::~Buffer() {
  delete _impl;
}

GLuint Buffer::name() const {
  return _impl->name();
}

template<typename T>
void Buffer::data(std::vector<T> const& data, GLenum usage) {
  _impl->data(data, usage);
}


template void Buffer::data<GLfloat>(std::vector<GLfloat> const&, GLenum);



} // namespace gl
