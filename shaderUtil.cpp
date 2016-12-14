#include <fstream>

#include <vector>
#include <memory>

#include "simpleGL.hpp"
#include "simpleUtil.hpp"

namespace simpleUtil {

	GLuint pipeline;

	GLuint currentVertex {0};
	GLuint currentGeometry {0};
	GLuint currentFragment {0};

	SimpleShader vertexShader;
	SimpleShader geometryShader;
	SimpleShader texFragmentShader;
	SimpleShader emptyFragmentShader;

	void initShaders() {
		glGenProgramPipelines(1, &pipeline);
		glBindProgramPipeline(pipeline);

		vertexShader = simpleGL::loadShader("shaders/vertex.glsl", GL_VERTEX_SHADER);

		geometryShader = simpleGL::loadShader("shaders/geometry.glsl", GL_GEOMETRY_SHADER);

		GLuint stat;
		glGenBuffers(1, &stat);
		glBindBuffer(GL_UNIFORM_BUFFER, stat);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, stat);

		float aspect = ((float) simpleGL::getWindowHeight()) / simpleGL::getWindowWidth();
		glBufferData(GL_UNIFORM_BUFFER, sizeof(float), &aspect, GL_STATIC_DRAW);

		GLuint dynamic;
		glGenBuffers(1, &dynamic);
		glBindBuffer(GL_UNIFORM_BUFFER, dynamic);
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, dynamic);

		glBufferData(GL_UNIFORM_BUFFER, 3*sizeof(float), nullptr, GL_DYNAMIC_DRAW);

		texFragmentShader = simpleGL::loadShader("shaders/texFragment.glsl", GL_FRAGMENT_SHADER);
		emptyFragmentShader = simpleGL::loadShader("shaders/emptyFragment.glsl", GL_FRAGMENT_SHADER);

		print("Shaders initialized");
	}

	void setDefaultShaders(SimpleSprite* sprite, bool empty) {

		sprite->setShader(vertexShader);
		sprite->setShader(geometryShader);
		if (empty)	sprite->setShader(emptyFragmentShader);
		else			sprite->setShader(texFragmentShader);

	}

	void useShaders(GLuint vertex, GLuint geometry, GLuint fragment) {

		if (currentVertex != vertex) {
			glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vertex);
			currentVertex = vertex;
		}

		if (currentGeometry != geometry) {
			glUseProgramStages(pipeline, GL_GEOMETRY_SHADER_BIT, geometry);
			currentGeometry = geometry;
		}

		if (currentFragment != fragment) {
			glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fragment);
			currentFragment = fragment;
		}
	}

}

using namespace simpleUtil;

namespace simpleGL {

	void setCameraPosition(SimpleVector position) {
		SimpleVector sv = toScreenCoords(position);
		float data[2] {sv.x, sv.y};

		glBufferSubData(GL_UNIFORM_BUFFER, 0, 2*sizeof(float), data);
	}

	void setCameraRotation(float rotation) {

		glBufferSubData(GL_UNIFORM_BUFFER, 2*sizeof(float), sizeof(float), &rotation);
	}

	SimpleShader loadShader(std::string path, GLenum type) {
		if (type != GL_VERTEX_SHADER && type != GL_GEOMETRY_SHADER && type != GL_FRAGMENT_SHADER) {
			print("Wrong shader type");
			return SimpleShader();
		}

		print("Loading shader");

		std::ifstream file(path);
		std::string shaderString;

		std::string line;
		while (getline(file, line)) {
			shaderString += line;
			shaderString.push_back('\n');
		}
		file.close();

		const char* source = shaderString.c_str();

		GLuint program = glCreateShaderProgramv(type, 1, &source);
		glValidateProgram(program);

		GLint status;
		glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
		if (!status) {
			GLint length;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

			std::unique_ptr<GLchar> infoLog(new GLchar[length + 1]);
			glGetProgramInfoLog(program, length, nullptr, infoLog.get());

			simpleUtil::print(infoLog.get());
			program = 0;
		} else	print("Program validated");

		if (type == GL_GEOMETRY_SHADER) {
			glUniformBlockBinding(program, glGetUniformBlockIndex(program, "DynamicData"), 1);
		}

		return SimpleShader(program, type);
	}
}

void SimpleShader::setUniformf(const char* location, std::initializer_list<GLfloat> list) const {
	GLint loc = glGetUniformLocation(shader, location);
	std::vector<GLfloat> data(list);

	switch (data.size()) {
		case 1:	glProgramUniform1f(shader, loc, data[0]); break;
		case 2:	glProgramUniform2f(shader, loc, data[0], data[1]); break;
		case 3:	glProgramUniform3f(shader, loc, data[0], data[1], data[2]); break;
		case 4:	glProgramUniform4f(shader, loc, data[0], data[1], data[2], data[3]); break;
		default:	print("Uniform: Not a valid number of arguments."); break;
	}
}

void SimpleShader::setUniformi(const char* location, std::initializer_list<GLint> list) const {
	GLint loc = glGetUniformLocation(shader, location);
	std::vector<GLint> data(list);

	switch (data.size()) {
		case 1:	glProgramUniform1i(shader, loc, data[0]); break;
		case 2:	glProgramUniform2i(shader, loc, data[0], data[1]); break;
		case 3:	glProgramUniform3i(shader, loc, data[0], data[1], data[2]); break;
		case 4:	glProgramUniform4i(shader, loc, data[0], data[1], data[2], data[3]); break;
		default:	print("Uniform: Not a valid number of arguments."); break;
	}
}

void SimpleShader::setUniformui(const char* location, std::initializer_list<GLuint> list) const {
	GLint loc = glGetUniformLocation(shader, location);
	std::vector<GLuint> data(list);

	switch (data.size()) {
		case 1:	glProgramUniform1ui(shader, loc, data[0]); break;
		case 2:	glProgramUniform2ui(shader, loc, data[0], data[1]); break;
		case 3:	glProgramUniform3ui(shader, loc, data[0], data[1], data[2]); break;
		case 4:	glProgramUniform4ui(shader, loc, data[0], data[1], data[2], data[3]); break;
		default:	print("Uniform: Not a valid number of arguments."); break;
	}
}
