#include <windows.h>

#include "simpleGL.hpp"
#include "simpleUtil.hpp"

namespace simpleUtil {
	boost::thread::id currentId;

	bool isCurrentThread() {
		return currentId == boost::this_thread::get_id();
	}
}

namespace simpleGL {
	GLFWwindow* window = nullptr;
	unsigned windowWidth, windowHeight;
	SimpleColor backgroundColor;

	GLuint vao;

	void errorCallback(int error, const char* description) {
		simpleUtil::print(description);
	}

	inline void init() {
		glfwSetErrorCallback(errorCallback);

		if(!glfwInit()) {
			simpleUtil::print("Failed to initialize GLFW");
			exit(EXIT_FAILURE);
		} else simpleUtil::print("GLFW initialized");
	}

	inline void setCreationHints(bool resizable, bool decorated) {
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_SAMPLES, 4);

		glfwWindowHint(GLFW_RESIZABLE, resizable);
		glfwWindowHint(GLFW_DECORATED, decorated);
	}

	inline void createWindow(const char* title, GLFWmonitor* monitor) {
		window = glfwCreateWindow(windowWidth, windowHeight, title, monitor, nullptr);

		if (!window) {
			simpleUtil::print("Failed to create GLFW window");
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
	}

	GLFWwindow* createFullscreenWindow(const char* title, bool borderless) {
		if (window)	{
			simpleUtil::print("Why would you need more than one window?");
			return nullptr;
		}

		init();

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();

		const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);

		setCreationHints(false, false);

		glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);

		if (borderless)	monitor = nullptr;

		windowWidth = vidmode->width;
		windowHeight = vidmode->height;

		createWindow(title, monitor);
		return window;
	}

	GLFWwindow* createWindowedWindow(const char* title, unsigned width, unsigned height, bool resizable, bool decorated) {
		if (window)	{
			simpleUtil::print("Why would you need more than one window?");
			return nullptr;
		}

		init();

		setCreationHints(resizable, decorated);

		windowWidth = width;
		windowHeight = height;

		createWindow(title, nullptr);
		return window;
	}

	unsigned getWindowWidth() {
		return windowWidth;
	}

	unsigned getWindowHeight() {
		return windowHeight;
	}

	float toScreenCoord(float f)  {
		return 2.0f*f/windowHeight;
	}

	void draw() {
		simpleUtil::currentId = boost::this_thread::get_id();

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

		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glEnable(GL_STENCIL_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		simpleUtil::initShaders(((float) windowWidth) / windowWidth);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		simpleUtil::initBuffers();

		glActiveTexture(GL_TEXTURE0);

		#if defined FPS_COUNTER && defined DEBUG
			double lastFPSTime = glfwGetTime();
			int frames = 0;
		#endif
		while (!glfwWindowShouldClose(window)) {
			#if defined FPS_COUNTER && defined DEBUG
				double currentTime = glfwGetTime();
				frames++;
				if (currentTime - lastFPSTime >= 1.0) {
					std::cout << "fps: " << frames << "; spf: " << 1.0/frames << ";" << std::endl;
					frames = 0;
					lastFPSTime = currentTime;
				}
			#endif

			simpleUtil::checkTextures();
			simpleUtil::checkSprites();
			simpleUtil::checkShaders();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			glBindVertexArray(vao);

			simpleUtil::drawSprites();

			glBindVertexArray(0);

			glfwSwapBuffers(window);
		}

		glfwTerminate();//raii
	}

	boost::thread startDrawThread(SimpleColor background) {
		backgroundColor = background;
		return boost::thread(draw);
	}

}
