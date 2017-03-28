#include "shader.h"
#include "text.h"
#include "util.h"
#include "shaderData.h"

namespace simpleGL {

GLuint Text::textFragmentShader = 0;

GLuint Text::getDefaultFragment() {
	if (textFragmentShader == 0)
		textFragmentShader = loadShaderSource(simpleShaderData::getTextFragment(), GL_FRAGMENT_SHADER);

	return textFragmentShader;
}

Text::Text(Font* font, std::string caption, Alignment alignment, float width,
					UnsortedSprite* parent, Vector position, int z, Vector scale, Angle rotation, Color color)
						: UnsortedSprite(parent, nullptr, position, scale, rotation, {}),
								font(font), caption(caption), alignment(alignment), width(width) {
	for (auto c = caption.begin(); c != caption.end(); c++) {
		Glyph* glyph;
		if (!font->getGlyph(*c, &glyph)) continue;

		Sprite* sprite = new Sprite(Sprite::Data(glyph->texture).parent(this).z(z).color(color));//a bit to the right????
		sprite->setFragmentShader(getDefaultFragment());
	}

	align();
}

void Text::align() {
	//init
	Vector newLine(0, -font->getLineSpacing());

	Vector cursor = newLine;

	auto sprite = getChildren().begin();
	auto lineStart = caption.begin();

	//for each line
	while (lineStart != caption.end()) {
		auto lineEnd = lineStart;
		float lineWidth = 0;
		int spaces = 0;

		//incrementing until end of line is found
		for (auto c = lineStart; ; c++) {
			if (c == caption.end()) {
				lineEnd = c;
				break;
			}

			Glyph* glyph;
			if (!font->getGlyph(*c, &glyph)) {
				lineEnd = c;

				if (*c == ' ')			spaces++;
				else if (*c == '\n')	break;//easy way
				continue;
			}

			lineWidth += glyph->advance;

			if ((width != 0) && (lineWidth > width) && (lineEnd != lineStart)) {//hard way(overflow)
				//substracting width of overflowing word
				for (auto s = lineEnd; s != c;) {
					s++;

					if (!font->getGlyph(*s, &glyph) && *s == ' ') 			spaces--;

					lineWidth -= glyph->advance;
				}

				//substracting all needless spaces
				c = lineEnd;
				while (!font->getGlyph(*c, &glyph))	if (*(c--) == ' ')	spaces--;

				break;
			}
			if (*c == '-')	lineEnd = c;
		}

		float spaceWidth = font->getSpaceWidth();

		lineWidth += spaceWidth*spaces;

		//alignment
		Vector lastPosition = cursor;

		if (alignment == JUSTIFIED) {
			if (lineEnd != caption.end()) {
				spaceWidth += (width - lineWidth)/spaces;
				lineWidth = width;
			} else
				cursor -= Vector(width/2, 0);//this will depend on language default alignment.
		} else if (alignment != LEFT)
			cursor -= Vector((lineWidth/(2 - (alignment == RIGHT))), 0);

		//actually setting sprite position
		if (lineEnd != caption.end())	lineEnd++;
		for (; lineStart != lineEnd; lineStart++) {
			Glyph* glyph;

			if (!font->getGlyph(*lineStart, &glyph)) {
				if (*lineStart == ' ')	cursor += Vector(spaceWidth, 0);

				continue;
			}

			(*(sprite++))->setPosition(cursor + glyph->offset);

			cursor += Vector(glyph->advance, 0);
		}

		cursor = lastPosition + newLine;
	}
}

}
