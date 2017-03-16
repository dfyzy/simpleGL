#ifndef SIMPLE_CAMERA_H
#define SIMPLE_CAMERA_H

#include "unsortedSprite.h"

namespace simpleGL {

class Camera : public UnsortedSprite {
private:
	static Camera* instance;

	void bindVertices();
	void bindTexture();

	Camera(Texture texture) : UnsortedSprite(nullptr, texture, {}, {1}, 0, {1}) {}

public:
	static void init(Texture texture);

	static Camera* getInstance();

};

}

#endif
