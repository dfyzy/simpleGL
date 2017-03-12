#ifndef SIMPLE_CURSOR_H
#define SIMPLE_CURSOR_H

#include "unsortedSprite.h"

namespace simpleGL {

class Cursor : public UnsortedSprite {
public:
	static constexpr int BUTTONS_MAX = 8;

private:
	static Cursor* instance;

	static void positionCallback(GLFWwindow* window, double xpos, double ypos);
	static void buttonCallback(GLFWwindow* window, int button, int action, int mods);

	bool buttons[BUTTONS_MAX];

	//TODO: custom cursor images
	void bindVertexData() {}
	void bindTextureData() {}

	Cursor();

public:
	static Cursor* getInstance();

	//0 = left; 1 = right; 2 = middle;
	bool getButton(int button) const;

};

}

#endif
