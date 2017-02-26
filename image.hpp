#ifndef SIMPLE_IMAGE_H
#define SIMPLE_IMAGE_H

#include "glfw.hpp"
#include "vector.hpp"

namespace simpleGL {

class Image {
protected:
	unsigned width, height;
	GLuint id;

	void genImage(GLenum filtering, GLenum format);

	Image() {}

public:
	/*
	 *	Creates empty image with given resolution.
	 */
	Image(unsigned width, unsigned height, GLenum format);

	/*
	 * Loads image from file into opengl texture object. Only works with png images(for now?).
	 *
	 * 'path': the path of the image file.
	 */
	Image(const char* path);

	Image(const Image& other) =delete;

	/*
	 *	Gets pixel width and height of this texture.
	 */
	unsigned getWidth() const { return width; }
	unsigned getHeight() const { return height; }

	GLuint getId() const { return id; }

	void setFiltering(GLenum tf) const;

	void unload();

};

}

#endif
