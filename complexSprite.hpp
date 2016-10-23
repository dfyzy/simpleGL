#ifndef COMPLEX_SPRITE_H
#define COMPLEX_SPRITE_H

#include "simpleSprite.hpp"

class ComplexTexture;

class ComplexSprite : public SimpleSprite {
public:
	struct Attrib {
		enum E { POSITION, BOUNDS, COLOR, TEX_DATA, COUNT } type;
	 	static const int sizes[4];

		std::unique_ptr<float> data;
		unsigned spriteId;

		Attrib(E t, unsigned i) : type(t), spriteId(i) {
			data = std::unique_ptr<float>(new float[sizes[t]]);
		}
	};

private:
	ComplexTexture* texture;

	void changeAttrib(Attrib att);

public:
	ComplexSprite(unsigned i, ComplexTexture* t) : SimpleSprite(i), texture(t) {}

	void changePosition(float x, float y, float z);
	void changeBounds(float width, float height);
	void changeColor(Color c);
	void changeTexData(float texX, float texY, float texW, float texH);

};

#endif
