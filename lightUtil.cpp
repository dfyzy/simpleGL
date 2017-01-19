#include "simpleGL.hpp"
#include "simpleUtil.hpp"

using namespace simpleUtil;

SimpleLight::Source::Source(SimpleLight* light, SimpleVector position, SimpleVector bounds, float rotation, SimpleRGB rgb)
			: SimplerSprite(0, position - light->position, {1}, rotation, rgb, {}, bounds), light(light) {
	simpleUtil::setLightingShaders(this);
	light->sources.push_back(this);
	light->toggleDraw();
}

SimpleLight::SimpleLight(SimpleVector position, int z, unsigned width, unsigned height, SimpleRGB baseRGB)
				: SimpleTexture(width, height, GL_RGB), SimpleSprite(texture, position, z, {1}, 0, {1}, {}, SimpleVector(width, height)),
							position(position), baseRGB(baseRGB) {
	setFiltering(GL_LINEAR);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
}

SimpleLight::~SimpleLight() {
	//TODO: delete fbo
	for (Source* s : sources)
		delete s;
}

void SimpleLight::draw() {
	if (needToDraw) {
		needToDraw = false;

		glBlendFunc(GL_ONE, GL_ONE);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
		glViewport(0, 0, width, height);
		setResolution(width, height);

		glClearColor(baseRGB.r, baseRGB.g, baseRGB.b, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		for (Source* s : sources)
			s->draw();

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, getMsaaFbo());
		glViewport(0, 0, simpleGL::getWindowWidth(), simpleGL::getWindowHeight());
		setDefaultResolution();
	}

	glBlendFunc(GL_ZERO, GL_SRC_COLOR);

	SimpleSprite::draw();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
