#include "camera.h"
#include "util.h"

namespace simpleGL {

Camera* Camera::instance = nullptr;

Camera* Camera::getInstance() {
	return instance;
}

void Camera::init(Texture texture) {
	if (instance == nullptr)
		instance = new Camera(texture);
}

void Camera::bindVertices() {
	util::setCameraData(getRealPosition(), getScale(), getRealRotation().get());
}

void Camera::bindTexture() {
	util::bindData(getId(), vboType::VERTEX, Matrix::scale(getTexture().getBounds()));

	UnsortedSprite::bindTexture();
}

}
