#include "window.h"
#include "simpleGL.h"
#include "glfw.h"
#include "util.h"

namespace {

void errorCallback(int error, const char* description) {
	simpleGL::util::println(description);
}

}

namespace simpleGL {

Window* Window::current = nullptr;

bool Window::init() {
	glfwSetErrorCallback(errorCallback);

	util::println("GLFW:load");
	if(!glfwInit()) {
		util::println("error:GLFW:failed to init");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	return true;
}

Window* Window::load(const std::string& title, unsigned width, unsigned height, bool resizable, bool decorated) {
	if (Window::getCurrent())	{
		util::println("error:GLFW:creating another window");
		return nullptr;
	}

	if (!init()) return nullptr;

	glfwWindowHint(GLFW_RESIZABLE, resizable);
	glfwWindowHint(GLFW_DECORATED, decorated);

	return load(title, width, height, nullptr);
}

Window* Window::loadFullscreen(const std::string& title, bool borderless) {
	if (Window::getCurrent())	{
		util::println("error:GLFW:creating another window");
		return nullptr;
	}

	if (!init()) return nullptr;

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();

	const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_DECORATED, false);
	glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);

	if (borderless)	monitor = nullptr;

	return load(title, vidmode->width, vidmode->height, monitor);
}

Window* Window::load(const std::string& title, unsigned width, unsigned height, GLFWmonitor* monitor) {
	GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), monitor, nullptr);

	if (!window) {
		util::println("error:GLFW:failed to create window");
		glfwTerminate();
		return nullptr;
	}

	Window* result = new Window(window, width, height);

	glfwMakeContextCurrent(window);

	loadGLContext();

	return result;
}

void Window::terminate() {
	if (current)	delete current;

	glfwTerminate();
}

void Window::maximize() {
	glfwMaximizeWindow(window);
}

}
