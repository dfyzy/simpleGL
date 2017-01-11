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

	GLuint vertexShader;
	GLuint geometryShader;

	GLuint defaultFragmentShader;
	GLuint emptyFragmentShader;
	GLuint textFragmentShader;

	GLuint overlayVertexShader;
	GLuint overlayFragmentShader;

	void setDefaultShaders(SimpleSprite* sprite, bool empty) {

		sprite->setVertexShader(vertexShader);
		sprite->setGeometryShader(geometryShader);
		if (empty)	sprite->setFragmentShader(emptyFragmentShader);
		else			sprite->setFragmentShader(defaultFragmentShader);

	}

	void setTextShader(SimpleSprite* sprite) {
		sprite->setFragmentShader(textFragmentShader);
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

	void initShaders() {
		glGenProgramPipelines(1, &pipeline);
		glBindProgramPipeline(pipeline);

		vertexShader = simpleGL::loadShaderSource(simpleShaderData::getVertex(), GL_VERTEX_SHADER);
		geometryShader = simpleGL::loadShaderSource(simpleShaderData::getGeometry(), GL_GEOMETRY_SHADER);

		defaultFragmentShader = simpleGL::loadShaderSource(simpleShaderData::getDefaultFragment(), GL_FRAGMENT_SHADER);
		emptyFragmentShader = simpleGL::loadShaderSource(simpleShaderData::getEmptyFragment(), GL_FRAGMENT_SHADER);
		textFragmentShader = simpleGL::loadShaderSource(simpleShaderData::getTextFragment(), GL_FRAGMENT_SHADER);

		overlayVertexShader = simpleGL::loadShaderSource(simpleShaderData::getOverlayVertex(), GL_VERTEX_SHADER);
		overlayFragmentShader = simpleGL::loadShaderSource(simpleShaderData::getOverlayFragment(), GL_FRAGMENT_SHADER);

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

		float defData[] {0, 0, 0, 1};
		glBufferData(GL_UNIFORM_BUFFER, 4*sizeof(float), defData, GL_DYNAMIC_DRAW);

		print("Shaders initialized");
	}
}

using namespace simpleUtil;

namespace simpleGL {

	GLuint loadShaderSource(std::string source, GLenum type) {
		if (type != GL_VERTEX_SHADER && type != GL_GEOMETRY_SHADER && type != GL_FRAGMENT_SHADER) {
			print("Wrong shader type");
			return 0;
		}

		print("Loading shader");

		const char* src = source.c_str();
		GLuint program = glCreateShaderProgramv(type, 1, &src);

		GLint status;
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if (!status) {
			printInfoLog(program);
			return 0;
		} else	print("Shader linked");

		glValidateProgram(program);

		glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
		if (!status) {
			printInfoLog(program);
			return 0;
		} else	print("Shader validated");

		if (type == GL_GEOMETRY_SHADER) {
			glUniformBlockBinding(program, glGetUniformBlockIndex(program, "DynamicData"), 1);
		}

		return program;
	}

	GLuint loadShaderPath(std::string path, GLenum type) {
		print("Loading from path");

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

	void setCameraPosition(SimpleVector position) {
		SimpleVector sv = actualToScreen(position);
		float data[2] {sv.x, sv.y};

		glBufferSubData(GL_UNIFORM_BUFFER, 0, 2*sizeof(float), data);
	}

	void setCameraRotation(float rotation) {

		glBufferSubData(GL_UNIFORM_BUFFER, 2*sizeof(float), sizeof(float), &rotation);
	}

	void setCameraScale(float scale) {
		
		glBufferSubData(GL_UNIFORM_BUFFER, 3*sizeof(float), sizeof(float), &scale);
	}

	void setOverlayShader(GLuint sh) {
		overlayFragmentShader = sh;
	}
}
