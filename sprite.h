/* Sprite, sorted by z and drawn every frame
*/

#ifndef SIMPLE_SPRITE_H
#define SIMPLE_SPRITE_H

#include "unsortedSprite.h"

namespace simpleGL {

class Sprite : public UnsortedSprite {
private:
	int z;

protected:
	~Sprite();

public:
	struct Loader {
		Point* pparent {nullptr};
		Vector pposition;
		Vector pscale {1};
		Angle protation;
		Texture ptexture;
		Anchor panchor {Center};
		Color pcolor {1};
		int pz {0};

		Loader() {}

		Loader& parent(Point* p) { pparent = p; return *this; }
		Loader& position(Vector v) { pposition = v; return *this; }
		Loader& scale(Vector v) { pscale = v; return *this; }
		Loader& rotation(Angle f) { protation = f; return *this; }
		Loader& texture(Texture t) { ptexture = t; return *this; }
		Loader& anchor(Anchor a) { panchor = a; return *this; }
		Loader& color(Color c) { pcolor = c; return *this; }
		Loader& z(int i) { pz = i; return *this; }

		Sprite* load() { return new Sprite(pparent, pposition, pscale, protation, ptexture, panchor, pcolor, pz); }
	};

	Sprite(Point* parent, Vector position, Vector scale, Angle rotation, Texture texture, Anchor anchor, Color color, int z);

	int getZ() const { return z; }
	void setZ(int i);

	void setTexture(Texture t) override;

	struct Comparer {
		bool operator()(const Sprite* lhs, const Sprite* rhs) {
			if (lhs->z != rhs->z)
				return lhs->z > rhs->z;

			if (lhs->getTexture().getImage() != rhs->getTexture().getImage())
				return lhs->getTexture().getImage() < rhs->getTexture().getImage();

			return lhs->getId() < rhs->getId();
		}
	};
};

}

#endif
