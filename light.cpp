#include "simpleGL.hpp"
#include "simpleUtil.hpp"

using namespace simpleUtil;
using namespace simpleGL;

Light::Source::Source(Light* light, Vector position, Vector bounds, float rotation, Rgb rgb)
			: UnsortedSprite(0, position - light->position, {1}, rotation, rgb, {}, bounds), light(light) {
	simpleUtil::setLightingShaders(this);
	light->sources.push_back(this);
	light->toggleDraw();
}

Light::Light(Vector position, int z, unsigned width, unsigned height, Rgb baseRgb)
				: Texture(width, height, GL_RGB), Sprite(texture, position, z, {1}, 0, {1}, {}, Vector(width, height)),
							position(position), baseRgb(baseRgb) {
	setFiltering(GL_LINEAR);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
}

Light::~Light() {
	//TODO: delete fbo
	for (Source* s : sources)
		delete s;
}

void Light::draw() {
	if (needToDraw) {
		needToDraw = false;

		glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
		//glBlendEquation(GL_MAX);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
		glViewport(0, 0, width, height);
		setResolution(width, height);

		glClearColor(baseRgb.r, baseRgb.g, baseRgb.b, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		for (Source* s : sources)
			if (s->isEnabled())	s->draw();

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, getMsaaFbo());
		glViewport(0, 0, simpleGL::getWindowWidth(), simpleGL::getWindowHeight());
		setDefaultResolution();

		//glBlendEquation(GL_FUNC_ADD);
	}

	glBlendFunc(GL_ZERO, GL_SRC_COLOR);

	Sprite::draw();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}