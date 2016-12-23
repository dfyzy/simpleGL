#ifndef SIMPLE_TEXTURE_H
#define SIMPLE_TEXTURE_H

#include "glfw.hpp"

class SimpleTexture {
private:
	const unsigned pixelWidth, pixelHeight;
	const GLuint texture;

public:
	SimpleTexture(unsigned width, unsigned height, GLuint id) : pixelWidth(width), pixelHeight(height), texture(id) {}
	~SimpleTexture();

	/*
	 *	Gets pixel width and height of this texture.
	 */
	unsigned getWidth() const { return pixelWidth; }
	unsigned getHeight() const { return pixelHeight; }

	GLuint getTexture() const { return texture; }

	void setFiltering(GLenum tf) const;

};

#endif
