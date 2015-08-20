#ifndef UGLY_VERTEX_ARRAY_H
#define UGLY_VERTEX_ARRAY_H

#include "gl_type.h"

namespace gl {

class VertexArray {
  public:
    /**
     * @brief Bind the VertexArray to the Context
     **/
    void bind();

    /**
     * @brief Unbind the VertexArray
     **/
    void unbind();

};

} // namespace gl

#endif
