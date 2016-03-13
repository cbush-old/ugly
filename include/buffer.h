#ifndef UGLY_BUFFER_H
#define UGLY_BUFFER_H

#include "gl_type.h"
#include "generated_object.h"

#include <vector>
#include <array>
#include <type_traits>

namespace gl {


class Texture;


class Buffer : public GeneratedObject<glGenBuffers, glDeleteBuffers> {
  public:
    Buffer();

    template<typename T>
    Buffer(std::vector<T> const& container, GLenum usage, GLenum target = GL_COPY_WRITE_BUFFER) {
      data(container, usage, target);
    }

    template<typename T, size_t N>
    Buffer(std::array<T, N> const& container, GLenum usage, GLenum target = GL_COPY_WRITE_BUFFER) {
      data(container, usage, target);
    }


  public:
    ~Buffer();
    Buffer(Buffer const&) = delete;
    Buffer& operator=(Buffer const&) = delete;

  public:
    template<typename T>
    void data(std::vector<T> const& container, GLenum usage,
      GLenum target = GL_COPY_WRITE_BUFFER) {
      data(container.size() * sizeof(T), container.data(), usage, target);
    }

    template<typename T, size_t N>
    void data(std::array<T, N> const& container, GLenum usage,
      GLenum target = GL_COPY_WRITE_BUFFER) {
      data(N * sizeof(T), container.data(), usage, target);
    }
  
    /**
     * @brief allocate storage for a buffer of specified size with no data
     **/
    void data(GLsizei size, GLenum usage, GLenum target = GL_COPY_WRITE_BUFFER) {
      data(size, nullptr, usage, target);
    }


  public:
    template<class Container>
    void subdata(size_t offset, size_t count, Container const& container, GLenum target = GL_COPY_WRITE_BUFFER) {
      assert(count <= container.size());
      _subdata(offset, count * sizeof(typename Container::value_type), container.data(), target);
    }

    template<class Container>
    void subdata(size_t offset, Container const& container, GLenum target = GL_COPY_WRITE_BUFFER) {
      subdata(offset, container.size(), container, target);
    }

  public:
    void* map(GLenum target, GLenum access);
    bool unmap();
  

  public:
    void get(size_t offset, size_t size, void* data) const;
  
  
  public:
    // glTexBuffer
    void texture(Texture& texture, GLenum internal_format);


  private:
    void data(size_t size, void const* data, GLenum usage, GLenum target);
    void _subdata(size_t offset, size_t size, void const* data, GLenum target);


  private:
    GLenum _target;
    bool _mapped { false };

};



} // namespace gl

#endif
