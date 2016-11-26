#include <fstream>
#include <queue>

#include "simpleGL.hpp"
#include "simpleUtil.hpp"
#include "simpleShader.hpp"

template<typename T>
struct Uniform {
	GLuint shader;
	std::string location;

	std::vector<T> data;

	Uniform(GLuint sh, const char* loc, std::initializer_list<T> d) : shader(sh), location(loc), data(d) {}
};

namespace simpleUtil {

	GLuint pipeline;

	SimpleShader vertexShader;
	SimpleShader geometryShader;
	SimpleShader texFragmentShader;
	SimpleShader emptyFragmentShader;

	boost::mutex shaderMutex;
	boost::condition_variable shaderCondition;
	bool shaderReady;

	std::string shaderPath;
	GLenum shaderType;
	GLuint returnProgram = 0;

	boost::mutex uniformMutex;
	std::queue<Uniform<GLfloat>> uniformfQueue;
	std::queue<Uniform<GLint>> uniformiQueue;
	std::queue<Uniform<GLuint>> uniformuiQueue;

	inline void notifyShader() {
		shaderReady = true;
		shaderCondition.notify_one();
	}

	void initShaders(float aspect) {
		glGenProgramPipelines(1, &pipeline);
		glBindProgramPipeline(pipeline);

		vertexShader = simpleGL::loadShader("shaders/vertex.glsl", GL_VERTEX_SHADER);

		geometryShader = simpleGL::loadShader("shaders/geometry.glsl", GL_GEOMETRY_SHADER);

		glProgramUniform1f(geometryShader.getShader(), glGetUniformLocation(geometryShader.getShader(), "rAspect"),
								((float) simpleGL::getWindowHeight()) / simpleGL::getWindowWidth());

		texFragmentShader = simpleGL::loadShader("shaders/texFragment.glsl", GL_FRAGMENT_SHADER);
		emptyFragmentShader = simpleGL::loadShader("shaders/emptyFragment.glsl", GL_FRAGMENT_SHADER);
	}

	void setDefaultShaders(SimpleSprite* sprite, bool empty) {

		sprite->setShader(vertexShader);
		sprite->setShader(geometryShader);
		if (empty)	sprite->setShader(emptyFragmentShader);
		else			sprite->setShader(texFragmentShader);

	}

	std::string loadSource(std::string path) {
		std::ifstream file(path);

		std::string shaderString;

		std::string line;
		while (getline(file, line)) {
			shaderString += line;
			shaderString.push_back('\n');
		}
		file.close();

		return shaderString;
	}

	void loadShader() {
		print("Loading shader");

		const char* source = loadSource(shaderPath).c_str();

		GLuint program = glCreateShaderProgramv(shaderType, 1, &source);
		shaderPath.clear();

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

		returnProgram = program;
		notifyShader();
	}

	void useShaders(GLuint vertex, GLuint geometry, GLuint fragment) {

		glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vertex);
		glUseProgramStages(pipeline, GL_GEOMETRY_SHADER_BIT, geometry);
		glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fragment);

	}

	void checkUniforms() {
		boost::lock_guard<boost::mutex> lock(uniformMutex);

		while (!uniformfQueue.empty()) {
			Uniform<GLfloat> uni = uniformfQueue.front();
			uniformfQueue.pop();

			GLint location = glGetUniformLocation(uni.shader, uni.location.c_str());

			switch (uni.data.size()) {
				case 1:	glProgramUniform1f(uni.shader, location, uni.data[0]); break;
				case 2:	glProgramUniform2f(uni.shader, location, uni.data[0], uni.data[1]); break;
				case 3:	glProgramUniform3f(uni.shader, location, uni.data[0], uni.data[1], uni.data[2]); break;
				case 4:	glProgramUniform4f(uni.shader, location, uni.data[0], uni.data[1], uni.data[2], uni.data[3]); break;
				default:	print("Uniform: Not a valid number of arguments."); break;
			}
		}

		while (!uniformiQueue.empty()) {
			Uniform<GLint> uni = uniformiQueue.front();
			uniformiQueue.pop();

			GLint location = glGetUniformLocation(uni.shader, uni.location.c_str());

			switch (uni.data.size()) {
				case 1:	glProgramUniform1i(uni.shader, location, uni.data[0]); break;
				case 2:	glProgramUniform2i(uni.shader, location, uni.data[0], uni.data[1]); break;
				case 3:	glProgramUniform3i(uni.shader, location, uni.data[0], uni.data[1], uni.data[2]); break;
				case 4:	glProgramUniform4i(uni.shader, location, uni.data[0], uni.data[1], uni.data[2], uni.data[3]); break;
				default:	print("Uniform: Not a valid number of arguments."); break;
			}
		}

		while (!uniformfQueue.empty()) {
			Uniform<GLuint> uni = uniformuiQueue.front();
			uniformuiQueue.pop();

			GLint location = glGetUniformLocation(uni.shader, uni.location.c_str());

			switch (uni.data.size()) {
				case 1:	glProgramUniform1ui(uni.shader, location, uni.data[0]); break;
				case 2:	glProgramUniform2ui(uni.shader, location, uni.data[0], uni.data[1]); break;
				case 3:	glProgramUniform3ui(uni.shader, location, uni.data[0], uni.data[1], uni.data[2]); break;
				case 4:	glProgramUniform4ui(uni.shader, location, uni.data[0], uni.data[1], uni.data[2], uni.data[3]); break;
				default:	print("Uniform: Not a valid number of arguments."); break;
			}
		}
	}

	void checkShaders() {
		{
			boost::lock_guard<boost::mutex> lock(shaderMutex);
			if (!shaderPath.empty()) loadShader();
		}

		checkUniforms();
	}

}

using namespace simpleUtil;

SimpleShader simpleGL::loadShader(std::string path, GLenum ptype) {
	if (ptype != GL_VERTEX_SHADER && ptype != GL_GEOMETRY_SHADER && ptype != GL_FRAGMENT_SHADER) {
		print("Wrong shader type");
		return SimpleShader();
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

	return SimpleShader(returnProgram, ptype);
}

void SimpleShader::setUniformf(const char* location, std::initializer_list<GLfloat> list) {
	boost::lock_guard<boost::mutex> lock(uniformMutex);

	uniformfQueue.emplace(shader, location, list);
}

void SimpleShader::setUniformi(const char* location, std::initializer_list<GLint> list) {
	boost::lock_guard<boost::mutex> lock(uniformMutex);

	uniformiQueue.emplace(shader, location, list);
}

void SimpleShader::setUniformui(const char* location, std::initializer_list<GLuint> list) {
	boost::lock_guard<boost::mutex> lock(uniformMutex);

	uniformuiQueue.emplace(shader, location, list);
}
