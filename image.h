#ifndef SIMPLE_IMAGE_H
#define SIMPLE_IMAGE_H

#include "glfw.h"
#include "math.h"
#include <string>

namespace simpleGL {

class Image {
private:
	GLuint id {0};
	GLint internal;
	GLenum format;
	GLenum type;
	GLenum filtering;
	unsigned width, height;

protected:
	void resize(unsigned newWidth, unsigned newHeight, GLint newInternal, GLenum newFormat, GLenum newType);

	Image(GLenum filtering);

	~Image();

public:
	/*
	 *	Creates empty image with given resolution.
	 */
	Image(unsigned width, unsigned height, GLint internal, GLenum format, GLenum type, GLenum filtering);

	Image(unsigned width, unsigned height, GLenum format, GLenum filtering)
		: Image(width, height, format, format, GL_UNSIGNED_BYTE, filtering) {}


	/*
	 * Loads image from file into opengl texture object. Only works with png images(for now?).
	 *
	 * 'path': the path of the image file.
	 */
	Image(std::string path, GLenum filtering);

	GLuint getId() const { return id; }

	GLint getInternalFormat() const { return internal; }
	GLenum getFormat() const {	return format; }
	GLenum getDataType() const { return type; }
	GLenum getFiltering() const {	return filtering; }

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
