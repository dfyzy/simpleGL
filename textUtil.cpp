#include "simpleGL.hpp"
#include "simpleUtil.hpp"

SimpleText::SimpleText(SimpleFont* font, std::string caption, SimpleVector position, int z,
									float scale, float rotation, SimpleColor color, Alignment align, float width)
 										: font(font), caption(caption) {
	for (auto c = caption.begin(); c != caption.end(); c++) {
		SimpleGlyphData* data;
		if (!font->getGlyphData(*c, &data)) continue;

		SimpleSprite* sprite = SimpleSprite::load(font->getTexture(), SimpleVector(0), z, scale,
										rotation, color, SimpleVector(data->x, 0), SimpleVector(data->width, data->height));//a bit to the right????
		simpleUtil::setTextShader(sprite);

		sprites.push_back(sprite);
	}

	setLayout(position, scale, rotation, align, width);
}

void SimpleText::setLayout(SimpleVector position, float scale, float rotation, Alignment align, float width) {
	//init
	float sinRot = std::sin(rotation);
	float cosRot = std::cos(rotation);

	SimpleVector newLine = SimpleVector(-sinRot, cosRot) * (font->getLineSpacing()*scale);

	//position is our cursor
	position -= newLine;

	auto sprite = sprites.begin();
	auto lineStart = caption.begin();

	//for each line
	while (lineStart != caption.end()) {
		auto lineEnd = caption.end();
		float lineWidth = 0;
		int spaces = 0;

		//incrementing until end of line is found
		for (auto c = lineStart; ; c++) {
			if (c == caption.end()) {
				lineEnd = c;
				break;
			}

			SimpleGlyphData* data;
			if (!font->getGlyphData(*c, &data)) {
				lineEnd = c;

				if (*c == ' ')			spaces++;
				else if (*c == '\n')	break;//easy way
				continue;
			}

			lineWidth += data->advance*scale;

			if ((width != 0) && (lineWidth > width) && (lineEnd != caption.end())) {//hard way(overflow)
				//substracting width of overflowing word
				for (auto s = lineEnd; s != c;) {
					s++;

					if (!font->getGlyphData(*s, &data) && *s == ' ') 			spaces--;

					lineWidth -= data->advance*scale;
				}

				//substracting all needless spaces
				c = lineEnd;
				while (!font->getGlyphData(*c, &data))	if (*(c--) == ' ')	spaces--;

				break;
			}
			if (*c == '-')	lineEnd = c;
		}

		lineWidth += font->getSpaceWidth()*spaces*scale;

		float spaceWidth = font->getSpaceWidth()*scale;

		//alignment
		SimpleVector lastPosition = position;
		if (align == JUSTIFIED) {
			if (lineEnd != caption.end()) {
				spaceWidth += (width - lineWidth)/spaces;
				lineWidth = width;
			} else {
				align = LEFT;//this will depend on language default alignment.
				position -= SimpleVector(cosRot, sinRot) * (width/2);//will in the general meaning of intention, not like i have time to do this
			}
		}

		if (align != LEFT) {
			position -= SimpleVector(cosRot, sinRot) * (lineWidth/(2 - (align == RIGHT)));
		}

		//actually setting position
		SimpleVector space = SimpleVector(cosRot, sinRot) * spaceWidth;
		if (lineEnd != caption.end())	lineEnd++;
		for (; lineStart != lineEnd; lineStart++) {
			SimpleGlyphData* data;

			if (!font->getGlyphData(*lineStart, &data)) {
				if (*lineStart == ' ')	position += space;

				continue;
			}

			SimpleVector offset(cosRot*data->offset.x - sinRot*data->offset.y, sinRot*data->offset.x + cosRot*data->offset.y);
			(*(sprite++))->setPosition(position + offset*scale);

			position += SimpleVector(cosRot, sinRot) * (data->advance*scale);
		}

		position = lastPosition - newLine;
	}
}
