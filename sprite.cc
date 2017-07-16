#include <set>

#include "simpleGL.h"
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

void Sprite::setZ(int pz) {
	sprites.erase(this);
	z = pz;
	sprites.insert(this);
}

void Sprite::setTexture(Texture tex) {
	sprites.erase(this);
	UnsortedSprite::setTexture(tex);
	sprites.insert(this);
}

}
