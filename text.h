/* Point-parent of sprites-letters
*/

#ifndef SIMPLE_TEXT_H
#define SIMPLE_TEXT_H

#include <string>

#include "glfw.h"
#include "point.h"
#include "color.h"

namespace simpleGL {

class Text : public Point {
public:
	enum class EAlignment { Left, Center, Right, Justified };

private:
	static GLuint textFragmentShader;

	class Font* font;

	std::string caption;
	EAlignment alignment;

	Vector bounds;

	int z;
	Color color;

	unsigned lastLineStr;
	std::list<class Sprite*>::const_iterator lastLineSpr;

	std::list<Sprite*> sprites;

protected:
	~Text() {}

public:
	static GLuint getDefaultFragment();

	struct Loader {
		Point* pparent {nullptr};
		Vector pposition;
		Vector pscale {1};
		Angle protation;
		Color pcolor {0};
		int pz {0};
		Font* pfont {nullptr};
		std::string pcaption;
		unsigned pwidth {0};
		EAlignment palignment {EAlignment::Center};

		Loader() {}

		Loader& parent(Point* p) { pparent = p; return *this; }
		Loader& position(Vector v) { pposition = v; return *this; }
		Loader& scale(Vector v) { pscale = v; return *this; }
		Loader& rotation(Angle a) { protation = a; return *this; }
		Loader& color(Color c) { pcolor = c; return *this; }
		Loader& z(int i) { pz = i; return *this; }
		Loader& font(Font* f) { pfont = f; return *this; }
		Loader& caption(std::string str) { pcaption = str; return *this; }
		Loader& width(unsigned u) { pwidth = u; return *this; }
		Loader& alignment(EAlignment a) { palignment = a; return *this; }

		Text* load() { return new Text(pparent, pposition, pscale, protation, pcolor, pz, pfont, pcaption, pwidth, palignment); }
	};

	Text(Point* parent, Vector position, Vector scale, Angle rotation, Color color, int z,
		Font* font, std::string caption, float width, EAlignment alignment)
			: Point(parent, position, scale, rotation), font(font), alignment(alignment), bounds(width, 0), z(z), color(color) {
			setCaption(caption);
		}

	Font* getFont() const { return font; }

	int getZ() const { return z; }
	void setZ(int pz);

	Color getColor() const { return color; }
	void setColor(Color c);

	std::string getCaption() const { return caption; }
	void setCaption(std::string str) {
		clear();
		addCaption(str);
	}

	void addCaption(std::string string);

	EAlignment getAlignment() const { return alignment; }

	Vector getBounds() const { return bounds; }

	const std::list<Sprite*>& getSprites() const { return sprites; }

	void clear();

};

}

#endif
