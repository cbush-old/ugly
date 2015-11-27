#ifndef UGLY_GENERATED_OBJECT_H
#define UGLY_GENERATED_OBJECT_H

#include "gl_type.h"

namespace gl {

template<void(*glGenFunc)(GLsizei, GLuint*), void(*glDeleteFunc)(GLsizei, GLuint const*)>
class GeneratedObject {
  public:
    GeneratedObject();
    GeneratedObject(GLuint name);
    virtual ~GeneratedObject() =0;

  public:
    inline GLuint name() const { return _name; }

  private:
    GLuint _name;
    bool _owner { false };

};

} // namespace gl

#endif

