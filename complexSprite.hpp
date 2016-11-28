#ifndef COMPLEX_SPRITE_H
#define COMPLEX_SPRITE_H

#include "simpleSprite.hpp"

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
	void unload();

	void setAttrib(Attrib att) const;

public:
	ComplexSprite(unsigned id, int pz, SimpleTexture* t) : SimpleSprite(id, pz, t) {}
	~ComplexSprite();

	void setZ(int pz);
	void setTexture(SimpleTexture* tex);

	void setPosition(SimpleVector position);
	void setBounds(SimpleVector bounds);
	void setRotation(float rotation);
	void setColor(SimpleColor c);
	void setTexData(float x, float y, float width, float height);

	void draw() const;
	
};

#endif
