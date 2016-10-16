#ifndef SIMPLE_GL_H
#define SIMPLE_GL_H

#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "simpleTextureI.hpp"

namespace simpleGL {

	GLFWwindow* createFullscreenWindow(const char* title, bool borderless);
	GLFWwindow* createWindowedWindow(const char* title, unsigned width, unsigned height, bool resizable, bool decorated);

	void startDrawThread();

	void joinDrawThread();

	SimpleTextureI* addTexture(std::string path);

	void removeTexture();

}

#endif
