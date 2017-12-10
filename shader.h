/* Shader stuff
*/

#ifndef SIMPLE_SHADER_H
#define SIMPLE_SHADER_H

#include <string>

#include "glfw.h"

namespace simpleGL {

GLuint getDefaultVertexShader();
GLuint getDefaultFragmentShader(bool empty);

GLuint loadShaderSource(const std::string& source, GLenum type);
GLuint loadShaderPath(const std::string& path, GLenum type);

void useShaders(GLuint vertex, GLuint fragment);

}

#endif
