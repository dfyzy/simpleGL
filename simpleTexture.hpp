#ifndef SIMPLE_TEXTURE_H
#define SIMPLE_TEXTURE_H

#include <string>

#include "glfw.hpp"

class SimpleTexture {
private:
	unsigned pixelWidth = 100, pixelHeight = 100;
	GLuint texture = 0;

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
	SimpleTexture(std::string path);
	
	/*
	 *	Gets pixel width and height of this texture.
	 */
	unsigned getWidth() const { return pixelWidth; }
	unsigned getHeight() const { return pixelHeight; }

	GLuint getTexture() const { return texture; }

	void setFiltering(GLenum tf) const;

	void unload() const;

};

#endif
