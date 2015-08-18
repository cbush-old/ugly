#ifndef CONTEXT_H
#define CONTEXT_H

#include "gl_type.h"

namespace ogl {

struct PackInfo {
  int alignment;
  int image_height;
  int row_length;
  int skip_images;
  int skip_pixels;
  int skip_rows;
  bool lsb_first;
  bool swap_bytes;
};

class Context {
  public:
    Context(void*);
    ~Context();


  public:
    void bind(Renderbuffer&);
    void unbind_renderbuffer();
    


  public: // BLENDING
    bool blend_enabled() const;
    color get_blend_color() const;
    color get_color_clear_value() const;
    bool get_color_logic_op() const;
    color get_color_writemask() const;

  public: // OPENGL VERSION INFO
    int get_major_version() const;
    int get_minor_version() const;

    template<typename T>
    T get(GLenum) const;

    Buffer* get_dispatch_indirect_buffer_binding() const;

    int get_debug_group_stack_depth() const;
    int get_context_flags() const;

    bool cull_face_enabled() const;
    Program* get_current_program() const;

    float get_depth_clear_value() const;
    GLenum get_depth_func() const;
    range get_depth_range() const;
    bool get_depth_test() const;
    bool get_depth_writemask() const;
    bool get_dither() const;
    bool get_doublebuffer() const;
    GLenum get_draw_buffer(uint32_t index) const;
    
    Framebuffer* get_draw_framebuffer() const;
    Framebuffer* get_read_framebuffer() const;
    Buffer* get_element_array_buffer() const;
    Buffer* get_pixel_pack_buffer() const;
    Buffer* get_pixel_unpack_buffer() const;
    Pipeline* get_program_pipeline() const;
    Renderbuffer* get_renderbuffer() const;
    Texture* get_active_texture() const;
    Texture* get_texture(GLenum) const;
    Buffer* get_texture_buffer() const;
    Buffer* get_transform_feedback_buffer() const;
    Buffer* get_uniform_buffer() const;
    VertexArray* get_vertex_array() const;
    Viewport* get_viewport() const;

    int64_t get_timestamp() const;
    GLenum get_fragment_shader_derivative_hint() const;
    GLenum get_implementation_color_read_format() const;
    GLenum get_implementation_color_read_type() const;
    bool get_line_smooth() const;
    GLenum get_line_smooth_hint() const;
    float get_line_width() const;
    GLenum get_logic_op_mode() const;

  private:
    void *_handle;
};


}


#endif
