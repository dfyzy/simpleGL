#ifndef SIMPLE_CAMERA_H
#define SIMPLE_CAMERA_H

#include "glfw.h"
#include "point.h"
#include "sortedFrame.h"

namespace simpleGL {

class Camera : public Point {
private:
	static Camera* instance;

	SortedFrame frame;
	DrawObject drawObject;

	const GLuint vertex;
	GLuint fragment;

	Camera();

public:
	static Camera* getInstance();

	SortedFrame* getFrame() { return &frame; }

	Color getBaseColor() const {
		return frame.getBaseColor();
	}
	void setBaseColor(Color c) {
		frame.setBaseColor(c);
	}

	void setFragmentShader(GLuint sh) { fragment = sh; }

	void resize(unsigned width, unsigned height) {
		frame.resize(width, height);
	}

	void draw();

};

}

#endif
