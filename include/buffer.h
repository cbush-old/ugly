#ifndef UGLY_BUFFER_H
#define UGLY_BUFFER_H

#include "gl_type.h"
#include "generated_object.h"

#include <vector>
#include <array>

namespace gl {



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
    virtual ~Buffer();
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
    template<typename T>
    void subdata(size_t offset, std::vector<T> const& container) {
      subdata(offset, container.data(), container.size() * sizeof(T));
    }
  

  public:
    void* map(GLenum target, GLenum access);
    bool unmap();
  

  public:
    void get(size_t offset, size_t size, void* data) const;


  private:
    void data(size_t size, void const* data, GLenum usage, GLenum target);
    void subdata(size_t offset, size_t size, void const* data);


  private:
    GLenum _target;
    bool _mapped { false };

};



} // namespace gl

#endif
