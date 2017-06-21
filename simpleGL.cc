#include <windows.h>

#include <chrono>

#include "simpleGL.h"
#include "util.h"

#ifdef FPS_COUNTER
	#include <iostream>
#endif

namespace {

typedef std::chrono::high_resolution_clock Clock;

GLFWwindow* window = nullptr;
unsigned windowWidth, windowHeight;

simpleGL::Color clearColor;

GLuint vao;

Clock::time_point previous;
double deltaTime = 0;

std::function<void()> update;

std::list<std::function<void()>> utilPreUpdates;
std::list<std::function<void()>> utilUpdates;

}

namespace simpleGL {

void errorCallback(int error, const char* description) {
	util::print(description);
}

inline bool initGLFW(bool resizable, bool decorated) {
	glfwSetErrorCallback(errorCallback);

	util::print("GLFW:load");
	if(!glfwInit()) {
		util::print("error:GLFW:failed to init");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, resizable);
	glfwWindowHint(GLFW_DECORATED, decorated);

	return true;
}

inline void createWindow(const char* title, GLFWmonitor* monitor, Color background) {
	clearColor = background;

	window = glfwCreateWindow(windowWidth, windowHeight, title, monitor, nullptr);

	if (!window) {
		util::print("error:GLFW:failed to create window");
		glfwTerminate();
		window = nullptr;
		return;
	}

	glfwMakeContextCurrent(window);

	util::print("GLEW:load");
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK) {
		util::print("error:GLEW:failed to init");
		glfwTerminate();
		window = nullptr;
		return;
	}

	#ifdef _WIN32
		// Turn on vertical screen sync if on Windows.
		typedef BOOL (WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int interval);
		PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
		if(wglSwapIntervalEXT)
			wglSwapIntervalEXT(1);
	#else
		glfwSwapInterval(1);
	#endif

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_STENCIL_TEST);

	util::initShaders();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	util::initDataBuffers();
}

GLFWwindow* createFullscreenWindow(const char* title, bool borderless, Color background) {
	if (window)	{
		util::print("error:GLFW:creating another window");
		return nullptr;
	}

	if (!initGLFW(false, false)) return nullptr;

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();

	const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);

	if (borderless)	monitor = nullptr;

	windowWidth = vidmode->width;
	windowHeight = vidmode->height;

	createWindow(title, monitor, background);
	return window;
}

GLFWwindow* createWindow(const char* title, unsigned width, unsigned height, bool resizable, bool decorated, Color background) {
	if (window)	{
		util::print("error:GLFW:creating another window");
		return nullptr;
	}

	if (!initGLFW(resizable, decorated)) return nullptr;

	windowWidth = width;
	windowHeight = height;

	createWindow(title, nullptr, background);
	return window;
}

unsigned getWindowWidth() {
	return windowWidth;
}

unsigned getWindowHeight() {
	return windowHeight;
}

double getDeltaTime() {
	return deltaTime;
}

GLFWwindow* util::getWindow() {
	return window;
}

void setUpdate(std::function<void()> func) {
	update = func;
}

void util::addPreUpdate(std::function<void()> updt) {
	utilPreUpdates.push_back(updt);
}

void util::addUpdate(std::function<void()> updt) {
	utilUpdates.push_back(updt);
}

void draw() {
	util::print("simpleGL:start of draw cycle");

	if (window == nullptr) {
		util::print("error:simpleGL:no current OpenGL context");
		return;
	}

	previous = Clock::now();

	#ifdef FPS_COUNTER
		double fpsTime = 0;
		int frames = 0;
	#endif
	while (!glfwWindowShouldClose(window)) {
		Clock::time_point now = Clock::now();
		deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - previous).count() * 0.000001;
		previous = now;

		#ifdef FPS_COUNTER
			fpsTime += deltaTime;
			frames++;
			if (fpsTime >= 1) {
				std::cout << "fps: " << frames << "; spf: " << fpsTime/frames << ";" << std::endl;
				fpsTime = 0;
				frames = 0;
			}
		#endif

		glfwPollEvents();

		for (auto upu : utilPreUpdates)
			upu();

		if (update)	update();

		for (auto uu : utilUpdates)
			uu();

		util::bindSprites();

		glBindVertexArray(vao);

		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

		util::drawSprites();

		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	util::print("GLFW:unload");

	glfwTerminate();
	window = nullptr;
}

}
