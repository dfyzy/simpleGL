#include <set>

#include "simpleGL.h"
#include "util.h"

namespace {

std::set<simpleGL::Sprite*, simpleGL::Sprite::Comparer> sprites;

}

namespace simpleGL {

void util::drawSprites() {
	Camera* camera = Camera::getInstance();

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, camera->getMSAAFbo());

	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	for (Sprite* cs : sprites)
		cs->draw();

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, camera->getRectFbo());
	glBindFramebuffer(GL_READ_FRAMEBUFFER, camera->getMSAAFbo());

	unsigned width = getWindowWidth(), height = getWindowHeight();
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glDisable(GL_BLEND);
	camera->draw();
	glEnable(GL_BLEND);
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
