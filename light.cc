#include "light.h"
#include "simpleGL.h"
#include "util.h"
#include "shaderData.h"

namespace simpleGL {

GLuint Light::Source::lightingFragmentShader = 0;

GLuint Light::Source::getDefaultFragment() {
	if (lightingFragmentShader == 0)
		lightingFragmentShader = loadShaderSource(simpleShaderData::getLightingDefaultFragment(), GL_FRAGMENT_SHADER);

	return lightingFragmentShader;
}

Light::Source::Source(Light* light, UnsortedSprite* parent, Anchor anchor, Vector position, Vector bounds, Angle rotation, Color color)
			: UnsortedSprite(parent, Texture(bounds), anchor, position, {1}, rotation, color), light(light) {
	util::print("LightSource:load");

	setFragmentShader(getDefaultFragment());

	if (light) {
		light->sources.push_back(this);
		light->toggleDraw();
	}
}

Light::Source::~Source() {
	util::print("LightSource:unload");

	light->sources.remove(this);
}

void Light::Source::draw() {
	Vector pos = getRealPosition();
	glProgramUniform2f(getFragmentShader(), centreLoc, pos.x, pos.y);
	glProgramUniform2f(getFragmentShader(), boundsLoc, getTexture().getBounds().x, getTexture().getBounds().y);

	UnsortedSprite::draw();
}

Light::Light(UnsortedSprite* parent, Anchor anchor, Vector position, int z, unsigned width, unsigned height, Color base)
				: Sprite(parent, {}, anchor, position, z, {1}, {}, {1}), base(base) {
	util::print("Light:load");

	image = new Image(width, height, GL_RGB, GL_LINEAR);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, image->getId(), 0);
}

Light::~Light() {
	util::print("Light:unload");

	glDeleteFramebuffers(1, &fbo);

	for (Source* s : sources)
		s->unload();

	image->unload();
}

void Light::draw() {
	if (needToDraw) {
		if (first) {
			first = false;
			setTexture(image);
		}

		needToDraw = false;

		glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
		//glBlendEquation(GL_MAX);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
		glViewport(0, 0, image->getWidth(), image->getHeight());
		util::setResolution(image->getWidth(), image->getHeight());

		glClearColor(base.r, base.g, base.b, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		util::setCameraData(getModelMatrix());

		for (Source* s : sources)
			s->draw();

		util::setCameraData(Camera::getInstance()->getModelMatrix());

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, Camera::getInstance()->getMSAAFbo());
		glViewport(0, 0, simpleGL::getWindowWidth(), simpleGL::getWindowHeight());
		util::setDefaultResolution();

		//glBlendEquation(GL_FUNC_ADD);
	}

	glBlendFunc(GL_ZERO, GL_SRC_COLOR);

	Sprite::draw();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

}
