#include <set>

#include "simpleGL.h"
#include "util.h"

namespace {

std::set<simpleGL::Sprite*, simpleGL::Sprite::Comparer> sprites;

}

namespace simpleGL {

void util::drawSprites() {
	Camera* cam = Camera::getInstance();

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, cam->getMSAAFbo());

	glClear(GL_COLOR_BUFFER_BIT);

	for (Sprite* cs : sprites)
		cs->draw();

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, cam->getRectFbo());
	glBindFramebuffer(GL_READ_FRAMEBUFFER, cam->getMSAAFbo());

	unsigned width = getWindowWidth(), height = getWindowHeight();
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glDisable(GL_BLEND);
	Camera::getInstance()->draw();
	glEnable(GL_BLEND);
}

Sprite::Sprite(UnsortedSprite* parent, Texture texture, Vector position, int z, Vector scale, Angle rotation, Color color)
					: UnsortedSprite(parent, texture, position, scale, rotation, color), z(z) {
	sprites.insert(this);
}

Sprite::~Sprite() {
	sprites.erase(this);
}

void Sprite::setEnabled(bool b) {
	if (!b && UnsortedSprite::isEnabled())			sprites.erase(this);
	else if (b && !UnsortedSprite::isEnabled())	sprites.insert(this);

	UnsortedSprite::setEnabled(b);
}

void Sprite::setZ(int pz) {
	sprites.erase(this);
	z = pz;
	sprites.insert(this);
}

void Sprite::setTexture(Texture tex) {
	sprites.erase(this);
	setTexture(tex);
	sprites.insert(this);
}

}
