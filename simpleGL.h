#ifndef SIMPLE_GL_H
#define SIMPLE_GL_H

#include <functional>

#include "shader.h"
#include "sprite.h"
#include "camera.h"
#include "speaker.h"

namespace simpleGL {

/*
 * Creates fullscreen window.
 *
 * 'title': title of the window.
 * 'borderless': if true window will not be proper fullscreen.
 *
 * returns: GLFW handle of created window.
 */
GLFWwindow* loadFullscreenWindow(std::string title, bool borderless, Color background);

/*
 * Creates a window.
 *
 * 'title': title of the window.
 * 'width': width in screen coordinates.
 * 'height': height in screen coordinates.
 * 'resizable': if true user can resize the window.
 * 'decorated': if window has any decorations(border, close widget).
 *
 * returns: GLFW handle of created window.
 */
GLFWwindow* loadWindow(std::string title, unsigned width, unsigned height, bool resizable, bool decorated, Color background);

GLFWwindow* getWindow();

/*
 * Returns width of the created window.
 */
unsigned getWindowWidth();

/*
 * Returns height of the created window.
 */
unsigned getWindowHeight();

Color getBackground();

double getDeltaTime();

void setUpdate(std::function<void()> func);

/*
 *	Checks if this OpenGL context has an extension 'ext'
 */
bool hasExtension(std::string ext);

/*
 *	Starts draw loop
 */
void draw();

}

#endif
