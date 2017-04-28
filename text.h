#ifndef SIMPLE_TEXT_H
#define SIMPLE_TEXT_H

#include <string>

#include "sprite.h"
#include "font.h"

namespace simpleGL {

class Text : public Point {
public:
	enum Alignment { LEFT, CENTER, RIGHT, JUSTIFIED };

private:
	static GLuint textFragmentShader;

	Font* font;

	std::string caption;
	Alignment alignment;

	float width;

	void align();

protected:
	~Text() {}

public:
	static GLuint getDefaultFragment();

	Text(Font* font, std::string caption, Alignment alignment, float width,
				Point* parent, Vector position, int z, Vector scale, Angle rotation, Color color);

	Font* getFont() const { return font; }

	std::string getCaption() const { return caption; }

	Alignment getAlignment() const { return alignment; }

	float getWidth() const { return width; }

};

}

#endif
