#ifndef SIMPLE_IMAGE_H
#define SIMPLE_IMAGE_H

#include "glfw.h"
#include "math.h"

namespace simpleGL {

class Image {
private:
	GLuint id {0};
	GLenum filtering;
	GLenum format;
	unsigned width, height;

protected:
	void resize(unsigned newWidth, unsigned newHeight, GLenum newFormat);

	Image(GLenum filtering);

	~Image();

public:
	/*
	 *	Creates empty image with given resolution.
	 */
	Image(unsigned width, unsigned height, GLenum format, GLenum filtering);

	/*
	 * Loads image from file into opengl texture object. Only works with png images(for now?).
	 *
	 * 'path': the path of the image file.
	 */
	Image(const char* path, GLenum filtering);

	//Image

	GLuint getId() const { return id; }

	GLenum getFiltering() const {	return filtering; }
	GLenum getFormat() const {	return format; }

	/*
	 *	Gets pixel width and height of this texture.
	 */
	unsigned getWidth() const { return width; }
	unsigned getHeight() const { return height; }

	void setFiltering(GLenum newFiltering);

	void unload() { delete this; }

};

}

#endif
