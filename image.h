/* A wrapper around opengl texture object
*/

#ifndef SIMPLE_IMAGE_H
#define SIMPLE_IMAGE_H

#include <string>

#include "glfw.h"

namespace simpleGL {

class Image {
private:
	GLuint id {0};
	GLint internal;
	GLenum format;
	GLenum type;
	GLenum filtering;
	unsigned width {0}, height {0};

public:
	static void unbind();

	/*
	 *	Creates empty image with given filtering mode.
	 */
	Image(GLenum filtering);

	Image(const Image& other) =delete;

	~Image();

	void loadData(unsigned pwidth, unsigned pheight, GLenum pformat, GLint pinternal, GLenum ptype, const void* data);
	void loadData(unsigned pwidth, unsigned pheight, GLenum pformat, const unsigned char* data) {
		return loadData(pwidth, pheight, pformat, pformat, GL_UNSIGNED_BYTE, data);
	}

	void loadData(const std::string& path);

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

	void setFiltering(GLenum gle);

	void bind() const;

};

}

#endif
