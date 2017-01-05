#ifndef SIMPLE_FONT_H
#define SIMPLE_FONT_H

#include "simpleVector.hpp"
#include "simpleColor.hpp"
#include "simpleTexture.hpp"

const int SIMPLE_FIRST_CHAR = 33;
const int SIMPLE_LAST_CHAR = 126;

struct SimpleGlyphData {
	unsigned x;
	unsigned width, height;
	SimpleVector offset;
	long advance;

};

class SimpleFont : public SimpleTexture {
private:
	SimpleGlyphData glyphData[SIMPLE_LAST_CHAR - SIMPLE_FIRST_CHAR + 1];

	float lineSpacing;
	unsigned spaceWidth;

public:
	SimpleFont(const char* path, int size);

	float getLineSpacing() { return lineSpacing; }
	unsigned getSpaceWidth() { return spaceWidth; }

	bool getGlyphData(char c, SimpleGlyphData** data) {
		if ((c > SIMPLE_LAST_CHAR) || (c < SIMPLE_FIRST_CHAR))	return false;

		*data = glyphData + (c - SIMPLE_FIRST_CHAR);
		return true;
	}

};

#endif
