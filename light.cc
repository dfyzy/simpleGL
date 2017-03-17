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

Light::Source::Source(Light* light, UnsortedSprite* parent, Vector position, Vector bounds, Angle rotation, Color color)
			: UnsortedSprite(parent, Texture(bounds), position, {1}, rotation, color), light(light) {
	setFragmentShader(getDefaultFragment());

	if (light) {
		light->sources.push_back(this);
		light->toggleDraw();
	}
}

void Light::Source::draw() {
	Vector pos = getRealPosition();
	glProgramUniform2f(getFragmentShader(), centreLoc, pos.x + getWindowWidth()*0.5f, pos.y + getWindowHeight()*0.5f);
	glProgramUniform2f(getFragmentShader(), boundsLoc, getTexture().getBounds().x, getTexture().getBounds().y);

	UnsortedSprite::draw();
}

Light::Light(UnsortedSprite* parent, Vector position, int z, unsigned width, unsigned height, Color base)
				: Image(width, height, GL_RGB, GL_LINEAR), Sprite(parent, Texture(this), position, z, {1}, {}, {1}), base(base) {
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, Image::id, 0);
}

Light::~Light() {
	glDeleteFramebuffers(1, &fbo);

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
		util::setResolution(width, height);

		glClearColor(base.r, base.g, base.b, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		Camera* cam = Camera::getInstance();
		Vector camPos = cam->getPosition();
		Angle camRot = cam->getRotation();

		//TODO: setREALPostion
		cam->setPosition(getRealPosition());
		cam->setRotation(getRealRotation());

		cam->bindData();

		for (Source* s : sources)
			s->draw();

		cam->setPosition(camPos);
		cam->setRotation(camRot);

		cam->bindData();

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
