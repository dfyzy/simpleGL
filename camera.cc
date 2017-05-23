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
		util::print("Camera:load");

		glGenFramebuffers(1, &msaaFbo);
		glBindFramebuffer(GL_FRAMEBUFFER, msaaFbo);

		GLuint rendRGB;
		glGenRenderbuffers(1, &rendRGB);
		glBindRenderbuffer(GL_RENDERBUFFER, rendRGB);

		//TODO: proper internal formats.
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, getWindowWidth(), getWindowHeight());
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rendRGB);


		GLuint rendStenc;
		glGenRenderbuffers(1, &rendStenc);
		glBindRenderbuffer(GL_RENDERBUFFER, rendStenc);

		//NOTE:	GL_STENCIL_INDEX8 isn't supported on OpenGL less than 4.3 or without ARB_ES3_compatibility or ARB_texture_stencil8 extensions.
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_STENCIL_INDEX8, getWindowWidth(), getWindowHeight());
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
	util::setCameraData(getModelMatrix());
}

void Camera::bindTexture() {
	util::bindQuadData(getId(), vboType::VERTEX, Matrix::scale(getTexture().getBounds()));

	UnsortedSprite::bindTexture();
}

}
