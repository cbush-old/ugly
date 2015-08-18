#ifndef UGLY_PIPELINE_H
#define UGLY_PIPELINE_H

#include "gl_type.h"
#include "program.h"

namespace gl {

class Pipeline {
  DECL_GL_OBJECT(Pipeline);

  public:
    /**
     * @brief Set the active Program object for the Pipeline.
     **/
    void set_active(Program&);

};

} // namespace gl

#endif
