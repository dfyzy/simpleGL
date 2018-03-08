#ifndef SIMPLE_OPENGL_CONTEXT_H
#define SIMPLE_OPENGL_CONTEXT_H

#include "glfw.h"

namespace simpleGL {

class OpenGLContext {
private:
	GLuint vao;

public:
	OpenGLContext();
	~OpenGLContext();

	GLuint getVAO() const { return vao; }

};

}

#endif
