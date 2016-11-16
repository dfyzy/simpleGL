#ifndef COMPLEX_TEXTURE_H
#define COMPLEX_TEXTURE_H

#include <list>

#include "simpleGL.hpp"
#include "complexSprite.hpp"

class ComplexTexture : public SimpleTexture {
private:
	GLuint texture;
	std::list<ComplexSprite> sprites;

public:
	ComplexTexture(unsigned w, unsigned h, GLuint id) : SimpleTexture(w, h), texture(id) {
		changeFiltering();
	}
	
	SimpleSprite* loadSprite(SimplePosition sp, float width, float height, float rotation, SimpleColor c,
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
