#include "texture.h"

namespace {

simpleGL::Image* currentImage;

}

void simpleGL::Texture::bind() {
	if (image != currentImage) {

		glBindTexture(GL_TEXTURE_RECTANGLE, image == nullptr ? 0 : image->getId());
		currentImage = image;
	}
}
