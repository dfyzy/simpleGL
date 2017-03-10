#include <fstream>

#include <vector>
#include <memory>

#include "simpleGL.h"
#include "util.h"
#include "shaderData.h"

namespace {

struct dynUniform {
	enum E { COLOR, RESOLUTION, CAMERA, COUNT };
	static int sizes[COUNT];
};
int dynUniform::sizes[COUNT] {4, 2, 5};

GLuint pipeline;

GLuint currentVertex {0};
GLuint currentFragment {0};

}

namespace simpleGL {

void setUniform(float* data, dynUniform::E type) {
	int offset = 0;
	for (int i = 0; i < type; i++)
		offset += dynUniform::sizes[i];

	glBufferSubData(GL_UNIFORM_BUFFER, offset*sizeof(float), dynUniform::sizes[type]*sizeof(float), data);
}

void util::useShaders(GLuint vertex, GLuint fragment, Color color) {
	float data[] { color.r, color.g, color.b, color.a };
	setUniform(data, dynUniform::COLOR);

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
	float data[] {(float)width, (float)height};

	setUniform(data, dynUniform::RESOLUTION);
}

void util::setDefaultResolution() {
	setResolution(getWindowWidth(), getWindowHeight());
}

void util::setCameraData(Vector position, Vector scale, double rotation) {
	float data[] {position.x, position.y, scale.x, scale.y, (float) rotation};

	setUniform(data, dynUniform::CAMERA);
}

inline void printInfoLog(GLuint program) {
	GLint length;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

	std::unique_ptr<GLchar> infoLog(new GLchar[length + 1]);
	glGetProgramInfoLog(program, length, nullptr, infoLog.get());

	util::print(infoLog.get());
}

void util::initShaders() {
	glGenProgramPipelines(1, &pipeline);
	glBindProgramPipeline(pipeline);

	GLuint dynamic;
	glGenBuffers(1, &dynamic);
	glBindBuffer(GL_UNIFORM_BUFFER, dynamic);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, dynamic);

	int size = 0;
	for (int i = 0; i < dynUniform::COUNT; i++)	size += dynUniform::sizes[i];
	glBufferData(GL_UNIFORM_BUFFER, size*sizeof(float), nullptr, GL_DYNAMIC_DRAW);
	setDefaultResolution();

	util::print("Shaders initialized");
}

GLuint loadShaderSource(std::string source, GLenum type) {
	if (type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER) {
		util::print("Wrong shader type");
		return 0;
	}

	util::print("Loading shader");

	const char* src = source.c_str();
	GLuint program = glCreateShaderProgramv(type, 1, &src);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status) {
		printInfoLog(program);
		return 0;
	} else	util::print("Shader linked");

	glValidateProgram(program);

	glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
	if (!status) {
		printInfoLog(program);
		return 0;
	} else	util::print("Shader validated");

	return program;
}

GLuint loadShaderPath(std::string path, GLenum type) {
	util::print("Loading from path");

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
