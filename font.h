#ifndef SIMPLE_FONT_H
#define SIMPLE_FONT_H

#include "vector.h"
#include "color.h"
#include "texture.h"

namespace simpleGL {

constexpr int FIRST_CHAR = 33;
constexpr int LAST_CHAR = 126;

struct GlyphData {
	Texture texture;
	Vector offset;
	long advance;

};

class Font : public Image {
private:
	GlyphData glyphData[LAST_CHAR - FIRST_CHAR + 1];

	float lineSpacing;
	unsigned spaceWidth;

public:
	Font(const char* path, int size);

	float getLineSpacing() { return lineSpacing; }
	unsigned getSpaceWidth() { return spaceWidth; }

	bool getGlyphData(char c, GlyphData** data) {
		if ((c > LAST_CHAR) || (c < FIRST_CHAR))	return false;

		*data = glyphData + (c - FIRST_CHAR);
		return true;
	}

};

}

#endif
