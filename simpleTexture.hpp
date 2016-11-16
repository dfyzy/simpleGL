#ifndef SIMPLE_TEXTURE_H
#define SIMPLE_TEXTURE_H

#include "simpleSprite.hpp"

class SimpleTexture {
protected:
	unsigned pixelWidth, pixelHeight;
	SimpleTexture(unsigned width, unsigned height) : pixelWidth(width), pixelHeight(height) {}

public:
	unsigned getWidth() { return pixelWidth; }
	unsigned getHeight() { return pixelHeight; }

	virtual SimpleSprite* loadSprite(SimplePosition sp, float width, float height, float rotation, SimpleColor c,
													float texX, float texY, float texW, float texH) =0;

	SimpleSprite* loadSprite(SimplePosition sp, float width, float height, float rotation, SimpleColor c) {
		return loadSprite(sp, width, height, rotation, c, 0, 0, pixelWidth, pixelHeight);
	}

	SimpleSprite* loadSprite(SimplePosition sp, float width, float height, SimpleColor c) {
		return loadSprite(sp, width, height, 0, c);
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
