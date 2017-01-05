#ifndef SIMPLE_TEXT_H
#define SIMPLE_TEXT_H

#include <list>
#include <string>

#include "simpleSprite.hpp"
#include "simpleFont.hpp"

class SimpleText {
public:
	enum Alignment { LEFT, CENTER, RIGHT };

private:
	SimpleFont* font;

	std::list<SimpleSprite*> sprites;
	std::string caption;

public:
	SimpleText(SimpleFont* font, std::string caption, SimpleVector position, int z,
						float scale, float rotation, SimpleColor color, Alignment align, float width);

	SimpleText(SimpleFont* font, std::string caption, SimpleVector position, int z,
						float scale, SimpleColor color, Alignment align, float width)
							: SimpleText(font, caption, position, z, scale, 0, color, align, width) {}
	SimpleText(SimpleFont* font, std::string caption, SimpleVector position, int z, SimpleColor color, Alignment align, float width)
					: SimpleText(font, caption, position, z, 1, color, align, width) {}

	void setLayout(SimpleVector position, float scale, float rotation, Alignment align, float width);

};

#endif
