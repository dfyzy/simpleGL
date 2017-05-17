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
	float data[] {(float)width, (float)height};

	setUniform(data, dynUniform::RESOLUTION);
}

void util::setDefaultResolution() {
	setResolution(getWindowWidth(), getWindowHeight());
}

void util::setCameraData(Matrix view) {
	Matrix m = view.inv();
	//transposing and padding
	float data[] {m.get(0, 0),		m.get(1, 0),	m.get(2, 0), 0,
						m.get(0, 1),	m.get(1, 1),	m.get(2, 1), 0,
						m.get(0, 2),	m.get(1, 2),	m.get(2, 2), 0};

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
	util::print("Shader pipeline:load");

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
}

GLuint loadShaderSource(std::string source, GLenum type) {
	util::print("Shader:load");

	if (type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER) {
		util::print("error:Shader:wrong shader type");
		return 0;
	}

	const char* src = source.c_str();
	GLuint program = glCreateShaderProgramv(type, 1, &src);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status) {
		util::print("error:Shader:linking");
		printInfoLog(program);
		return 0;
	}

	glValidateProgram(program);

	glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
	if (!status) {
		util::print("error:Shader:validation");
		printInfoLog(program);
		return 0;
	}

	return program;
}

GLuint loadShaderPath(std::string path, GLenum type) {
	util::print("Shader:load file");

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
