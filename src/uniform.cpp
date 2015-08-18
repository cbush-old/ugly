#include "uniform.h"
#include "gl_type.h"

#include <functional>

#define glUniformNT(N, T) glUniform##N##T
#define glUniformNTv(N, T) glUniformNT(N, T)v
#define glUniformMatrixN(NxN) glUniformMatrix##NxN##fv

namespace gl {

template<typename T> using Uniform1Func = std::function<void(GLint, T)>;
template<typename T> using Uniform2Func = std::function<void(GLint, T, T)>;
template<typename T> using Uniform3Func = std::function<void(GLint, T, T, T)>;
template<typename T> using Uniform4Func = std::function<void(GLint, T, T, T, T)>;
template<typename T> using UniformVFunc = std::function<void(GLint, GLsizei, GLfloat const*)>;
using UniformMatrixFunc = std::function<void(GLint, GLsizei, GLboolean, GLfloat const*)>;


}

