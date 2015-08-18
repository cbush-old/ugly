#ifndef OGL_VBO_H
#define OGL_VBO_H

#include <vector>
#include "gltype.h"

namespace ogl {

class VBO {
  public:
    VBO(std::vector<float> const&);
    ~VBO();

  public:
    VBO(VBO const&) = delete;
    VBO& operator=(VBO const&) = delete;

  public:
    void set_data(std::vector<float> const&);
    void use_attribute(attribute, int offset = 0) const;

  private:
    GLuint _id { 0 };

};

}

#endif
