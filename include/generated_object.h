#ifndef UGLY_GENERATED_OBJECT_H

#include "gl_type.h"

namespace gl {

template<void(*glGenFunc)(GLsizei, GLuint*), void(*glDeleteFunc)(GLsizei, GLuint const*)>
class GeneratedObject {
  public:
    GeneratedObject();
    GeneratedObject(GLuint name);
    virtual ~GeneratedObject() =0;

  protected:
    GLuint _name;

};

} // namespace gl

#endif

