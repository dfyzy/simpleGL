#include "text.h"
#include "shaderData.h"
#include "util.h"

namespace {

constexpr float MIN_JUSTIFIED_SPACE = 0.25f;

}

namespace simpleGL {

GLuint Text::textFragmentShader = 0;

GLuint Text::getDefaultFragment() {
	if (textFragmentShader == 0)
		textFragmentShader = loadShaderSource(shaderData::getTextFragment(), GL_FRAGMENT_SHADER);

	return textFragmentShader;
}

void Text::addCaption(std::string string) {
	util::println("Text:addCaption");

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

			float spss = spaces*spaceWidth*(alignment == EAlignment::Justified ? MIN_JUSTIFIED_SPACE : 1);
			if ((bounds.x != 0) && (lineEnd != cursorStr) && (lineWidth + spss > bounds.x)) {//hard way(overflow)
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

		if (alignment == EAlignment::Justified) {
			if (notEOS) {
				if (spaces != 0)	spaceWidth += (bounds.x - lineWidth)/spaces;
				lineWidth = bounds.x;
			} else
				offset -= (bounds.x - lineWidth)/2;//this will depend on language default alignment.
		}

		if (alignment != EAlignment::Left)
			offset -= lineWidth/(2 - (alignment == EAlignment::Right));

		//actually setting sprite position
		if (notEOS)	lineEnd++;
		for (; cursorStr != lineEnd; cursorStr++) {
			Glyph* glyph;

			if (!font->getGlyph(caption[cursorStr], &glyph)) {
				if (caption[cursorStr] == ' ')	offset += spaceWidth;

				continue;
			}

			Vector position = Vector(offset, -bounds.y) + glyph->offset;

			if (cursorSpr == sprites.end()) {
				Sprite* spr = Sprite::Loader().texture(glyph->texture).anchor(EAnchor::TopLeft).parent(this)
															.position(position).color(color).z(z).load();
				spr->setFragmentShader(getDefaultFragment());
				sprites.push_back(spr);

				if (lastLineSpr == sprites.end())	--lastLineSpr;
			} else (*(cursorSpr++))->setPosition(position);

			offset += glyph->advance;
		}

		if (!notEOS)	break;

		lastLineStr = cursorStr;
		lastLineSpr = cursorSpr;

		bounds.y += font->getLineSpacing();
	}
}

}
