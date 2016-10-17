#ifndef SIMPLE_TEXTURE_I_H
#define SIMPLE_TEXTURE_I_H

#include "color.hpp"

class SimpleTextureI {
protected:
	unsigned width, height;
	SimpleTextureI(unsigned w, unsigned h) : width(w), height(h) {}

public:
	unsigned getWidth() { return width; }
	unsigned getHeight() { return height; }

	virtual void createSprite(float x, float y, float z, float w, float h, Color c, float texX, float texY, float texW, float texH) =0;

	void createSprite(float x, float y, float z, float w, float h, Color c) {
		createSprite(x, y, z, w, h, c, 0, 0, width, height);
	}

	void createSprite(float x, float y, float z, Color c) {
		createSprite(x, y, z, width, height, c);
	}

//	virtual void changeSprite() =0;
//	virtual void deleteSprite() =0;
};

#endif
