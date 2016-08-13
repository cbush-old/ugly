#include "image.h"
#include <fstream>
#include <vector>
#include <functional>

#include <png.h>

namespace glx {

struct ImageImpl : public gl::ImageDesc2D {
  ImageImpl(): pixels(new std::vector<uint32_t>) {}
  std::shared_ptr<std::vector<uint32_t>> pixels;
};


namespace {

ImageImpl* load_png(const char* path) {
  
  FILE *fp = fopen(path, "rb");
  
  if (!fp) {
    throw gl::exception("file error: %s", path);
  }

  png_structp png_ptr;
  png_infop info_ptr;
  png_byte header[8];

  fread(header, 1, 8, fp);

  if (png_sig_cmp(header, 0, 8)) {
    fclose(fp);
    return nullptr;
  }

  fseek(fp, 0, SEEK_SET);

  if (!(png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL))) {
    fclose(fp);
    throw gl::exception("png load error: couldn't allocate read struct for %s", path);
  }

  if (!(info_ptr = png_create_info_struct(png_ptr))) {
    fclose(fp);
    throw gl::exception("png load error: couldn't allocate info struct for %s", path);
  }

  png_init_io(png_ptr, fp);
  png_read_png(png_ptr, info_ptr, 0, NULL);
  fclose(fp);

  ImageImpl *impl = new ImageImpl;
  impl->height = png_get_image_height(png_ptr, info_ptr);
  impl->width = png_get_image_width(png_ptr, info_ptr);

  png_bytep *row_pointers = png_get_rows(png_ptr, info_ptr);

  impl->pixels->resize(impl->width * impl->height);

  for (size_t i = 0, k = 0; i < impl->height; ++i) {
    for (size_t j = 0; j < impl->width * sizeof(uint32_t); /* control in loop */) {
      uint32_t r, g, b, a;
      r = row_pointers[i][j++] << 24;
      g = row_pointers[i][j++] << 16;
      b = row_pointers[i][j++] << 8;
      a = row_pointers[i][j++];
      (*impl->pixels)[k++] = r | g | b | a;
    }
  }

  png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
  impl->data = impl->pixels->data();
  impl->format = GL_RGBA;
  impl->type = GL_UNSIGNED_INT_8_8_8_8;

  return impl;
}


ImageImpl* load_image(const char* path) {
  static const std::vector<std::function<ImageImpl*(const char*)>> formats {
    load_png,
  };

  for (auto const& format : formats) {
    ImageImpl *image (format(path));
    if (image) {
      return image;
    }
  }

  throw gl::exception("couldn't load image: %s", path);

}


}

Image::Image(const char* path): _desc(load_image(path)) {
}

Image::Image(std::string const& path): _desc(load_image(path.c_str())) {
}

gl::ImageDesc2D const& Image::desc() const {
  return *_desc;
}




}

