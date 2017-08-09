/* Point-parent of sprites-letters
*/

#ifndef SIMPLE_TEXT_H
#define SIMPLE_TEXT_H

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

	Vector bounds;

	int z;
	Color color;

	unsigned lastLineStr;
	std::list<Sprite*>::const_iterator lastLineSpr;

	std::list<Sprite*> sprites;

protected:
	~Text() {}

public:
	static GLuint getDefaultFragment();

	Text(Font* font, std::string caption, Alignment alignment, float width,
				Point* parent, Vector position, int z, Vector scale, Angle rotation, Color color)
					: Point(parent, position, scale, rotation), font(font), alignment(alignment), bounds(width, 0), z(z), color(color) {
		setCaption(caption);
	}

	Font* getFont() const { return font; }

	int getZ() const { return z; }
	void setZ(int pz) {
		z = pz;
		for (Sprite* sprite : sprites)
			sprite->setZ(z);
	}

	Color getColor() const { return color; }
	void setColor(Color c) {
		color = c;
		for (Sprite* sprite : sprites)
			sprite->setColor(color);
	}

	std::string getCaption() const { return caption; }
	void setCaption(std::string str) {
		clear();
		addCaption(str);
	}

	void addCaption(std::string string);

	Alignment getAlignment() const { return alignment; }

	Vector getBounds() const { return bounds; }

	const std::list<Sprite*>& getSprites() const { return sprites; }

	void clear() {
		for (Sprite* sprite : sprites)
			sprite->unload();

		sprites.clear();
		caption.clear();

		bounds.y = font->getLineSpacing();
		lastLineStr = 0;
		lastLineSpr = sprites.begin();
	}

};

}

#endif
