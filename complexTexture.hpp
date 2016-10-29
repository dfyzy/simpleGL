#ifndef COMPLEX_TEXTURE_H
#define COMPLEX_TEXTURE_H

#include <list>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "simpleGL.hpp"
#include "simpleTexture.hpp"
#include "complexSprite.hpp"

class ComplexTexture : public SimpleTexture {
private:
	GLuint texture;
	std::list<ComplexSprite> sprites;

public:
	ComplexTexture(unsigned w, unsigned h, GLuint id) : SimpleTexture(w, h), texture(id) {
		changeFiltering();
	}

	void loadPosition(SimplePosition sp, float* array) {
		array[0] = 2*sp.x/simpleGL::getWidth();
		array[1] = 2*sp.y/simpleGL::getHeight();
		array[2] = sp.z*ZPOINT;
	}

	void loadBounds(float w, float h, float* array, int offset) {
		array[0 + offset] = 2*w*width/simpleGL::getWidth();
		array[1 + offset] = 2*h*height/simpleGL::getHeight();
	}

	void loadColor(SimpleColor c, float* array, int offset) {
		array[0 + offset] = c.r;
		array[1 + offset] = c.g;
		array[2 + offset] = c.b;
		array[3 + offset] = c.a;
	}

	void loadTexData(float x, float y, float w, float h, float* array, int offset) {
		array[0 + offset] = x + w/2;
		array[1 + offset] = y + h/2;
		array[2 + offset] = w;
		array[3 + offset] = h;
	}

	SimpleSprite* loadSprite(SimplePosition sp, float w, float h, SimpleColor c,
											float texX, float texY, float texW, float texH);

	void removeSprite(ComplexSprite* cs) {
		sprites.remove(*cs);
	}

	void draw();

	void changeFiltering();

	void unloadTexture();

	void unload();

	bool operator==(const ComplexTexture& other) const {
		return texture == other.texture;
	}

};

#endif
