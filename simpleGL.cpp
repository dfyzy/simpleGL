#include <windows.h>

#include <chrono>

#include "simpleGL.hpp"
#include "simpleUtil.hpp"

#ifdef FPS_COUNTER
	#include <iostream>
#endif

namespace simpleGL {
	typedef void (*Callback)();
	typedef std::chrono::high_resolution_clock Clock;

	GLFWwindow* window = nullptr;
	unsigned windowWidth, windowHeight;

	GLuint vao;

	void defaultUpdate() {}
	Callback update = defaultUpdate;

	Clock::time_point previous;
	double deltaTime = 0;

	void errorCallback(int error, const char* description) {
		simpleUtil::print(description);
	}

	inline void initGLFW(bool resizable, bool decorated) {
		glfwSetErrorCallback(errorCallback);

		if(!glfwInit()) {
			simpleUtil::print("Failed to initialize GLFW");
			exit(EXIT_FAILURE);
		} else simpleUtil::print("GLFW initialized");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_RESIZABLE, resizable);
		glfwWindowHint(GLFW_DECORATED, decorated);
	}

	inline void createWindow(const char* title, GLFWmonitor* monitor, SimpleColor background) {
		window = glfwCreateWindow(windowWidth, windowHeight, title, monitor, nullptr);

		if (!window) {
			simpleUtil::print("Failed to create GLFW window");
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		glfwMakeContextCurrent(window);

		// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
		glewExperimental = GL_TRUE;
		// Initialize GLEW to setup the OpenGL Function pointers
		if (glewInit() != GLEW_OK) {
			simpleUtil::print("Failed to initialize GLEW");
			glfwTerminate();
			exit(EXIT_FAILURE);
		} else	simpleUtil::print("GLEW initialized");

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

		glViewport(0, 0, windowWidth, windowHeight);

		glClearColor(background.r, background.g, background.b, background.a);

		glEnable(GL_STENCIL_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		simpleUtil::initShaders();

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		simpleUtil::initBuffers();
		simpleUtil::initFbos();
		simpleUtil::initFonts();
	}

	GLFWwindow* createFullscreenWindow(const char* title, bool borderless, SimpleColor background) {
		if (window)	{
			simpleUtil::print("Why would you need more than one window?");
			return nullptr;
		}

		initGLFW(false, false);

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

	GLFWwindow* createWindowedWindow(const char* title, unsigned width, unsigned height, bool resizable, bool decorated, SimpleColor background) {
		if (window)	{
			simpleUtil::print("Why would you need more than one window?");
			return nullptr;
		}

		initGLFW(resizable, decorated);

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

	SimpleVector actualToScreen(SimpleVector f) {
		return f*(2.0f/windowHeight);
	}

	SimpleVector glfwToScreen(double x, double y) {
		return SimpleVector(x/windowWidth, 1 - y/windowHeight);
	}

	void setUpdate(void func()) {
		update = func;
	}

	double getDeltaTime() {
		return deltaTime;
	}

	void draw() {
		previous = Clock::now();

		#ifdef FPS_COUNTER
			double fpsTime = 0;
			int frames = 0;
		#endif
		while (!glfwWindowShouldClose(window)) {
			Clock::time_point now = Clock::now();
			deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - previous).count();
			previous = now;

			#ifdef FPS_COUNTER
				fpsTime += deltaTime;
				frames++;
				if (fpsTime >= 1000000) {
					std::cout << "fps: " << frames << "; spf: " << fpsTime/1000000/frames << ";" << std::endl;
					fpsTime = 0;
					frames = 0;
				}
			#endif

			glfwPollEvents();

			update();

			glBindVertexArray(vao);

			simpleUtil::drawSprites();

			glBindVertexArray(0);

			glfwSwapBuffers(window);
		}

		simpleUtil::closeFonts();
		glfwTerminate();//raii
	}

}
