#ifndef COMPLEX_TEXTURE_H
#define COMPLEX_TEXTURE_H

#include <list>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "simpleTexture.hpp"
#include "complexSprite.hpp"

class ComplexTexture : public SimpleTexture {
private:
	GLuint texture;
	std::list<ComplexSprite> sprites;

public:
	ComplexTexture(unsigned w, unsigned h, GLuint id) : SimpleTexture(w, h), texture(id) {}

	SimpleSprite* loadSprite(float x, float y, float z, float w, float h, SimpleColor c,
											float texX, float texY, float texW, float texH);

	void removeSprite(ComplexSprite* cs) {
		sprites.remove(*cs);
	}

	void draw();

	void unload();

	bool operator==(const ComplexTexture& other) const {
		return texture == other.texture;
	}

};

#endif
