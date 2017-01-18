#include "simpleLight.hpp"
#include "simpleUtil.hpp"

namespace simpleUtil {
	std::list<SimpleLight*> lights;

	GLuint lightFbo;

	void drawLights() {
		glBlendFunc(GL_ONE, GL_ONE);

		for (SimpleLight* l : lights)
			l->draw();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		setDefaultResolution();
	}

}

using namespace simpleUtil;

SimpleLight::Source* SimpleLight::Source::load(SimpleLight& light, SimpleVector position, SimpleVector bounds, float rotation, SimpleRGB rgb) {
	Source* source = new Source(light, position, bounds, rotation, rgb);
	light.sources.push_back(source);
	light.toggleDraw();
	return source;
}

SimpleLight::SimpleLight(SimpleVector position, int z, unsigned width, unsigned height, SimpleRGB baseRGB)
														: SimpleTexture(width, height), position(position), baseRGB(baseRGB) {
	genTexture(&texture, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

	sprite = SimpleSprite::Loader(*this).position(position).z(z).load();
	simpleUtil::setLightShader(sprite);

	lights.push_back(this);
}

void SimpleLight::unload() {
	lights.remove(this);
	for (Source* s : sources)
		s->unload();
	sprite->unload();
	SimpleTexture::unload();
}

void SimpleLight::draw() {
	if (!needToDraw)	return;
	needToDraw = false;

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

	glViewport(0, 0, width, height);
	setResolution(width, height);

	glClearColor(baseRGB.r, baseRGB.g, baseRGB.b, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	for (Source* s : sources)
		s->draw();
}
