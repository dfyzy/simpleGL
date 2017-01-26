#ifndef SIMPLE_GL_H
#define SIMPLE_GL_H

#include "text.hpp"
#include "light.hpp"

namespace simpleGL {

/*
 * Creates fullscreen window.
 *
 * 'title': title of the window.
 * 'borderless': if true window will not be proper fullscreen.
 *
 * returns: GLFW handle of created window.
 */
GLFWwindow* createFullscreenWindow(const char* title, bool borderless, Color background);

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
GLFWwindow* createWindowedWindow(const char* title, unsigned width, unsigned height, bool resizable, bool decorated, Color background);

/*
 * Returns width of the created window.
 */
unsigned getWindowWidth();

/*
 * Returns height of the created window.
 */
unsigned getWindowHeight();

Vector glfwToScreen(double x, double y);

//microseconds
double getDeltaTime();


void setUpdate(void func());

GLuint loadShaderSource(std::string source, GLenum type);

GLuint loadShaderPath(std::string path, GLenum type);

/*
 *	Starts draw loop
 */
void draw();

void setCameraPosition(Vector position);
void setCameraRotation(float rotation);
void setCameraScale(float scale);

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
