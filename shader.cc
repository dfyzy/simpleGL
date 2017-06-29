#include <fstream>

#include <vector>
#include <memory>

#include "simpleGL.h"
#include "util.h"
#include "shaderData.h"

namespace {

struct dynUniform {
	enum E { CAMERA, RESOLUTION, COUNT };
	static int sizes[COUNT];
};
int dynUniform::sizes[COUNT] {12, 2};//if adding new - change 2 to 4.

simpleGL::Matrix viewMatrix;

unsigned resWidth;
unsigned resHeight;

GLuint pipeline;

GLuint currentVertex {0};
GLuint currentFragment {0};

GLuint defaultVertexShader {0};

GLuint defaultFragmentShader {0};
GLuint emptyFragmentShader {0};

}

namespace simpleGL {

GLuint getDefaultVertexShader() {
	if (defaultVertexShader == 0)
		defaultVertexShader = loadShaderSource(shaderData::getVertex(), GL_VERTEX_SHADER);

	return defaultVertexShader;
}

GLuint getDefaultFragmentShader(bool empty) {
	if (empty) {
		if (emptyFragmentShader == 0)
			emptyFragmentShader = loadShaderSource(shaderData::getEmptyFragment(), GL_FRAGMENT_SHADER);

		return emptyFragmentShader;
	} else {
		if (defaultFragmentShader == 0)
			defaultFragmentShader = loadShaderSource(shaderData::getDefaultFragment(), GL_FRAGMENT_SHADER);

		return defaultFragmentShader;
	}
}

void setUniform(float* data, dynUniform::E type) {
	int offset = 0;
	for (int i = 0; i < type; i++)
		offset += dynUniform::sizes[i];

	glBufferSubData(GL_UNIFORM_BUFFER, offset*sizeof(float), dynUniform::sizes[type]*sizeof(float), data);
}

void util::useShaders(GLuint vertex, GLuint fragment) {
	if (currentVertex != vertex) {
		glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vertex);
		currentVertex = vertex;
	}

	if (currentFragment != fragment) {
		glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fragment);
		currentFragment = fragment;
	}
}

void util::setResolution(unsigned width, unsigned height) {
	resWidth = width;
	resHeight = height;

	float data[] {(float)width, (float)height};

	setUniform(data, dynUniform::RESOLUTION);
	glViewport(0, 0, width, height);
}

unsigned util::getResWidth() {
	return resWidth;
}

unsigned util::getResHeight() {
	return resHeight;
}

void util::setViewMatrix(Matrix view) {
	viewMatrix = view;

	Matrix m = view.inv();
	//transposing and padding
	float data[] {m.get(0, 0),		m.get(1, 0),	m.get(2, 0), 0,
						m.get(0, 1),	m.get(1, 1),	m.get(2, 1), 0,
						m.get(0, 2),	m.get(1, 2),	m.get(2, 2), 0};

	setUniform(data, dynUniform::CAMERA);
}

Matrix util::getViewMatrix() {
	return viewMatrix;
}

inline void printInfoLog(GLuint program) {
	GLint length;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

	std::unique_ptr<GLchar> infoLog(new GLchar[length + 1]);
	glGetProgramInfoLog(program, length, nullptr, infoLog.get());

	util::println(infoLog.get());
}

void util::initShaders() {
	util::println("Shader pipeline:load");

	glGenProgramPipelines(1, &pipeline);
	glBindProgramPipeline(pipeline);

	GLuint dynamic;
	glGenBuffers(1, &dynamic);
	glBindBuffer(GL_UNIFORM_BUFFER, dynamic);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, dynamic);

	int size = 0;
	for (int i = 0; i < dynUniform::COUNT; i++)	size += dynUniform::sizes[i];
	glBufferData(GL_UNIFORM_BUFFER, size*sizeof(float), nullptr, GL_DYNAMIC_DRAW);
	setResolution(getWindowWidth(), getWindowHeight());
}

GLuint loadShaderSource(std::string source, GLenum type) {
	util::println("Shader:load");

	if (type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER) {
		util::println("error:Shader:wrong shader type");
		return 0;
	}

	const char* src = source.c_str();
	GLuint program = glCreateShaderProgramv(type, 1, &src);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status) {
		util::println("error:Shader:linking");
		printInfoLog(program);
		return 0;
	}

	glValidateProgram(program);

	glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
	if (!status) {
		util::println("error:Shader:validation");
		printInfoLog(program);
		return 0;
	}

	return program;
}

GLuint loadShaderPath(std::string path, GLenum type) {
	util::println(std::string("Shader:load file:") + path);

	std::ifstream file(path);
	std::string shaderString;

	std::string line;
	while (getline(file, line)) {
		shaderString += line;
		shaderString.push_back('\n');
	}
	file.close();

	return loadShaderSource(shaderString, type);
}

}
