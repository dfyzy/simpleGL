#ifndef COMPLEX_SPRITE_H
#define COMPLEX_SPRITE_H

#include "simpleSprite.hpp"

const float ZPOINT = 0.0001f;

class ComplexSprite : public SimpleSprite {
public:
	struct Attrib {
		enum E { POSITION, BOUNDS, ROTATION, COLOR, TEX_DATA, COUNT } type;
	 	static const unsigned sizes[5];

		std::unique_ptr<float> data;
		unsigned spriteId;

		Attrib(E t) : type(t) {
			data = std::unique_ptr<float>(new float[sizes[t]]);
		}
	};

private:
	int z;

	void resort();
	void unload();

	void setAttrib(Attrib att);

public:
	ComplexSprite(unsigned id, SimpleTexture* t, int pz) : SimpleSprite(id, t), z(pz) {}
	~ComplexSprite();

	int getZ() const { return z; }

	void setPosition(SimplePosition sp);
	void setBounds(float width, float height);
	void setRotation(float rotation);
	void setColor(SimpleColor c);
	void setTexData(float x, float y, float width, float height);

	void draw();

	bool operator==(const ComplexSprite& other) const {
		return id == other.id;
	}

};

#endif
