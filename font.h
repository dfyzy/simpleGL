#ifndef SIMPLE_FONT_H
#define SIMPLE_FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H

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
	static FT_Library ftLibrary;
	static int dpiX, dpiY;

	GlyphData glyphData[LAST_CHAR - FIRST_CHAR + 1];

	float lineSpacing;
	unsigned spaceWidth;

protected:
	~Font() {}

public:
	static FT_Library getFTLibrary();

	Font(const char* path, int size);

	float getLineSpacing() const { return lineSpacing; }
	unsigned getSpaceWidth() const { return spaceWidth; }

	bool getGlyphData(char c, GlyphData** data) {
		if ((c > LAST_CHAR) || (c < FIRST_CHAR))	return false;

		*data = glyphData + (c - FIRST_CHAR);
		return true;
	}

};

}

#endif
