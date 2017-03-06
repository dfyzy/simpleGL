#include <set>

#include "simpleGL.hpp"
#include "simpleUtil.hpp"

using namespace simpleGL;

namespace simpleUtil {
	GLuint msaaFbo;
	GLuint rectFbo;

	UnsortedSprite* overlay;

	std::set<Sprite*, Sprite::Comparer> sprites;

	GLuint getMsaaFbo() {
		return msaaFbo;
	}

	void initFbos() {
		glGenFramebuffers(1, &msaaFbo);
		glBindFramebuffer(GL_FRAMEBUFFER, msaaFbo);

		GLuint renderbuffer;
		glGenRenderbuffers(1, &renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);

		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, getWindowWidth(), getWindowHeight());

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);

		glGenFramebuffers(1, &rectFbo);
		glBindFramebuffer(GL_FRAMEBUFFER, rectFbo);

		glActiveTexture(GL_TEXTURE1);

		Image image(getWindowWidth(), getWindowHeight(), GL_RGB);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, image.getId(), 0);

		glActiveTexture(GL_TEXTURE0);

		float width = getWindowWidth(), height = getWindowHeight();
		overlay = new UnsortedSprite({Vector(width, height)}, {}, {1}, 0, {1});
		setOverlayShaders(overlay);

		print("Fbos initialized");
	}

	void drawSprites() {
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
		overlay->draw();
		glEnable(GL_BLEND);
	}

}

using namespace simpleUtil;

Sprite::Sprite(Texture texture, Vector position, int z, Vector scale, double rotation, Color color)
					: UnsortedSprite(texture, position, scale, rotation, color), z(z) {
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
