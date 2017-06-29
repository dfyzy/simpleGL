#include "framebuffer.h"
#include "util.h"

namespace simpleGL {

Framebuffer::Framebuffer(unsigned width, unsigned height, GLenum format, GLenum filtering, Color base) : base(base) {
	util::print("Framebuffer:load:");

	glGenFramebuffers(1, &fbo);

	util::println(std::to_string(fbo));

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	image = new Image(width, height, format, filtering);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, image->getId(), 0);
}

Framebuffer::~Framebuffer() {
	util::println(std::string("Framebuffer:unload:") + std::to_string(fbo));

	glDeleteFramebuffers(1, &fbo);
	image->unload();
}

void Framebuffer::bind(Matrix view) {
	lastWidth = util::getResWidth();
	lastHeight = util::getResHeight();
	lastFbo = util::getFbo();
	lastView = util::getViewMatrix();

	util::setResolution(image->getWidth(), image->getHeight());
	util::setFbo(fbo);
	util::setViewMatrix(view);

	glClearColor(base.r, base.g, base.b, base.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Framebuffer::unbind() {
	util::setResolution(lastWidth, lastHeight);
	util::setFbo(lastFbo);
	util::setViewMatrix(lastView);
}

void unbind() {

}


}
