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
	GLuint lightFragmentShader;

	GLuint overlayVertexShader;
	GLuint overlayFragmentShader;

	void setDefaultShaders(SimplerSprite* sprite, bool empty) {

		sprite->setVertexShader(vertexShader);
		sprite->setGeometryShader(geometryShader);
		if (empty)	sprite->setFragmentShader(emptyFragmentShader);
		else			sprite->setFragmentShader(defaultFragmentShader);

	}

	void setTextShader(SimpleSprite* sprite) {
		sprite->setFragmentShader(textFragmentShader);
	}

	void setLightShader(SimpleSprite* sprite) {
		sprite->setFragmentShader(lightFragmentShader);
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

	void setResolution(unsigned width, unsigned height) {
		float data[] {2.0f/height, ((float) height)/width};

		glBufferSubData(GL_UNIFORM_BUFFER, 4*sizeof(float), 2*sizeof(float), data);
	}

	void setDefaultResolution() {
		setResolution(simpleGL::getWindowWidth(), simpleGL::getWindowHeight());
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
		lightFragmentShader = simpleGL::loadShaderSource(simpleShaderData::getLightFragment(), GL_FRAGMENT_SHADER);

		overlayVertexShader = simpleGL::loadShaderSource(simpleShaderData::getOverlayVertex(), GL_VERTEX_SHADER);
		overlayFragmentShader = simpleGL::loadShaderSource(simpleShaderData::getOverlayFragment(), GL_FRAGMENT_SHADER);

		GLuint dynamic;
		glGenBuffers(1, &dynamic);
		glBindBuffer(GL_UNIFORM_BUFFER, dynamic);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, dynamic);

		glBufferData(GL_UNIFORM_BUFFER, 6*sizeof(float), nullptr, GL_DYNAMIC_DRAW);
		simpleGL::setCameraScale(1);
		setDefaultResolution();

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
		float data[2] {position.x, position.y};

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
