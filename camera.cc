#include "camera.h"
#include "simpleGL.h"
#include "util.h"
#include "shaderData.h"

namespace simpleGL {

Camera* Camera::instance = nullptr;
GLuint Camera::msaaFbo = 0;
GLuint Camera::rectFbo = 0;

Camera* Camera::getInstance() {
	if (instance == nullptr) {
		glGenFramebuffers(1, &msaaFbo);
		glBindFramebuffer(GL_FRAMEBUFFER, msaaFbo);

		GLuint renderbuffer;
		glGenRenderbuffers(1, &renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);

		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, getWindowWidth(), getWindowHeight());

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);

		glGenFramebuffers(1, &rectFbo);
		glBindFramebuffer(GL_FRAMEBUFFER, rectFbo);

		Image* image = new Image(getWindowWidth(), getWindowHeight(), GL_RGB, GL_NEAREST);//DEST
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, image->getId(), 0);

		instance = new Camera(image);
		instance->setVertexShader(loadShaderSource(simpleShaderData::getOverlayVertex(), GL_VERTEX_SHADER));
		instance->setFragmentShader(loadShaderSource(simpleShaderData::getOverlayFragment(), GL_FRAGMENT_SHADER));

		util::print("Camera initialized");
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
	util::setCameraData(getModelMatrix());
}

void Camera::bindTexture() {
	util::bindData(getId(), vboType::VERTEX, Matrix::scale(getTexture().getBounds()));

	UnsortedSprite::bindTexture();
}

}
