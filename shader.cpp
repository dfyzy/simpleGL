#include <fstream>

#include <vector>
#include <memory>

#include "simpleGL.hpp"
#include "simpleUtil.hpp"
#include "shaderData.hpp"

using namespace simpleGL;

namespace simpleUtil {

	struct dynUniform {
		enum E { COLOR, CAM_POSITION, CAM_ROTATION, CAM_SCALE, RESOLUTION, COUNT };
		static int sizes[COUNT];
	};
	int dynUniform::sizes[COUNT] {4, 2, 1, 1, 2};

	GLuint pipeline;

	GLuint currentVertex {0};
	GLuint currentFragment {0};

	GLuint vertexShader;

	GLuint defaultFragmentShader;
	GLuint emptyFragmentShader;
	GLuint textFragmentShader;
	GLuint lightingFragmentShader;

	GLuint overlayVertexShader;
	GLuint overlayFragmentShader;

	void setDefaultShaders(UnsortedSprite* sprite, bool empty) {

		sprite->setVertexShader(vertexShader);
		if (empty)	sprite->setFragmentShader(emptyFragmentShader);
		else			sprite->setFragmentShader(defaultFragmentShader);

	}

	void setOverlayShaders(simpleGL::UnsortedSprite* sprite) {
		sprite->setVertexShader(overlayVertexShader);
		sprite->setFragmentShader(overlayFragmentShader);
	}

	void setLightingShaders(UnsortedSprite* sprite) {
		sprite->setFragmentShader(lightingFragmentShader);
	}


	void setTextShader(Sprite* sprite) {
		sprite->setFragmentShader(textFragmentShader);
	}

	void setUniform(float* data, dynUniform::E type) {
		int offset = 0;
		for (int i = 0; i < type; i++)
			offset += dynUniform::sizes[i];

		glBufferSubData(GL_UNIFORM_BUFFER, offset*sizeof(float), dynUniform::sizes[type]*sizeof(float), data);
	}

	void useShaders(GLuint vertex, GLuint fragment, Color color) {
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

	void setResolution(unsigned width, unsigned height) {
		float data[] {(float)width, (float)height};

		setUniform(data, dynUniform::RESOLUTION);
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

		defaultFragmentShader = simpleGL::loadShaderSource(simpleShaderData::getDefaultFragment(), GL_FRAGMENT_SHADER);
		emptyFragmentShader = simpleGL::loadShaderSource(simpleShaderData::getEmptyFragment(), GL_FRAGMENT_SHADER);
		textFragmentShader = simpleGL::loadShaderSource(simpleShaderData::getTextFragment(), GL_FRAGMENT_SHADER);
		lightingFragmentShader = simpleGL::loadShaderSource(simpleShaderData::getLightingDefaultFragment(), GL_FRAGMENT_SHADER);

		overlayVertexShader = simpleGL::loadShaderSource(simpleShaderData::getOverlayVertex(), GL_VERTEX_SHADER);
		overlayFragmentShader = simpleGL::loadShaderSource(simpleShaderData::getOverlayFragment(), GL_FRAGMENT_SHADER);

		GLuint dynamic;
		glGenBuffers(1, &dynamic);
		glBindBuffer(GL_UNIFORM_BUFFER, dynamic);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, dynamic);

		int size = 0;
		for (int i = 0; i < dynUniform::COUNT; i++)	size += dynUniform::sizes[i];
		glBufferData(GL_UNIFORM_BUFFER, size*sizeof(float), nullptr, GL_DYNAMIC_DRAW);
		simpleGL::setCameraScale(1);
		setDefaultResolution();

		print("Shaders initialized");
	}
}

using namespace simpleUtil;

namespace simpleGL {

	GLuint loadShaderSource(std::string source, GLenum type) {
		if (type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER) {
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

	void setCameraPosition(Vector position) {
		float data[2] {position.x, position.y};

		setUniform(data, dynUniform::CAM_POSITION);
	}

	void setCameraRotation(float rotation) {

		setUniform(&rotation, dynUniform::CAM_ROTATION);
	}

	void setCameraScale(float scale) {

		setUniform(&scale, dynUniform::CAM_SCALE);
	}

	void setOverlayShader(GLuint sh) {
		overlayFragmentShader = sh;
	}
}
