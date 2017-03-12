#include <set>

#include "simpleGL.h"
#include "util.h"
#include "shaderData.h"

namespace {

GLuint msaaFbo;
GLuint rectFbo;

std::set<simpleGL::Sprite*, simpleGL::Sprite::Comparer> sprites;

}

namespace simpleGL {

GLuint util::getMsaaFbo() {
	return msaaFbo;
}

void util::initFbos() {
	glGenFramebuffers(1, &msaaFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, msaaFbo);

	GLuint renderbuffer;
	glGenRenderbuffers(1, &renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);

	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, getWindowWidth(), getWindowHeight());

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);

	glGenFramebuffers(1, &rectFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, rectFbo);

	Image image(getWindowWidth(), getWindowHeight(), GL_RGB, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, image.getId(), 0);

	Camera::init({&image});
	Camera::getInstance()->setVertexShader(loadShaderSource(simpleShaderData::getOverlayVertex(), GL_VERTEX_SHADER));
	Camera::getInstance()->setFragmentShader(loadShaderSource(simpleShaderData::getOverlayFragment(), GL_FRAGMENT_SHADER));

	util::print("Fbos initialized");
}

void util::drawSprites() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, msaaFbo);

	glClear(GL_COLOR_BUFFER_BIT);

	for (Sprite* cs : sprites)
		cs->draw();

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rectFbo);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaFbo);

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
	if (!b && enabled)		sprites.erase(this);
	else if (b && !enabled)	sprites.insert(this);

	enabled = b;
}

void Sprite::setZ(int pz) {
	sprites.erase(this);
	z = pz;
	sprites.insert(this);
}

void Sprite::setTexture(Texture tex) {
	sprites.erase(this);
	texture = tex;
	sprites.insert(this);
}

}
