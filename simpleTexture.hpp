#ifndef SIMPLE_TEXTURE_H
#define SIMPLE_TEXTURE_H

#include "glfw.hpp"

class SimpleTexture {
protected:
	unsigned width = 100, height = 100;
	GLuint texture = 0;

	void genTexture(GLenum filtering);

public:
	/*
	 * Texture object without texture with width and height equal 100.
	 */
	SimpleTexture() {}

	/*
	 *	Creates empty texture with given resolution.
	 */
	SimpleTexture(unsigned width, unsigned height, GLenum format);

	/*
	 * Loads texture from file into opengl texture object. Only works with png images(for now?).
	 *
	 * 'path': the path of the image file.
	 *
	 * returns: handle to texture object.
	 */
	SimpleTexture(const char* path);

	SimpleTexture(const SimpleTexture& other) =delete;

	/*
	 *	Gets pixel width and height of this texture.
	 */
	unsigned getWidth() const { return width; }
	unsigned getHeight() const { return height; }

	GLuint getTexture() const { return texture; }

	void setFiltering(GLenum tf) const;

	void unload();

};

#endif
