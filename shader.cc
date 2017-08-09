#include <fstream>

#include <memory>

#include "shader.h"
#include "shaderData.h"
#include "util.h"

namespace {

GLuint pipeline {0};

GLuint currentVertex {0};
GLuint currentFragment {0};

GLuint defaultVertexShader {0};

GLuint defaultFragmentShader {0};
GLuint emptyFragmentShader {0};


inline void printInfoLog(GLuint program) {
	GLint length;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

	std::unique_ptr<GLchar[]> infoLog(new GLchar[length + 1]);
	glGetProgramInfoLog(program, length, nullptr, infoLog.get());

	simpleGL::util::println(infoLog.get());
}

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

GLuint loadShaderSource(std::string source, GLenum type) {
	if (pipeline == 0) {
		util::println("Shader pipeline:load");

		glGenProgramPipelines(1, &pipeline);
		glBindProgramPipeline(pipeline);
	}

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
	if (file.fail()) {
		util::println("error:Shader:failed to open file");
		return 0;
	}

	std::string shaderString;

	std::string line;
	while (getline(file, line)) {
		shaderString += line;
		shaderString.push_back('\n');
	}
	file.close();

	return loadShaderSource(shaderString, type);
}

void useShaders(GLuint vertex, GLuint fragment) {
	if (currentVertex != vertex) {
		glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vertex);
		currentVertex = vertex;
	}

	if (currentFragment != fragment) {
		glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fragment);
		currentFragment = fragment;
	}
}

}
