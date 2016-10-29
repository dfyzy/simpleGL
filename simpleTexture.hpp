#ifndef SIMPLE_TEXTURE_H
#define SIMPLE_TEXTURE_H

#include "simpleSprite.hpp"

class SimpleTexture {
protected:
	unsigned width, height;
	SimpleTexture(unsigned w, unsigned h) : width(w), height(h) {}

public:
	unsigned getWidth() { return width; }
	unsigned getHeight() { return height; }

	virtual SimpleSprite* loadSprite(SimplePosition sp, float w, float h, SimpleColor c,
													float texX, float texY, float texW, float texH) =0;

	SimpleSprite* loadSprite(SimplePosition sp, float w, float h, SimpleColor c) {
		return loadSprite(sp, w, h, c, 0, 0, width, height);
	}

	SimpleSprite* loadSprite(SimplePosition sp, float scale, SimpleColor c) {
		return loadSprite(sp, scale, scale, c);
	}

	SimpleSprite* loadSprite(SimplePosition sp, SimpleColor c) {
		return loadSprite(sp, 1, c);
	}

	virtual void unload() =0;

};

#endif
