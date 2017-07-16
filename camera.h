#ifndef SIMPLE_CAMERA_H
#define SIMPLE_CAMERA_H

#include "unsortedSprite.h"
#include "framebuffer.h"

namespace simpleGL {

class Camera : public UnsortedSprite {
private:
	static Camera* instance;

	Framebuffer* framebuffer;

	void bindVertices() {}
	void bindTexture() {
		getDrawObject()->bindVertexData(Matrix::scale(getTexture().getBounds()));

		UnsortedSprite::bindTexture();
	}

	Camera(Framebuffer* framebuffer) : UnsortedSprite(nullptr, framebuffer->getImage(), {Center}, {}, {1}, 0, {1}), framebuffer(framebuffer) {}
	~Camera() {
		framebuffer->unload();
	}

public:
	static Camera* getInstance();

	Framebuffer* getFramebuffer() const { return framebuffer; }

	void draw();

};

}

#endif
