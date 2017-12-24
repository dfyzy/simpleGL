/* Sprite, sorted by z and drawn every frame
*/

#ifndef SIMPLE_SPRITE_H
#define SIMPLE_SPRITE_H

#include <set>

#include "unsortedSprite.h"

namespace simpleGL {

class Sprite : public UnsortedSprite {
private:
	struct Comparer {
		bool operator()(const Sprite* lhs, const Sprite* rhs) {
			if (lhs->z != rhs->z)
				return lhs->z > rhs->z;

			if (lhs->getTexture().getImage() != rhs->getTexture().getImage())
				return lhs->getTexture().getImage() < rhs->getTexture().getImage();

			return lhs->getId() < rhs->getId();
		}
	};

	static std::set<simpleGL::Sprite*, simpleGL::Sprite::Comparer> sprites;

	int z;

protected:
	~Sprite() {
		sprites.erase(this);
	}

public:
	static void drawAll();

	struct Loader {
		Point* pparent {nullptr};
		Vector pposition;
		Vector pscale {1};
		Angle protation;
		Texture ptexture;
		Vector ppivot;
		Color pcolor {1};
		int pz {0};

		Loader() {}

		Loader& parent(Point* p) { pparent = p; return *this; }
		Loader& position(Vector v) { pposition = v; return *this; }
		Loader& scale(Vector v) { pscale = v; return *this; }
		Loader& rotation(Angle a) { protation = a; return *this; }
		Loader& texture(Texture t) { ptexture = t; return *this; }
		Loader& pivot(Vector v) { ppivot = v; return *this; }
		Loader& color(Color c) { pcolor = c; return *this; }
		Loader& z(int i) { pz = i; return *this; }

		Sprite* load() { return new Sprite(pparent, pposition, pscale, protation, ptexture, ppivot, pcolor, pz); }
	};

	Sprite(Point* parent, Vector position, Vector scale, Angle rotation, Texture texture, Vector pivot, Color color, int z)
		: UnsortedSprite(parent, position, scale, rotation, texture, pivot, color), z(z) {
		sprites.insert(this);
	}

	int getZ() const { return z; }
	void setZ(int i) {
		sprites.erase(this);
		z = i;
		sprites.insert(this);
	}

	void setTexture(Texture t) override {
		sprites.erase(this);
		UnsortedSprite::setTexture(t);
		sprites.insert(this);
	}

};

}

#endif
