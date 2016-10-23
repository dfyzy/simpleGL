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
	virtual SimpleSprite* createSprite(float x, float y, float z, float w, float h, Color c,
													float texX, float texY, float texW, float texH) =0;

	SimpleSprite* createSprite(float x, float y, float z, float w, float h, Color c) {
		return createSprite(x, y, z, w, h, c, 0, 0, width, height);
	}

	SimpleSprite* createSprite(float x, float y, float z, Color c) {
		return createSprite(x, y, z, 1, 1, c);
	}

//	virtual void deleteSprite() =0;
};

#endif
