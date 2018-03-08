#ifndef SIMPLE_WINDOW_H
#define SIMPLE_WINDOW_H

#include <string>
#include <chrono>

#include "openGLContext.h"
#include "openALContext.h"

#include "globalInstance.h"
#include "camera.h"

struct GLFWwindow;
struct GLFWmonitor;

namespace simpleGL {

class Window;

class BaseWindow {
private:
	static void onResize(GLFWwindow* window, int width, int height);

	unsigned width;
	unsigned height;

	GLFWwindow* window = nullptr;

protected:
	static BaseWindow* current;

public:
	BaseWindow(const std::string& title, unsigned width, unsigned height, bool resizable, bool decorated, bool fullscreen);
	~BaseWindow();

	GLFWwindow* getWindow() const { return window; }

	unsigned getWidth() const { return width; }
	unsigned getHeight() const { return height; }

	virtual void resize(unsigned pWidth, unsigned pHeight) {
		width = pWidth;
		height = pHeight;
	}

	void maximize();

};

class Window : public BaseWindow {
private:
	typedef std::chrono::high_resolution_clock Clock;

	Clock::time_point previous;
	double deltaTime = 0;

	OpenGLContext openGLContext;
	OpenALContext openALContext;

	GlobalInstance<Camera> cameraInstance;

	Window(const std::string& title, unsigned width, unsigned height, bool resizable, bool decorated, bool fullscreen)
		: BaseWindow(title, width, height, resizable, decorated, fullscreen), cameraInstance(getWidth(), getHeight()) {}

public:
	static Window* getCurrent();

	Window(const std::string& title, unsigned width, unsigned height, bool resizable, bool decorated)
		: Window(title, width, height, resizable, decorated, false) {}

	//fullscreen
	Window(const std::string& title, bool borderless)
		: Window(title, 0, 0, false, false, borderless) {}

	void resize(unsigned pWidth, unsigned pHeight) override;

	double getDeltaTime() {
		return deltaTime;
	}

	//keyboard callbacks

	void draw();

};

}

#endif
