#include "texture.h"

namespace {
	GLuint currentTexture {0};
}

void simpleGL::Texture::bind() {
	if (id != currentTexture) {

		glBindTexture(GL_TEXTURE_RECTANGLE, id);
		currentTexture = id;
	}
}
