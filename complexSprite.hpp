#ifndef COMPLEX_SPRITE_H
#define COMPLEX_SPRITE_H

#include "simpleSprite.hpp"

const float ZPOINT = 0.0001f;

class ComplexTexture;

class ComplexSprite : public SimpleSprite {
public:
	struct Attrib {
		enum E { POSITION, BOUNDS, COLOR, TEX_DATA, COUNT } type;
	 	static const unsigned sizes[4];

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

	void changePosition(SimplePosition sp);
	void changeBounds(float width, float height);
	void changeColor(SimpleColor c);
	void changeTexData(float x, float y, float w, float h);

	void deleteData();

	void unload();

	bool operator==(const ComplexSprite& other) const {
		return id == other.id;
	}

};

#endif
