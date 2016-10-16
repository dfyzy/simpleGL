#ifndef SIMPLE_TEXTURE_I_H
#define SIMPLE_TEXTURE_I_H

class SimpleTextureI {
protected:
	unsigned width, height;
	SimpleTextureI(unsigned w, unsigned h) : width(w), height(h) {}

public:
	unsigned getWidth() { return width; }
	unsigned getHeight() { return height; }



//	virtual void createSprite() =0;
//	virtual void changeSprite() =0;
//	virtual void deleteSprite() =0;
};

#endif
