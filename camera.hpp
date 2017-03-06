#ifndef SIMPLE_CAMERA_H
#define SIMPLE_CAMERA_H

#include "unsortedSprite.hpp"

namespace simpleGL {

class Camera : public UnsortedSprite {
private:
	static Camera* instance;

	void bindVertexData();
	void bindTextureData();

	Camera(Texture texture) : UnsortedSprite(texture, {}, {1}, 0, {1}) {
		bindVertexData();
		bindTextureData();
	}

public:
	static void init(Texture texture);

	static Camera* getInstance();

};

}

#endif
