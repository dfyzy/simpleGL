#ifndef SIMPLE_FRAMEBUFFER_H
#define SIMPLE_FRAMEBUFFER_H

#include "unsortedSprite.h"

namespace simpleGL {

class Framebuffer {
private:
	Image* image;
	GLuint fbo;

	Color base;

	unsigned lastWidth;
	unsigned lastHeight;
	GLuint lastFbo;
	Matrix lastView;

protected:
	~Framebuffer();

public:
	Framebuffer(unsigned width, unsigned height, GLenum format, GLenum filtering, Color base);

	Image* getImage() const { return image; }
	GLuint getFbo() const { return fbo; }

	void bind(Matrix view);
	void unbind();

	void unload() { delete this; }
};

}

#endif
