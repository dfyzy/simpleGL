#include "camera.h"
#include "shader.h"
#include "shaderData.h"
#include "log.h"

namespace simpleGL {

Camera::Camera(unsigned width, unsigned height)
	: Point(),
		frame(
			width,
			height,
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
