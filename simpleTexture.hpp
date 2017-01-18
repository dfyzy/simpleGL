#ifndef SIMPLE_TEXTURE_H
#define SIMPLE_TEXTURE_H

#include "glfw.hpp"

class SimpleTexture {
protected:
	unsigned width = 100, height = 100;
	GLuint texture = 0;

	SimpleTexture(unsigned width, unsigned height) : width(width), height(height) {}

public:
	/*
	 * Texture object without texture with width and height equal 100.
	 */
	SimpleTexture() {}

	/*
	 * Loads texture from file into opengl texture object. Only works with png images(for now?).
	 *
	 * 'path': the path of the image file.
	 *
	 * returns: handle to texture object.
	 */
	SimpleTexture(const char* path);

	/*
	 *	Gets pixel width and height of this texture.
	 */
	unsigned getWidth() const { return width; }
	unsigned getHeight() const { return height; }

	GLuint getTexture() const { return texture; }

	void setFiltering(GLenum tf) const;

	virtual void unload();

};

#endif
