#ifndef SIMPLE_TEXTURE_H
#define SIMPLE_TEXTURE_H

class SimpleTextureI {
protected:
	int width, height;

public:
	int getWidth() { return width; }
	int getHeight() { return height; }



//	virtual void createSprite() =0;
//	virtual void changeSprite() =0;
//	virtual void deleteSprite() =0;
};

#endif
