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

struct Glyph {
	Texture texture;
	Vector offset;
	long advance;

};

class Font {
private:
	static FT_Library ftLibrary;
	static int dpiX, dpiY;

	Image* image;

	Glyph glyphs[LAST_CHAR - FIRST_CHAR + 1];

	float lineSpacing;
	unsigned spaceWidth;

protected:
	virtual ~Font() { image->unload(); }

public:
	static FT_Library getFTLibrary();

	Font(const char* path, int size);

	Image* getImage() const { return image; }

	float getLineSpacing() const { return lineSpacing; }
	unsigned getSpaceWidth() const { return spaceWidth; }

	bool getGlyph(char c, Glyph** data) {
		if ((c > LAST_CHAR) || (c < FIRST_CHAR))	return false;

		*data = glyphs + (c - FIRST_CHAR);
		return true;
	}

	virtual void unload() { delete this; }

};

}

#endif
