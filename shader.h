#ifndef SIMPLE_SHADER_H
#define SIMPLE_SHADER_H

#include <string>

#include "glfw.h"

namespace simpleGL {

GLuint loadShaderSource(std::string source, GLenum type);

GLuint loadShaderPath(std::string path, GLenum type);

}

#endif
