#include <fstream>

#include "simpleGL.hpp"
#include "simpleUtil.hpp"

namespace simpleUtil {
	GLuint pipeline;

	GLuint vertexShader;
	GLuint geometryShader;
	GLuint texFragmentShader;
	GLuint emptyFragmentShader;

	boost::mutex shaderMutex;
	boost::condition_variable shaderCondition;
	bool shaderReady;

	std::string shaderPath;
	GLenum shaderType;
	GLuint returnProgram = 0;

	inline void notifyShader() {
		shaderReady = true;
		shaderCondition.notify_one();
	}

	const char* loadSource(std::string path) {
		std::ifstream file(path);

		std::string shaderString;

		std::string line;
		while (getline(file, line)) {
			shaderString += line;
			shaderString.push_back('\n');
		}
		file.close();

		return shaderString.c_str();
	}

	void loadShader() {
		const char* source = loadSource(shaderPath);
		shaderPath.clear();

		GLuint program = glCreateShaderProgramv(shaderType, 1, &source);

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
		} else	simpleUtil::print("Program validated");

		returnProgram = program;
		notifyShader();
	}

	void initShaders(float aspect) {
		glGenProgramPipelines(1, &pipeline);
		glBindProgramPipeline(pipeline);

		vertexShader = simpleGL::loadShader("shaders/vertex.glsl", GL_VERTEX_SHADER);

		geometryShader = simpleGL::loadShader("shaders/geometry.glsl", GL_GEOMETRY_SHADER);

		glProgramUniform1f(geometryShader, glGetUniformLocation(geometryShader, "rAspect"),
								((float) simpleGL::getWindowHeight()) / simpleGL::getWindowWidth());

		texFragmentShader = simpleGL::loadShader("shaders/texFragment.glsl", GL_FRAGMENT_SHADER);
		emptyFragmentShader = simpleGL::loadShader("shaders/emptyFragment.glsl", GL_FRAGMENT_SHADER);
	}

	void setDefaultShaders(SimpleSprite* sprite, bool empty) {

		sprite->changeVertexShader(vertexShader);
		sprite->changeGeometryShader(geometryShader);
		if (empty)	sprite->changeFragmentShader(emptyFragmentShader);
		else			sprite->changeFragmentShader(texFragmentShader);

	}

	void useShaders(GLuint vertex, GLuint geometry, GLuint fragment) {

		glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vertex);
		glUseProgramStages(pipeline, GL_GEOMETRY_SHADER_BIT, geometry);
		glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fragment);

	}

	void checkShaders() {
		shaderMutex.lock();
		bool empty = shaderPath.empty();
		shaderMutex.unlock();

		if (!empty)	loadShader();
	}

}

using namespace simpleUtil;

GLuint simpleGL::loadShader(std::string path, GLenum ptype) {
	if (ptype != GL_VERTEX_SHADER && ptype != GL_GEOMETRY_SHADER && ptype != GL_FRAGMENT_SHADER) {
		print("Wrong shader type");
		return 0;
	}

	boost::unique_lock<boost::mutex> lock(shaderMutex);
	shaderReady = false;

	shaderPath = path;
	shaderType = ptype;

	if (!isCurrentThread())
		do 	shaderCondition.wait(lock);
		while	(!shaderReady);
	else
		simpleUtil::loadShader();

	return returnProgram;
}
