#include <fstream>
#include <boost/thread.hpp>
#include <iostream>//TODO: logs

#include <windows.h>

#include "simpleGL.hpp"
#include "simpleUtil.hpp"

const int SHADER_ATTRIB_COUNT = 4;

GLFWwindow* window = nullptr;
unsigned windowWidth, windowHeight;

GLuint vao;
GLuint vbos[SHADER_ATTRIB_COUNT];

boost::thread thread;

void errorCallback(int error, const char* description) {
	std::cout << "GLFW Error (" << error << "): " << description << std::endl;
}

inline void init() {
	glfwSetErrorCallback(errorCallback);

	if(!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		exit(EXIT_FAILURE);
	} else std::cout << "GLFW initialized" << std::endl;
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
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
}

GLFWwindow* simpleGL::createFullscreenWindow(const char* title, bool borderless) {
	if (window)	{
		std::cout << "Why would you need more than one window?" << std::endl;
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

GLFWwindow* simpleGL::createWindowedWindow(const char* title, unsigned width, unsigned height, bool resizable, bool decorated) {
	if (window)	{
		std::cout << "Why would you need more than one window?" << std::endl;
		return nullptr;
	}

	init();

	setCreationHints(resizable, decorated);

	windowWidth = width;
	windowHeight = height;

	createWindow(title, nullptr);
	return window;
}


void printOutProgInfoLog(GLuint program) {
	GLint length;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

	std::unique_ptr<GLchar> infoLog(new GLchar[length + 1]);
	glGetProgramInfoLog(program, length, nullptr, infoLog.get());

	std::cout << "Program #" << program << " infolog: " << infoLog.get() << std::endl;
}

const int MAX_PATH_CHARS = 64;
GLuint loadShader(std::string filename, GLenum type) {
	std::string path = "shaders\\" + filename;

	std::ifstream file(path);

	std::string shaderString;

	std::string line;
	while (getline(file, line)) {
		shaderString += line;
		shaderString.push_back('\n');
	}
	file.close();

	const GLchar* shaderChars = shaderString.c_str();

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderChars, nullptr);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (!status) {
		GLint length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

		std::unique_ptr<GLchar> infoLog(new GLchar[length + 1]);
		glGetShaderInfoLog(shader, length, nullptr, infoLog.get());

		std::cout << "Compilation error in shader #" << type << ": " << infoLog.get() << std::endl;
	} else	std::cout << "Compiled shader #" << type << std::endl;

	return shader;
}

void programStuff() {
	GLuint program = glCreateProgram();

	GLuint vertexShader = loadShader("simpleV.glsl", GL_VERTEX_SHADER);
	glAttachShader(program, vertexShader);

	GLuint fragmentShader = loadShader("simpleF.glsl", GL_FRAGMENT_SHADER);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status) {
		printOutProgInfoLog(program);
		glfwTerminate();
		exit(EXIT_FAILURE);
	} else	std::cout << "Program #" << program << " linked" << std::endl;

	glValidateProgram(program);
	glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
	if (!status) {
		printOutProgInfoLog(program);
		glfwTerminate();
		exit(EXIT_FAILURE);
	} else	std::cout << "Program #" << program << " validated" << std::endl;

	glUseProgram(program);

	glUniform1f(glGetUniformLocation(program, "rAspect"), ((float) windowHeight) / windowWidth);
}

inline void setAttrib(int id, int size, GLenum type) {
	glBindBuffer(GL_ARRAY_BUFFER, vbos[id]);

	glVertexAttribPointer(id + 1, size, type, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(id + 1);
	glVertexAttribDivisor(id + 1, 1);
}

void draw() {
	glfwMakeContextCurrent(window);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	} else	std::cout << "GLEW initialized" << std::endl;

	#ifdef _WIN32
	// Turn on vertical screen sync under Windows.
		typedef BOOL (WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int interval);
		PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
		if(wglSwapIntervalEXT)
			wglSwapIntervalEXT(1);
	#else
		glfwSwapInterval(1);
	#endif

	glViewport(0, 0, windowWidth, windowHeight);

	glClearColor(0.8f, 0.8f, 0.8f, 1);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	programStuff();

	//Data stuff
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

	float instanceData [] {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f};//the quad I'm going to draw instances of

	glBufferData(GL_ARRAY_BUFFER, sizeof(instanceData), instanceData, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	glGenBuffers(SHADER_ATTRIB_COUNT, vbos);

	//POSITION
	setAttrib(0, 3, GL_FLOAT);

	//BOUNDS
	setAttrib(1, 2, GL_FLOAT);

	//COLOR
	setAttrib(2, 4, GL_FLOAT);

	//TEX DATA
	setAttrib(3, 4, GL_FLOAT);

	while (!glfwWindowShouldClose(window)) {
		checkTextures();
		checkSprites();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(vao);

		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glfwTerminate();//raii
}

void simpleGL::startDrawThread() {
	thread = boost::thread(draw);
}

void simpleGL::joinDrawThread() {
	thread.join();
}
