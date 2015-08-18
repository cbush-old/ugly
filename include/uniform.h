#ifndef OGL_UNIFORM_H
#define OGL_UNIFORM_H

#include "gl_type.h"
#include <vector>

namespace gl {

class uniform {
  public:
    uniform();

  public:
    virtual ~uniform() =0;

  public:
    operator int() { return _name; }

  private:
    GLint _name { 0 };

};

template<typename T>
class uniform1 : public uniform {
  public:
    ~uniform1() override {}

  public:
    void set(T const);

};

template<typename T>
class uniform2 : public uniform {
  public:
    ~uniform2() override {}

  public:
    void set(T const, T const);

};

template<typename T>
class uniform3 : public uniform {
  public:
    ~uniform3() override {}

  public:
    void set(T const, T const, T const);

};

template<typename T>
class uniform4 : public uniform {
  public:
    ~uniform4() override {};

  public:
    void set(T const, T const, T const, T const);

};

template<size_t Dimensions>
class uniform_vector : public uniform {
  public:
    ~uniform_vector() override {}

  public:
    void set(std::vector<float> const&);

};

} // namespace gl

#endif
