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
	Sprite(Texture t, Data d, int z);

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
