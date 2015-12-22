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
    GLenum format() const;

  public:
    size_t width() const;
    size_t height() const;

  public:
    size_t size() const;
    uint8_t const* data() const;
  
  private:
    std::shared_ptr<class ImageImpl> _impl;

};


};


#endif
