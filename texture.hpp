#ifndef SIMPLE_TEXTURE_H
#define SIMPLE_TEXTURE_H

#include "image.hpp"

namespace simpleGL {

class Texture {
protected:
	GLuint id = 0;
	Vector position;
	Vector bounds;

public:
	Texture() {}
	Texture(Vector bounds) : bounds(bounds) {}

	Texture(Image* image, Vector position, Vector bounds) : id(image->getId()), position(position), bounds(bounds) {}
	Texture(Image* image) : id(image->getId()), bounds(image->getWidth(), image->getHeight()) {}

	GLuint getId() const { return id; }

	Vector getPosition() const { return position; }

	Vector getBounds() const { return bounds; }

	void bind();

};

}

#endif
