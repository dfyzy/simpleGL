#ifndef SIMPLE_TEXTURE_H
#define SIMPLE_TEXTURE_H

#include "glfw.hpp"

class SimpleTexture {
protected:
	unsigned pixelWidth, pixelHeight;
	GLuint texture;

public:
	SimpleTexture(unsigned width, unsigned height, GLuint id);
	~SimpleTexture();

	/*
	 *	Gets pixel width and height of this texture.
	 */
	unsigned getWidth() { return pixelWidth; }
	unsigned getHeight() { return pixelHeight; }

	GLuint getTexture() { return texture; }

	/*
	 * Unloads this texture and all the sprites created by this texture object. Deletes this object.
	 */
	void unload();

	bool operator==(const SimpleTexture& other) const {
		return texture == other.texture;
	}

};

#endif
