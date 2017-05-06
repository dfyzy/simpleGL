#include "shader.h"
#include "text.h"
#include "util.h"
#include "shaderData.h"
#include <iostream>
namespace {

constexpr float MIN_JUSTIFIED_SPACE = 0.25f;

}

namespace simpleGL {

GLuint Text::textFragmentShader = 0;

GLuint Text::getDefaultFragment() {
	if (textFragmentShader == 0)
		textFragmentShader = loadShaderSource(simpleShaderData::getTextFragment(), GL_FRAGMENT_SHADER);

	return textFragmentShader;
}

Text::Text(Font* font, std::string caption, Alignment alignment, float width,
				Point* parent, Vector position, int z, Vector scale, Angle rotation, Color color)
					: Point(parent, position, scale, rotation), font(font), alignment(alignment), width(width), z(z), color(color) {
	setCaption(caption);
}

void Text::setCaption(std::string string) {
	unloadChildren();
	height = font->getLineSpacing();
	lastLineStr = 0;
	lastLineSpr = getFirstChild();

	caption.clear();

	addCaption(string);
}

void Text::addCaption(std::string string) {
	caption += string;

	unsigned cursorStr = lastLineStr;
	auto cursorSpr = lastLineSpr;

	//for each line
	while (true) {
		unsigned lineEnd = cursorStr;

		float lineWidth = 0;
		int spaces = 0;

		float spaceWidth = font->getSpaceWidth();

		//incrementing until end of line is found
		for (unsigned c = cursorStr; ; c++) {
			if (c == caption.size()) {
				lineEnd = c;
				break;
			}

			Glyph* glyph;
			if (!font->getGlyph(caption[c], &glyph)) {
				lineEnd = c;

				if (caption[c] == ' ')			spaces++;
				else if (caption[c] == '\n')	break;//easy way
				continue;
			}

			lineWidth += glyph->advance;

			float spss = spaces*spaceWidth*(alignment == JUSTIFIED ? MIN_JUSTIFIED_SPACE : 1);
			if ((width != 0) && (lineEnd != cursorStr) && (lineWidth + spss > width)) {//hard way(overflow)
				//substracting width of overflowing word
				for (unsigned s = lineEnd; s != c;) {
					s++;

					if (!font->getGlyph(caption[s], &glyph) && caption[s] == ' ') 	spaces--;

					lineWidth -= glyph->advance;
				}

				//substracting all needless spaces
				c = lineEnd;
				while (!font->getGlyph(caption[c], &glyph))	if (caption[c--] == ' ')	spaces--;

				break;
			}
			if (caption[c] == '-')	lineEnd = c;
		}

		lineWidth += spaceWidth*spaces;

		//alignment
		bool notEOS = lineEnd != caption.size();

		float offset = 0;

		if (alignment == JUSTIFIED) {
			if (notEOS) {
				if (spaces != 0)	spaceWidth += (width - lineWidth)/spaces;
				lineWidth = width;
			} else
				offset -= (width - lineWidth)/2;//this will depend on language default alignment.
		}

		if (alignment != LEFT)
			offset -= lineWidth/(2 - (alignment == RIGHT));

		//actually setting sprite position
		if (notEOS)	lineEnd++;
		for (; cursorStr != lineEnd; cursorStr++) {
			Glyph* glyph;

			if (!font->getGlyph(caption[cursorStr], &glyph)) {
				if (caption[cursorStr] == ' ')	offset += spaceWidth;

				continue;
			}

			Vector position = Vector(offset, -height) + glyph->offset;

			if (cursorSpr == getLastChild()) {
				Sprite* spr = new Sprite(Sprite::Data(glyph->texture).anchor(Sprite::UL).parent(this).position(position).z(z).color(color));
				spr->setFragmentShader(getDefaultFragment());

				if (lastLineSpr == getLastChild())	--lastLineSpr;
			} else (*(cursorSpr++))->setPosition(position);

			offset += glyph->advance;
		}

		if (!notEOS)	break;

		lastLineStr = cursorStr;
		lastLineSpr = cursorSpr;

		height += font->getLineSpacing();
	}
}

}
