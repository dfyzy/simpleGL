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

Text::Text(Font* font, std::string caption, Vector position, int z, float scale, Angle rotation, Color color, Alignment align, float width)
 										: font(font), caption(caption) {
	for (auto c = caption.begin(); c != caption.end(); c++) {
		GlyphData* data;
		if (!font->getGlyphData(*c, &data)) continue;

		Sprite* sprite = new Sprite(nullptr, data->texture, Vector(0), z, scale, rotation, color);//a bit to the right????
		sprite->setFragmentShader(getDefaultFragment());

		sprites.push_back(sprite);
	}

	setLayout(position, scale, rotation, align, width);
}

void Text::setLayout(Vector position, float scale, Angle rotation, Alignment align, float width) {
	//init
	Vector newLine = Vector(-rotation.sin(), rotation.cos()) * (font->getLineSpacing()*scale);

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

			GlyphData* data;
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
		Vector lastPosition = position;
		if (align == JUSTIFIED) {
			if (lineEnd != caption.end()) {
				spaceWidth += (width - lineWidth)/spaces;
				lineWidth = width;
			} else {
				align = LEFT;//this will depend on language default alignment.
				position -= Vector(rotation.cos(), rotation.sin()) * (width/2);//will in the general meaning of intention, not like i have time to do this
			}
		}

		if (align != LEFT) {
			position -= Vector(rotation.cos(), rotation.sin()) * (lineWidth/(2 - (align == RIGHT)));
		}

		//actually setting position
		Vector space = Vector(rotation.cos(), rotation.sin()) * spaceWidth;
		if (lineEnd != caption.end())	lineEnd++;
		for (; lineStart != lineEnd; lineStart++) {
			GlyphData* data;

			if (!font->getGlyphData(*lineStart, &data)) {
				if (*lineStart == ' ')	position += space;

				continue;
			}

			Vector offset = data->offset.rotate(rotation);
			(*(sprite++))->setPosition(position + offset*scale);

			position += Vector(rotation.cos(), rotation.sin()) * (data->advance*scale);
		}

		position = lastPosition - newLine;
	}
}

}
