#include "camera.h"
#include "simpleGL.h"
#include "util.h"
#include "shaderData.h"

namespace simpleGL {

Camera* Camera::instance = nullptr;

Camera* Camera::getInstance() {
	if (instance == nullptr) {
		util::println("Camera:load");

		instance = new Camera(new Framebuffer(getWindowWidth(), getWindowHeight(), GL_RGB, true, GL_NEAREST, getBackground()));
		instance->setVertexShader(loadShaderSource(shaderData::getOverlayVertex(), GL_VERTEX_SHADER));
		instance->setFragmentShader(loadShaderSource(shaderData::getOverlayFragment(), GL_FRAGMENT_SHADER));
	}

	return instance;
}

void Camera::draw() {
	framebuffer->bind(getModelMatrix());

	util::drawSprites();

	framebuffer->unbind();

	glDisable(GL_BLEND);
	UnsortedSprite::draw();
	glEnable(GL_BLEND);
}

}
