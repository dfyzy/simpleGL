#ifndef SIMPLE_WINDOW_H
#define SIMPLE_WINDOW_H

#include "color.h"

#include <string>

struct GLFWwindow;
struct GLFWmonitor;

namespace simpleGL {

class Window {
private:
	static Window* current;

	static bool init();
	static Window* load(const std::string& title, unsigned width, unsigned height, GLFWmonitor* monitor);

	GLFWwindow* window;

	unsigned width;
	unsigned height;

	Window(GLFWwindow* window, unsigned width, unsigned height) : window(window), width(width), height(height) { current = this; }
	~Window() { current = nullptr; }

public:
	static Window* getCurrent() { return current; }

	static Window* load(const std::string& title, unsigned width, unsigned height, bool resizable, bool decorated);
	static Window* loadFullscreen(const std::string& title, bool borderless);

	static void terminate();

	GLFWwindow* getWindow() const { return window; }

	unsigned getWidth() const { return width; }
	//setter

	unsigned getHeight() const { return height; }
	//setter

	void maximize();

	//keyboard callbacks

};

}

#endif
