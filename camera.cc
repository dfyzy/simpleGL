#include "camera.h"
#include "simpleGL.h"
#include "util.h"
#include "shaderData.h"

namespace {

GLuint currentFbo;

}

namespace simpleGL {

constexpr int SAMPLES_NUM = 4;

Camera* Camera::instance = nullptr;
GLuint Camera::msaaFbo = 0;
GLuint Camera::rectFbo = 0;

void util::setFbo(GLuint fbo) {
	currentFbo = fbo;

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

GLuint util::getFbo() {
	return currentFbo;
}

Camera* Camera::getInstance() {
	if (instance == nullptr) {
		util::println("Camera:load");

		glGenFramebuffers(1, &msaaFbo);
		util::setFbo(msaaFbo);

		GLuint rendRGB;
		glGenRenderbuffers(1, &rendRGB);
		glBindRenderbuffer(GL_RENDERBUFFER, rendRGB);

		//TODO: proper internal formats.
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, SAMPLES_NUM, GL_RGB, getWindowWidth(), getWindowHeight());
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rendRGB);


		GLuint rendStenc;
		glGenRenderbuffers(1, &rendStenc);
		glBindRenderbuffer(GL_RENDERBUFFER, rendStenc);

		util::print("simpleGL:stencil only support:");

		std::string word;
		GLenum format;
		if (GLEW_ARB_ES3_compatibility || GLEW_ARB_texture_stencil8) {
			word = "true";
			format = GL_STENCIL_INDEX8;
		} else {
			word = "false";
			format = GL_DEPTH24_STENCIL8;
		}

		util::println(word);

		glRenderbufferStorageMultisample(GL_RENDERBUFFER, SAMPLES_NUM, format, getWindowWidth(), getWindowHeight());
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rendStenc);

		glGenFramebuffers(1, &rectFbo);
		glBindFramebuffer(GL_FRAMEBUFFER, rectFbo);

		Image* image = new Image(getWindowWidth(), getWindowHeight(), GL_RGB, GL_NEAREST);//DEST
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, image->getId(), 0);

		instance = new Camera(image);
		instance->setVertexShader(loadShaderSource(shaderData::getOverlayVertex(), GL_VERTEX_SHADER));
		instance->setFragmentShader(loadShaderSource(shaderData::getOverlayFragment(), GL_FRAGMENT_SHADER));
	}

	return instance;
}

GLuint Camera::getMSAAFbo() {
	return msaaFbo;
}

GLuint Camera::getRectFbo() {
	return rectFbo;
}

void Camera::bindVertices() {
	util::setViewMatrix(getModelMatrix());
}

void Camera::bindTexture() {
	getDrawObject()->bindVertexData(Matrix::scale(getTexture().getBounds()));

	UnsortedSprite::bindTexture();
}

}
