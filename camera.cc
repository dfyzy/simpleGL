#include "simpleGL.h"
#include "shaderData.h"
#include "util.h"

namespace simpleGL {

Camera* Camera::instance = nullptr;

Camera* Camera::getInstance() {
	if (instance == nullptr) {
		util::println("Camera:load");

		instance = new Camera();
	}

	return instance;
}

Camera::Camera() : Point(),
			framebuffer(new Framebuffer(getWindowWidth(), getWindowHeight(), GL_RGB, true, GL_NEAREST, getBackground())),
			drawObject(new DrawObject()),
			vertex(loadShaderSource(shaderData::getOverlayVertex(), GL_VERTEX_SHADER)),
			fragment(loadShaderSource(shaderData::getOverlayFragment(), GL_FRAGMENT_SHADER)) {

				drawObject->bindVertexData(Matrix::scale(2.0f));
			}

void Camera::draw() {
	framebuffer->bind(getModelMatrix());

	util::drawSprites();

	framebuffer->unbind();

	useShaders(vertex, fragment);
	framebuffer->getImage()->bind();

	glDisable(GL_BLEND);
	drawObject->draw();
	glEnable(GL_BLEND);
}

}
