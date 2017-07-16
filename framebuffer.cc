#include "framebuffer.h"
#include "util.h"

namespace {

constexpr int SAMPLES_NUM = 4;

GLuint currentFbo {0};

GLenum stencilFormat {0};

}

namespace simpleGL {

void util::setFbo(GLuint fbo) {
	currentFbo = fbo;

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

GLuint util::getFbo() {
	return currentFbo;
}

GLenum getStencilFormat() {
	if (stencilFormat == 0) {
		util::print("simpleGL:stencil only support:");

		std::string word;

		if (GLEW_ARB_ES3_compatibility || GLEW_ARB_texture_stencil8) {
			word = "true";
			stencilFormat = GL_STENCIL_INDEX8;
		} else {
			word = "false";
			stencilFormat = GL_DEPTH24_STENCIL8;
		}

		util::println(word);
	}

	return stencilFormat;
}

Framebuffer::Framebuffer(unsigned width, unsigned height, GLint internalFormat, GLenum format, GLenum type,
									bool stencil, GLenum filtering, Color base) : base(base) {
	util::println("Framebuffer:load");

	glGenFramebuffers(1, &msaaFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, msaaFbo);

	glGenRenderbuffers(1, &colorRbo);
	glBindRenderbuffer(GL_RENDERBUFFER, colorRbo);

	glRenderbufferStorageMultisample(GL_RENDERBUFFER, SAMPLES_NUM, internalFormat, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRbo);

	if (stencil) {
		glGenRenderbuffers(1, &stencilRbo);
		glBindRenderbuffer(GL_RENDERBUFFER, stencilRbo);

		glRenderbufferStorageMultisample(GL_RENDERBUFFER, SAMPLES_NUM, getStencilFormat(), width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencilRbo);
	}

	glGenFramebuffers(1, &rectFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, rectFbo);

	image = new Image(width, height, internalFormat, format, type, filtering);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, image->getId(), 0);
}

Framebuffer::~Framebuffer() {
	util::println(std::string("Framebuffer:unload"));

	glDeleteFramebuffers(1, &msaaFbo);
	glDeleteFramebuffers(1, &rectFbo);
	glDeleteRenderbuffers(1, &colorRbo);
	glDeleteRenderbuffers(1, &stencilRbo);
	image->unload();
}

void Framebuffer::bind(Matrix view) {
	lastWidth = util::getResWidth();
	lastHeight = util::getResHeight();
	lastFbo = util::getFbo();
	lastView = util::getViewMatrix();

	util::setResolution(image->getWidth(), image->getHeight());
	util::setFbo(msaaFbo);
	util::setViewMatrix(view);

	glClearColor(base.r, base.g, base.b, base.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Framebuffer::unbind() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rectFbo);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaFbo);

	glBlitFramebuffer(0, 0, image->getWidth(), image->getHeight(),
							0, 0, image->getWidth(), image->getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);

	util::setResolution(lastWidth, lastHeight);
	util::setFbo(lastFbo);
	util::setViewMatrix(lastView);
}


}
