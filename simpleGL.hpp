#ifndef SIMPLE_GL_H
#define SIMPLE_GL_H

#include "simpleText.hpp"

namespace simpleGL {

	/*
	 * Creates fullscreen window.
	 *
	 * 'title': title of the window.
	 * 'borderless': if true window will not be proper fullscreen.
	 *
	 * returns: GLFW handle of created window.
	 */
	GLFWwindow* createFullscreenWindow(const char* title, bool borderless, SimpleColor background);

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
	GLFWwindow* createWindowedWindow(const char* title, unsigned width, unsigned height, bool resizable, bool decorated, SimpleColor background);

	/*
	 * Returns width of the created window.
	 */
	unsigned getWindowWidth();

	/*
	 * Returns height of the created window.
	 */
	unsigned getWindowHeight();

	/*
	 *
	 */
	SimpleVector actualToScreen(SimpleVector sv);

	SimpleVector glfwToScreen(double x, double y);

	//microseconds
	double getDeltaTime();


	void setUpdate(void func());

	GLuint loadShaderSource(std::string source, GLenum type);

	GLuint loadShaderPath(std::string path, GLenum type);

	/*
	 *	Starts draw loop
	 */
	void draw();

	void setCameraPosition(SimpleVector position);

	void setCameraRotation(float rotation);

	/*
	 * Changes texture filtering on textures that will be loaded from this point.
	 * Default value GL_NEAREST.
	 *
	 * 'tf': can only be GL_LINEAR or GL_NEAREST.
	 */
	void setTextureFiltering(GLenum tf);

	void setOverlayShader(GLuint sh);

}

#endif
