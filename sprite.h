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

	class Loader {
	private:
		UnsortedSprite* pparent {nullptr};
		Texture ptexture;
		Vector pposition;
		int pz{0};
		Vector pscale{1};
		Angle protation;
		Color pcolor{1};

	public:
		Loader(Texture texture) : ptexture(texture) {}

		Loader& parent(UnsortedSprite* us) { pparent = us; return *this; }
		Loader& position(Vector sv) { pposition = sv; return *this; }
		Loader& z(int i) { pz = i; return *this; }
		Loader& scale(Vector sv) { pscale = sv; return *this; }
		Loader& rotation(Angle f) { protation = f; return *this; }
		Loader& color(Color sc) { pcolor = sc; return *this; }

		Sprite* load() {
			return new Sprite(pparent, ptexture, pposition, pz, pscale, protation, pcolor);
		}
	};

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
