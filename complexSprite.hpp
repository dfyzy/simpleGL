#ifndef COMPLEX_SPRITE_H
#define COMPLEX_SPRITE_H

#include "simpleSprite.hpp"

const float ZPOINT = 0.0001f;

class ComplexTexture;

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
	ComplexTexture* texture;

	void changeAttrib(Attrib att);

public:
	ComplexSprite(ComplexTexture* t) : texture(t) {}//for empty sprite objects
	ComplexSprite(unsigned id, ComplexTexture* t) : SimpleSprite(id), texture(t) {}

	void loadPosition(SimplePosition sp, float* array, int offset);
	void loadBounds(float width, float height, float* array, int offset);
	void loadRotation(float rotation, float* array, int offset);
	void loadColor(SimpleColor c, float* array, int offset);
	void loadTexData(float x, float y, float width, float height, float* array, int offset);

	void changePosition(SimplePosition sp) {
		Attrib att(Attrib::POSITION);
		loadPosition(sp, att.data.get(), 0);

		changeAttrib(std::move(att));
	}

	void changeBounds(float width, float height) {
		Attrib att(Attrib::BOUNDS);
		loadBounds(width, height, att.data.get(), 0);

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

	void deleteData();

	void unload();

	bool operator==(const ComplexSprite& other) const {
		return id == other.id;
	}

};

#endif
