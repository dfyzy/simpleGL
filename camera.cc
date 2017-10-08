#include "camera.h"
#include "framebuffer.h"
#include "sprite.h"
#include "image.h"
#include "drawObject.h"
#include "shader.h"
#include "shaderData.h"
#include "simpleGL.h"
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

Camera::Camera()
	: Point(), framebuffer(new Framebuffer(getWindowWidth(), getWindowHeight(), GL_RGB, true, GL_NEAREST, getBackground())),
		drawObject(new DrawObject()),
		vertex(loadShaderSource(shaderData::getOverlayVertex(), GL_VERTEX_SHADER)),
		fragment(loadShaderSource(shaderData::getOverlayFragment(), GL_FRAGMENT_SHADER)) {
	drawObject->bindVertexData(Matrix::scale(2.0f));
}

Camera::~Camera() {
	framebuffer->unload();
	drawObject->unload();
}

void Camera::draw() {
	framebuffer->bind(getModelMatrix());

	Sprite::drawAll();

	framebuffer->unbind();

	useShaders(vertex, fragment);
	framebuffer->getImage()->bind();

	glDisable(GL_BLEND);
	glDisable(GL_STENCIL_TEST);

	drawObject->draw();

	glEnable(GL_BLEND);
	glEnable(GL_STENCIL_TEST);
}

}
