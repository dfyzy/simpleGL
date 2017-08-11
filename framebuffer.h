/* A wrapper around opengl framebuffer object
 * Draws stuff on an associated image object, instead of the screen
 *
 * Right now only with msaa
*/

#ifndef SIMPLE_FRAMEBUFFER_H
#define SIMPLE_FRAMEBUFFER_H

#include "unsortedSprite.h"

namespace simpleGL {

class Framebuffer {
private:
	GLuint msaaFbo;
	GLuint rectFbo;

	GLuint colorRbo;
	GLuint stencilRbo {0};

	Image* image;

	Color base;

	unsigned lastWidth;
	unsigned lastHeight;
	GLuint lastFbo;
	Matrix lastView;

protected:
	~Framebuffer();

public:
	Framebuffer(unsigned width, unsigned height, GLint internalFormat, GLenum format, GLenum type, bool stencil, GLenum filtering, Color base);
	Framebuffer(unsigned width, unsigned height, GLenum format, bool stencil, GLenum filtering, Color base)
		: Framebuffer(width, height, format, format, GL_UNSIGNED_BYTE, stencil, filtering, base) {}

	Image* getImage() const { return image; }
	GLuint getMsaaFbo() const { return msaaFbo; }
	GLuint getRectFbo() const { return rectFbo; }

	void bind(Matrix view);
	void unbind();

	void unload() { delete this; }
};

}

#endif
