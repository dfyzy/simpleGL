#include "texture.hpp"

namespace simpleUtil {
	GLuint currentTexture {0};
}

using namespace simpleUtil;

void simpleGL::Texture::bind() {
	if (id != currentTexture) {

		glBindTexture(GL_TEXTURE_RECTANGLE, id);
		currentTexture = id;
	}
}
