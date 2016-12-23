#include <fstream>

#include <vector>
#include <memory>

#include "simpleGL.hpp"
#include "simpleUtil.hpp"
#include "shaderData.hpp"

namespace simpleUtil {

	GLuint pipeline;

	GLuint currentVertex {0};
	GLuint currentGeometry {0};
	GLuint currentFragment {0};

	SimpleShader vertexShader;
	SimpleShader geometryShader;
	SimpleShader texFragmentShader;
	SimpleShader emptyFragmentShader;

	GLuint overlayVertexShader;
	GLuint overlayFragmentShader;

	void initShaders() {
		glGenProgramPipelines(1, &pipeline);
		glBindProgramPipeline(pipeline);

		vertexShader = simpleGL::loadShaderSource(simpleShaderData::getVertex(), GL_VERTEX_SHADER);

		geometryShader = simpleGL::loadShaderSource(simpleShaderData::getGeometry(), GL_GEOMETRY_SHADER);

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

		texFragmentShader = simpleGL::loadShaderSource(simpleShaderData::getTexFragment(), GL_FRAGMENT_SHADER);
		emptyFragmentShader = simpleGL::loadShaderSource(simpleShaderData::getEmptyFragment(), GL_FRAGMENT_SHADER);

		overlayVertexShader = simpleGL::loadShaderSource(simpleShaderData::getOverlayVertex(), GL_VERTEX_SHADER).getShader();
		overlayFragmentShader = simpleGL::loadShaderSource(simpleShaderData::getOverlayFragment(), GL_FRAGMENT_SHADER).getShader();


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

	void useOverlayShaders() {

		useShaders(overlayVertexShader, 0, overlayFragmentShader);
	}

	inline void printInfoLog(GLuint program) {
		GLint length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

		std::unique_ptr<GLchar> infoLog(new GLchar[length + 1]);
		glGetProgramInfoLog(program, length, nullptr, infoLog.get());

		print(infoLog.get());
	}

}

using namespace simpleUtil;

namespace simpleGL {

	void setCameraPosition(SimpleVector position) {
		SimpleVector sv = actualToScreen(position);
		float data[2] {sv.x, sv.y};

		glBufferSubData(GL_UNIFORM_BUFFER, 0, 2*sizeof(float), data);
	}

	void setCameraRotation(float rotation) {

		glBufferSubData(GL_UNIFORM_BUFFER, 2*sizeof(float), sizeof(float), &rotation);
	}

	void setOverlayShader(SimpleShader ss) {
		if (ss.getType() != GL_FRAGMENT_SHADER) {
			print("Not a fragment shader");
			return;
		}

		overlayFragmentShader = ss.getShader();
	}

	SimpleShader loadShaderSource(std::string source, GLenum type) {
		if (type != GL_VERTEX_SHADER && type != GL_GEOMETRY_SHADER && type != GL_FRAGMENT_SHADER) {
			print("Wrong shader type");
			return SimpleShader();
		}

		print("Loading shader");

		const char* src = source.c_str();
		GLuint program = glCreateShaderProgramv(type, 1, &src);

		GLint status;
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if (!status) {
			printInfoLog(program);
			return SimpleShader(0, type);
		} else	print("Shader linked");

		glValidateProgram(program);

		glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
		if (!status) {
			printInfoLog(program);
			return SimpleShader(0, type);
		} else	print("Shader validated");

		if (type == GL_GEOMETRY_SHADER) {
			glUniformBlockBinding(program, glGetUniformBlockIndex(program, "DynamicData"), 1);
		}

		return SimpleShader(program, type);
	}

	SimpleShader loadShaderPath(std::string path, GLenum type) {
		print("Loading source");

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
