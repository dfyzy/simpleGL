#ifndef SIMPLE_GL_H
#define SIMPLE_GL_H

#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "simpleTexture.hpp"

namespace simpleGL {

	/*
	 * Creates fullscreen window.
	 *
	 * 'title': title of the window.
	 * 'borderless': if true window will not be proper fullscreen.
	 *
	 * returns: GLFW handle of created window.
	 */
	GLFWwindow* createFullscreenWindow(const char* title, bool borderless);

	/*
	 * Creates windowed window.
	 *
	 * 'title': title of the window.
	 * 'width': width in screen coordinates.
	 * 'height': height in screen coordinates.
	 * 'resizable': if true user can resize the window.
	 * 'decorated': if window has any decorations(border, close widget).
	 *
	 * returns: GLFW handle of created window.
	 */
	GLFWwindow* createWindowedWindow(const char* title, unsigned width, unsigned height, bool resizable, bool decorated);

	/*
	 * Starts thread on which the opengl context will be initializated.
	 * Should call this function before loading textures.
	 */
	void startDrawThread();

	/*
	 * Waits for draw thread to finish. Do not call if glfwWindowShouldClose is false.
	 */
	void joinDrawThread();

	/*
	 * Loads texture from file into opengl texture object. Only works with png images(for now?).
	 *
	 * 'path': the path of the image file.
	 *
	 * returns: handle to texture object.
	 */
	SimpleTexture* loadTexture(std::string path);

}

#endif
