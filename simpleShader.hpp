#ifndef SIMPLE_SHADER_H
#define SIMPLE_SHADER_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <initializer_list>

class SimpleShader {
private:
	GLuint shader;
	GLenum type;

public:
	SimpleShader() : shader(0), type(0) {}
	SimpleShader(GLuint sh, GLenum t) : shader(sh), type(t) {}

	GLuint getShader() const { return shader; }
	GLenum getType() const { return type; }

	void setUniformf(const char* location, std::initializer_list<GLfloat> list);
	void setUniformi(const char* location, std::initializer_list<GLint> list);
	void setUniformui(const char* location, std::initializer_list<GLuint> list);

	//TODO: matrix

};

#endif
