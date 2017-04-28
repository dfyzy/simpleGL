#ifndef SIMPLE_CAMERA_H
#define SIMPLE_CAMERA_H

#include "unsortedSprite.h"

namespace simpleGL {

class Camera : public UnsortedSprite {
private:
	static Camera* instance;
	static GLuint msaaFbo;
	static GLuint rectFbo;

	void bindVertices();
	void bindTexture();

	Camera(Texture texture) : UnsortedSprite(nullptr, texture, {}, {1}, 0, {1}) {}

protected:
	~Camera() {}

public:
	static Camera* getInstance();

	static GLuint getMSAAFbo();
	static GLuint getRectFbo();

};

}

#endif
