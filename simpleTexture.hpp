#ifndef SIMPLE_TEXTURE_H
#define SIMPLE_TEXTURE_H

#include "color.hpp"
#include "simpleSprite.hpp"

class SimpleTexture {
protected:
	unsigned width, height;
	SimpleTexture(unsigned w, unsigned h) : width(w), height(h) {}

public:
	unsigned getWidth() { return width; }
	unsigned getHeight() { return height; }

	//SpriteData?
	virtual SimpleSprite* loadSprite(float x, float y, float z, float w, float h, Color c,
													float texX, float texY, float texW, float texH) =0;

	SimpleSprite* loadSprite(float x, float y, float z, float w, float h, Color c) {
		return loadSprite(x, y, z, w, h, c, 0, 0, width, height);
	}

	SimpleSprite* loadSprite(float x, float y, float z, float scale, Color c) {
		return loadSprite(x, y, z, scale, scale, c);
	}

	SimpleSprite* loadSprite(float x, float y, float z, Color c) {
		return loadSprite(x, y, z, 1, c);
	}

	virtual void unload() =0;
	
};

#endif
