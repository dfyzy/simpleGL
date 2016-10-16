#include <fstream>
#include <list>
#include <queue>
#include <boost/thread.hpp>
#include <iostream>//TODO: logs

#include <windows.h>

#include <libpng/png.h>
#include <zlib.h>

#include "simpleGL.hpp"

//TODO: clean up
struct ShaderAttribs { enum E { POSITION, BOUNDS, COLOR, TEX_DATA, COUNT }; };
struct ShaderAttribsSizes {
	enum E { POSITION = 3, BOUNDS = 2, COLOR = 4, TEX_DATA = 4,
				SUM = POSITION + BOUNDS + COLOR + TEX_DATA, DEFAULT = 0 };

	static E get(ShaderAttribs::E e) {
		switch (e) {
			case ShaderAttribs::POSITION:	return POSITION;
			case ShaderAttribs::BOUNDS:	return BOUNDS;
			case ShaderAttribs::COLOR:		return COLOR;
			case ShaderAttribs::TEX_DATA:	return TEX_DATA;
			default:								return DEFAULT;
		}
	}
};

class SimpleTexture {
private:
	GLuint texture;
	std::list<unsigned> sprites;

public:
	SimpleTexture(GLuint id) : texture(id) {}

	void draw() {}

};

unsigned windowWidth, windowHeight;
GLFWwindow* window = nullptr;

GLuint vao;
GLuint vbos[ShaderAttribs::COUNT];

boost::thread thread;

std::list<SimpleTexture*> textures;

boost::mutex mutex;
std::queue<std::string> textureQueue;

boost::condition_variable returnReady;
unsigned returnValue = -1;

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

void loadtextures() {
	FILE *file = fopen("example\\body_front.png", "rb");
	if (!file) {
		std::cout << "Error opening texture" << std::endl;
		return;
	}

	png_byte header[8];
	fread(header, 1, 8, file);
	if (png_sig_cmp(header, 0, 8)) {
		std::cout << "Not a png" << std::endl;
		fclose(file);
		return;
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if (!png_ptr) {
		std::cout << "Failed to create read struct" << std::endl;
		fclose(file);
		return;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		std::cout << "Failed to create info struct" << std::endl;
		png_destroy_read_struct(&png_ptr, nullptr, nullptr);
		fclose(file);
		return;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		std::cout << "Libpng error" << std::endl;
		png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
		fclose(file);
		return;
	}

	png_init_io(png_ptr, file);

	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	png_uint_32 width, height;
	png_get_IHDR(png_ptr, info_ptr, &width, &height, nullptr, nullptr, nullptr, nullptr, nullptr);

	glActiveTexture(GL_TEXTURE0);
	GLuint texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//return;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	std::unique_ptr<png_byte> row(new png_byte[width]);

	for (png_uint_32 i = height; i > 0; i--) {
		png_read_row(png_ptr, row.get(), nullptr);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, i - 1, width, 1, GL_RGBA, GL_UNSIGNED_BYTE, row.get());
	}

	png_read_end(png_ptr, nullptr);
	png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
	fclose(file);

	std::cout << "Finito" << std::endl;
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

	//Program stuff
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

	/*glGenBuffers(ShaderAttribs::COUNT, vbos);

	for (int i = 0; i < ShaderAttribs::COUNT; i++) {
		glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);

		int attSize = ShaderAttribsSizes::get(static_cast<ShaderAttribs::E> (i));

		glVertexAttribPointer(i + 1, attSize, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(i + 1);
		glVertexAttribDivisor(i + 1, 1);
	}*/

	glGenBuffers(1, vbos);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);

	float data[] = {0, 0, 0, 1, 1, 0, 0, 0, 1, 0.5f, 0.5f, 1, 1};
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	int offset = 0;
	for (int j = 0; j < ShaderAttribs::COUNT; j++) {
		int attSize = ShaderAttribsSizes::get(static_cast<ShaderAttribs::E> (j));

		glVertexAttribPointer(j + 1, attSize, GL_FLOAT, GL_FALSE, sizeof(float) * ShaderAttribsSizes::SUM, (void*) (sizeof(float) * offset));
		glEnableVertexAttribArray(j + 1);
		glVertexAttribDivisor(j + 1, 1);

		offset += attSize;
	}

	loadtextures();

	while (!glfwWindowShouldClose(window)) {
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

unsigned simpleGL::addTexture(std::string path) {

	return 0;
}
