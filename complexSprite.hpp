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

	void changeAttrib(Attrib att);

public:
	ComplexSprite(unsigned id, SimpleTexture* t, int pz) : SimpleSprite(id, t), z(pz) {}
	~ComplexSprite();

	int getZ() const { return z; }

	static void loadPosition(SimplePosition sp, float* array, int offset);
	static void loadBounds(float width, float height, SimpleTexture* tex, float* array, int offset);
	static void loadRotation(float rotation, float* array, int offset);
	static void loadColor(SimpleColor c, float* array, int offset);
	static void loadTexData(float x, float y, float width, float height, float* array, int offset);

	void changePosition(SimplePosition sp) {
		z = sp.z;
		resort();

		Attrib att(Attrib::POSITION);
		loadPosition(sp, att.data.get(), 0);

		changeAttrib(std::move(att));
	}

	void changeBounds(float width, float height) {
		Attrib att(Attrib::BOUNDS);
		loadBounds(width, height, texture, att.data.get(), 0);

		changeAttrib(std::move(att));
	}

	void changeRotation(float rotation) {
		Attrib att(Attrib::ROTATION);
		loadRotation(rotation, att.data.get(), 0);

		changeAttrib(std::move(att));
	}

	void changeColor(SimpleColor c) {
		Attrib att(Attrib::COLOR);
		loadColor(c, att.data.get(), 0);

		changeAttrib(std::move(att));
	}

	void changeTexData(float x, float y, float width, float height) {
		Attrib att(Attrib::TEX_DATA);
		loadTexData(x, y, width, height, att.data.get(), 0);

		changeAttrib(std::move(att));
	}

	void draw();

	bool operator==(const ComplexSprite& other) const {
		return id == other.id;
	}

};

#endif
