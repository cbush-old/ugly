#include "gl_type.h"
#include "buffer.h"
#include "framebuffer.h"
#include "texture.h"
#include "renderbuffer.h"
#include "texture_unit.h"
#include "vertex_array.h"
#include "program.h"

namespace gl {


template<typename T, void(*BindFunction)(GLenum, GLuint)>
Bindguard<T, BindFunction>::Bindguard(GLenum target, T const& object)
  : _target(target) {
  GL_CALL(BindFunction(_target, object.name()));
}

template<typename T, void(*BindFunction)(GLenum, GLuint)>
Bindguard<T, BindFunction>::~Bindguard() {
  GL_CALL(BindFunction(_target, 0));
}


template<typename T, void(*BindFunction)(GLuint), GLuint Default>
NoTargetBindguard<T, BindFunction, Default>::NoTargetBindguard(T const& object) {
  GL_CALL(BindFunction(object.name()));
}

template<typename T, void(*BindFunction)(GLuint), GLuint Default>
NoTargetBindguard<T, BindFunction, Default>::~NoTargetBindguard() {
  GL_CALL(BindFunction(Default));
}


template<>
NoTargetBindguard<TextureUnit, glActiveTexture, GL_TEXTURE0>::NoTargetBindguard(
  TextureUnit const& object) {
  GL_CALL(glActiveTexture(GL_TEXTURE0 + object.unit()));
}


// Instantiations
template class Bindguard<Buffer, glBindBuffer>;
template class Bindguard<Texture, glBindTexture>;
template class Bindguard<Framebuffer, glBindFramebuffer>;
template class Bindguard<Renderbuffer, glBindRenderbuffer>;

template class NoTargetBindguard<TextureUnit, glActiveTexture, GL_TEXTURE0>;
template class NoTargetBindguard<VertexArray, glBindVertexArray>;
template class NoTargetBindguard<Program, glUseProgram>;


}
