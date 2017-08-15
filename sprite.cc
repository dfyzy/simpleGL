#include "sprite.h"

namespace simpleGL {

std::set<simpleGL::Sprite*, simpleGL::Sprite::Comparer> Sprite::sprites;

void Sprite::drawAll() {
	for (Sprite* s : sprites)
		s->draw();
}

}
