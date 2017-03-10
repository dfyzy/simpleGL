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

void Camera::bindVertexData() {
	util::setCameraData(getRealPosition(), scale, getRealRotation());

	bindChildData();
}

void Camera::bindTextureData() {
	util::bindData(id, vboType::VERTEX, {}, texture.getBounds(), 0, 1);

	UnsortedSprite::bindTextureData();
}

}
