#include <windows.h>

#include <chrono>
#include <set>

#include "simpleGL.h"
#include "util.h"

#ifdef FPS_COUNTER
	#include <iostream>
#endif

namespace {

typedef std::chrono::high_resolution_clock Clock;

GLFWwindow* window = nullptr;
unsigned windowWidth = 0, windowHeight = 0;

simpleGL::Color backColor;

GLuint vao;

Clock::time_point previous;
double deltaTime = 0;

std::function<void()> update;

std::list<std::function<void()>> utilPreUpdates;
std::list<std::function<void()>> utilPostUpdates;

}

namespace simpleGL {

void errorCallback(int error, const char* description) {
	util::println(description);
}

inline bool initGLFW(bool resizable, bool decorated) {
	glfwSetErrorCallback(errorCallback);

	util::println("GLFW:load");
	if(!glfwInit()) {
		util::println("error:GLFW:failed to init");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, resizable);
	glfwWindowHint(GLFW_DECORATED, decorated);

	return true;
}

inline void loadWindow(std::string title, GLFWmonitor* monitor, Color background) {
	backColor = background;

	window = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), monitor, nullptr);

	if (!window) {
		util::println("error:GLFW:failed to create window");
		glfwTerminate();
		window = nullptr;
		return;
	}

	glfwMakeContextCurrent(window);

	util::println("GLEW:load");
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK) {
		util::println("error:GLEW:failed to init");
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

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	ALCdevice* device = alcOpenDevice(NULL);

	if (!device) {
		util::println("error:OpenAL:failed to create device");
		return;
	}

	ALCcontext* context = alcCreateContext(device, NULL);
	if (!context) {
		util::println("error:OpenAL:failed to create context");
		return;
	}

	alcMakeContextCurrent(context);
}

GLFWwindow* loadFullscreenWindow(std::string title, bool borderless, Color background) {
	if (window)	{
		util::println("error:GLFW:creating another window");
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

	loadWindow(title, monitor, background);
	return window;
}

GLFWwindow* loadWindow(std::string title, unsigned width, unsigned height, bool resizable, bool decorated, Color background) {
	if (window)	{
		util::println("error:GLFW:creating another window");
		return nullptr;
	}

	if (!initGLFW(resizable, decorated)) return nullptr;

	windowWidth = width;
	windowHeight = height;

	loadWindow(title, nullptr, background);
	return window;
}

GLFWwindow* getWindow() {
	return window;
}

unsigned getWindowWidth() {
	return windowWidth;
}

unsigned getWindowHeight() {
	return windowHeight;
}

Color getBackground() {
	return backColor;
}

double getDeltaTime() {
	return deltaTime;
}

void setUpdate(std::function<void()> func) {
	update = func;
}

void util::addPreUpdate(std::function<void()> updt) {
	utilPreUpdates.push_back(updt);
}

void util::addPostUpdate(std::function<void()> updt) {
	utilPostUpdates.push_back(updt);
}

void draw() {
	util::println("simpleGL:start of draw cycle");

	if (window == nullptr) {
		util::println("error:simpleGL:no current OpenGL context");
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

		glBindVertexArray(vao);

		for (auto upu : utilPreUpdates)
			upu();

		if (update)	update();

		for (auto uu : utilPostUpdates)
			uu();

		Camera::getInstance()->draw();

		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	util::println("GLFW:unload");

	glfwTerminate();
	window = nullptr;
}

}
