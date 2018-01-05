#include "texture.h"
#include "image.h"

namespace simpleGL {

Texture::Texture(const Image* image) : image(image), bounds(image != nullptr ? Vector(image->getWidth(), image->getHeight()) : 0) {}

void Texture::bind() {
	if (image)	image->bind();
	else			Image::unbind();
}

}
