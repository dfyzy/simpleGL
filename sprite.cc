#include <set>

#include "sprite.h"
#include "util.h"

namespace {

std::set<simpleGL::Sprite*, simpleGL::Sprite::Comparer> sprites;

}

namespace simpleGL {

void util::drawSprites() {
	for (Sprite* cs : sprites)
		cs->draw();
}

Sprite::Sprite(Texture t, Data d, int z) : UnsortedSprite(t, d), z(z) {
	sprites.insert(this);
}

Sprite::~Sprite() {
	sprites.erase(this);
}

void Sprite::setZ(int z) {
	sprites.erase(this);
	z = z;
	sprites.insert(this);
}

void Sprite::setTexture(Texture t) {
	sprites.erase(this);
	UnsortedSprite::setTexture(t);
	sprites.insert(this);
}

}
