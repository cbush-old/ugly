#ifndef UGLY_EXT_IMAGE_H
#define UGLY_EXT_IMAGE_H

#include "ugly.h"
#include <string>

namespace glx {

class Image {
  public:
    Image(const char* path);
    Image(std::string const& path);

  public:
    gl::ImageDesc2D const& desc() const;

  private:
    std::shared_ptr<gl::ImageDesc2D> _desc;

};


};


#endif
