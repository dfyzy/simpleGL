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
	int lines {0};

	int z;
	Color color;

	void align();

protected:
	~Text() {}

public:
	static GLuint getDefaultFragment();

	Text(Font* font, std::string caption, Alignment alignment, float width,
				Point* parent, Vector position, int z, Vector scale, Angle rotation, Color color);

	Font* getFont() const { return font; }

	std::string getCaption() const { return caption; }
	void setCaption(std::string str);

	Alignment getAlignment() const { return alignment; }

	Vector getBounds() const { return {width, lines*font->getLineSpacing()}; }

};

}

#endif
