#include "simpleGL.hpp"
#include "simpleUtil.hpp"

SimpleText::SimpleText(SimpleFont* font, std::string caption, SimpleVector position, int z,
									float scale, float rotation, SimpleColor color, Alignment align, float width)
 										: font(font), caption(caption) {
	for (auto c = caption.begin(); c != caption.end(); c++) {
		SimpleGlyphData* data;
		if (!font->getGlyphData(*c, &data)) continue;

		SimpleSprite* sprite = SimpleSprite::load(font, SimpleVector(0), z, scale,
										rotation, color, SimpleVector(data->x, 0), SimpleVector(data->width, data->height));//a bit to the right????
		simpleUtil::setTextShader(sprite);

		sprites.push_back(sprite);
	}

	setLayout(position, scale, rotation, align, width);
}

void SimpleText::setLayout(SimpleVector position, float scale, float rotation, Alignment align, float width) {
	float sinRot = std::sin(rotation);
	float cosRot = std::cos(rotation);

	position -= SimpleVector(-sinRot, cosRot) * (font->getLineSpacing()*scale);
	SimpleVector startOfLine = position;

	auto ss = sprites.begin();
	for (auto c = caption.begin(); c != caption.end(); c++) {
		SimpleGlyphData* data;
		if (!font->getGlyphData(*c, &data)) {
			if (*c == '\n') {
				startOfLine -= SimpleVector(-sinRot, cosRot) * (font->getLineSpacing()*scale);
				position = startOfLine;
			} else if (*c == ' ')
				position += SimpleVector(cosRot, sinRot) * (font->getSpaceWidth()*scale);

			continue;
		}

		SimpleVector offset(cosRot*data->offset.x - sinRot*data->offset.y, sinRot*data->offset.x + cosRot*data->offset.y);

		(*(ss++))->setPosition(position + offset*scale);

		position += SimpleVector(cosRot, sinRot) * (data->advance*scale);
	}
}
