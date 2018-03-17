#include "window.h"
#include "updatable.h"
#include "log.h"

#ifdef FPS_COUNTER
	#include <iostream>
#endif

namespace {

void errorCallback(int error, const char* description) {
	simpleGL::println(description);
}

}

namespace simpleGL {

BaseWindow* BaseWindow::current = nullptr;

BaseWindow::BaseWindow(const std::string& title, unsigned pWidth, unsigned pHeight, bool resizable, bool decorated, bool fullscreen) {
	if (current)	{
		println("error:GLFW:creating another window");
		return;
	}

	glfwSetErrorCallback(errorCallback);

	println("GLFW:load");
	if(!glfwInit()) {
		println("error:GLFW:failed to init");
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);

	width = pWidth ? pWidth : vidmode->width;
	height = pHeight ? pHeight : vidmode->height;

	glfwWindowHint(GLFW_RESIZABLE, resizable);
	glfwWindowHint(GLFW_DECORATED, decorated);
	glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);

	window = glfwCreateWindow(width, height, title.c_str(), (fullscreen ? monitor : nullptr), nullptr);

	if (!window) {
		println("error:GLFW:failed to create window");
		glfwTerminate();
		return;
	}

	glfwSetWindowSizeCallback(window, Window::onResize);

	glfwMakeContextCurrent(window);

	current = this;
}

BaseWindow::~BaseWindow() {
	if (window)
	{
		println("GLFW:unload");

		glfwTerminate();
	}

	current = nullptr;
}

void BaseWindow::onResize(GLFWwindow* window, int width, int height) {
	if (width == 0 || height == 0) {
		//window is now minimized (probably)
		return;
	}

	current->resize(width, height);
}

void BaseWindow::maximize() {
	glfwMaximizeWindow(window);
}


Window* Window::getCurrent() { return static_cast<Window*>(current); }

void Window::resize(unsigned pWidth, unsigned pHeight) {
	BaseWindow::resize(pWidth, pHeight);

	//properly resize camera?
	cameraInstance->resize(getWidth(), getHeight());
}

void Window::draw() {
	println("simpleGL:Window:start of draw cycle");

	deltaStopwatch.start();

	#ifdef FPS_COUNTER
		double fpsTime = 0.0;
		int frames = 0;
	#endif

	while (!glfwWindowShouldClose(current->getWindow())) {
		deltaTime = deltaStopwatch.lap();

		#ifdef FPS_COUNTER
			fpsTime += deltaTime;
			++frames;
			if (fpsTime >= 1.0) {
				std::cout << "fps: " << frames << "; spf: " << fpsTime/frames << ";" << std::endl;
				fpsTime -= 1.0;
				frames = 0;
			}
		#endif

		glfwPollEvents();

		glBindVertexArray(openGLContext->getVAO());

		globalUpdate();

		cameraInstance->draw();

		glBindVertexArray(0);

		glfwSwapBuffers(current->getWindow());
	}
}

}
