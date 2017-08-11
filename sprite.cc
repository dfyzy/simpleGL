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

Sprite::Sprite(Point* parent, Vector position, Vector scale, Angle rotation, Texture texture, Anchor anchor, Color color, int z)
	: UnsortedSprite(parent, position, scale, rotation, texture, anchor, color), z(z) {
	sprites.insert(this);
}

Sprite::~Sprite() {
	sprites.erase(this);
}

void Sprite::setZ(int i) {
	sprites.erase(this);
	z = i;
	sprites.insert(this);
}

void Sprite::setTexture(Texture t) {
	sprites.erase(this);
	UnsortedSprite::setTexture(t);
	sprites.insert(this);
}

}
