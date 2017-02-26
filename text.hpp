#ifndef SIMPLE_TEXT_H
#define SIMPLE_TEXT_H

#include <string>

#include "sprite.hpp"
#include "font.hpp"

namespace simpleGL {

class Text {
public:
	enum Alignment { LEFT, CENTER, RIGHT, JUSTIFIED };

private:
	Font* font;

	std::list<Sprite*> sprites;
	std::string caption;

public:
	Text(Font* font, std::string caption, Vector position, int z,
						float scale, float rotation, Color color, Alignment align, float width);

	Text(Font* font, std::string caption, Vector position, int z,
						float scale, Color color, Alignment align, float width)
							: Text(font, caption, position, z, scale, 0, color, align, width) {}
	Text(Font* font, std::string caption, Vector position, int z, Color color, Alignment align, float width)
					: Text(font, caption, position, z, 1, color, align, width) {}

	void setLayout(Vector position, float scale, float rotation, Alignment align, float width);

};

}

#endif
