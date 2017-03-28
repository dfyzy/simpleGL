#ifndef SIMPLE_SPRITE_H
#define SIMPLE_SPRITE_H

#include "unsortedSprite.h"
#include "texture.h"

namespace simpleGL {

class Sprite : public UnsortedSprite {
private:
	int z;

protected:
	~Sprite();

public:
	/*
	 * Creates sprite object and loads attribute data into data buffers.
	 *
	 * returns: sprite handle.
	 */
	Sprite(UnsortedSprite* parent, Texture texture, Vector position, int z, Vector scale, Angle rotation, Color color);

	struct Data {
		UnsortedSprite* pparent {nullptr};
		Texture ptexture;
		Vector pposition;
		int pz{0};
		Vector pscale{1};
		Angle protation;
		Color pcolor{1};

		Data(Texture texture) : ptexture(texture) {}

		Data& parent(UnsortedSprite* us) { pparent = us; return *this; }
		Data& position(Vector sv) { pposition = sv; return *this; }
		Data& z(int i) { pz = i; return *this; }
		Data& scale(Vector sv) { pscale = sv; return *this; }
		Data& rotation(Angle f) { protation = f; return *this; }
		Data& color(Color sc) { pcolor = sc; return *this; }
	};

	Sprite(Data d) : Sprite(d.pparent, d.ptexture, d.pposition, d.pz, d.pscale, d.protation, d.pcolor) {}

	void setEnabled(bool b);

	int getZ() const { return z; }
	void setZ(int pz);

	void setTexture(Texture tex);

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
