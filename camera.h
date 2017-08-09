/* A singleton class which does 2 things:
 * 1.	It's the view matrix
 * 2.	It's the framebuffer on which all the draw calls are made
 * 	After that, it draws the resulting image object on the screen, using assigned fragment shader
*/

#ifndef SIMPLE_CAMERA_H
#define SIMPLE_CAMERA_H

#include "unsortedSprite.h"
#include "framebuffer.h"

namespace simpleGL {

class Camera : public Point {
private:
	static Camera* instance;

	Framebuffer* framebuffer;
	DrawObject* drawObject;

	const GLuint vertex;
	GLuint fragment;

	Camera();
	~Camera() {
		framebuffer->unload();
		drawObject->unload();
	}

public:
	static Camera* getInstance();

	Framebuffer* getFramebuffer() const { return framebuffer; }

	void setFragmentShader(GLuint sh) { fragment = sh; }

	void draw();

};

}

#endif
