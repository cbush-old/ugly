#include "image.h"
#include <fstream>

namespace glx {

class ImageImpl {
  public:
    virtual GLenum format() const =0;
    virtual size_t size() const =0;
    virtual size_t width() const =0;
    virtual size_t height() const =0;
    virtual uint8_t const* data() const =0;
};



namespace {

ImageImpl* load_image(const char* path) {
  std::ifstream file (path);
  if (!file.good()) {
    throw gl::exception("bad file: %s", path);
  }
  return nullptr;
}


}

Image::Image(const char* path): _impl(load_image(path)) {
}

Image::Image(std::string const& path): _impl(load_image(path.c_str())) {
}

GLenum Image::format() const {
  return _impl->format();
}

size_t Image::size() const {
  return _impl->size();
}

size_t Image::width() const {
  return _impl->width();
}

size_t Image::height() const {
  return _impl->height();
}

uint8_t const* Image::data() const {
  return _impl->data();
}





}

