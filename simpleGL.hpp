#ifndef SIMPLE_GL_H
#define SIMPLE_GL_H

#include <string>

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
	 * Returns width of the created window.
	 */
	unsigned getWindowWidth();

	/*
	 * Returns height of the created window.
	 */
	unsigned getWindowHeight();

	/*
	 * Transforms float to screen coord.
	 */
	float toScreenCoord(float f);

	/*
	 * Starts thread on which the opengl context will be initializated.
	 * Should call this function before doing anything opengl related.
	 *
	 * returns: thread handle of the draw thread. Join the thread while closing the window to terminate GLFW properly.
	 */
	boost::thread startDrawThread(SimpleColor background);

	/*
	 * Loads texture from file into opengl texture object. Only works with png images(for now?).
	 *
	 * 'path': the path of the image file.
	 *
	 * returns: handle to texture object.
	 */
	SimpleTexture* loadTexture(std::string path);

	/*
	 * Texture object without texture with width and height equal 100.
	 */
	SimpleTexture* getEmptyTexture();

	/*
	 * Changes texture filtering on all currently loaded textures and textures that will be loaded from this point.
	 * Default value GL_NEAREST.
	 *
	 * 'tf': can only be GL_LINEAR or GL_NEAREST.
	 */
	void changeTextureFiltering(GLenum tf);

	/*
	 * Loads, compiles, links and validates shader program of type ptype.
	 *
	 * 'path': path to shader file.
	 * 'ptype': type of shader program. Must be only GL_VERTEX_SHADER or GL_FRAGMENT_SHADER.
	 *
	 * returns: handle of created program. If 0 shader did not load.
	 */
	SimpleShader loadShader(std::string path, GLenum ptype);

}

#endif
