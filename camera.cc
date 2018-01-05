#include "camera.h"
#include "framebuffer.h"
#include "sprite.h"
#include "image.h"
#include "drawObject.h"
#include "shader.h"
#include "shaderData.h"
#include "window.h"
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
	: Point(), frame(	Window::getCurrent()->getWidth(),
									Window::getCurrent()->getHeight(),
									GL_RGB,
									true,
									GL_NEAREST,
									Color(0.0f)
								),
		vertex(loadShaderSource(shaderData::getOverlayVertex(), GL_VERTEX_SHADER)),
		fragment(loadShaderSource(shaderData::getOverlayFragment(), GL_FRAGMENT_SHADER)) {
	drawObject.bindVertexData(Matrix::scale(2.0f));
}

void Camera::draw() {
	frame.draw(getModelMatrix());

	useShaders(vertex, fragment);
	frame.getImage()->bind();

	glDisable(GL_BLEND);
	glDisable(GL_STENCIL_TEST);

	drawObject.draw();

	glEnable(GL_BLEND);
	glEnable(GL_STENCIL_TEST);
}

}
