#ifndef SIMPLE_TEXTURE_H
#define SIMPLE_TEXTURE_H

#include "image.h"

namespace simpleGL {

class Texture {
protected:
	Image* image = nullptr;
	Vector position;
	Vector bounds;

public:
	Texture() {}
	Texture(Vector bounds) : bounds(bounds) {}

	Texture(Image* image, Vector position, Vector bounds) : image(image), position(position), bounds(bounds) {}
	Texture(Image* image) : image(image), bounds(image->getWidth(), image->getHeight()) {}

	Image* getImage() const { return image; }

	Vector getPosition() const { return position; }

	Vector getBounds() const { return bounds; }

	void bind();

};

}

#endif
