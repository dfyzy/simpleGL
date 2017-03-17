#ifndef SIMPLE_CURSOR_H
#define SIMPLE_CURSOR_H

#include <functional>

#include "sprite.h"

namespace simpleGL {

class Button {
public:

	virtual void onPress(int mouseButton) {}
	virtual void onRelease(int mouseButton) {}
	virtual void onClick(int mouseButton) {}

	virtual void onDrag(int mouseButton) {}

	virtual void onEnter() {}
	virtual void onExit() {}

};

class Cursor : public UnsortedSprite {
public:
	static constexpr int BUTTONS_MAX = 8;

private:
	static Cursor* instance;

	static void update();

	static void positionCallback(GLFWwindow* window, double xpos, double ypos);
	static void buttonCallback(GLFWwindow* window, int button, int action, int mods);

	bool mouseButtons[BUTTONS_MAX];

	std::function<void(Cursor*)> posCallback;
	std::function<void(Cursor*, int)> buttCallback;

	//TODO: custom cursor images
	void bindVertices() {}
	void bindTexture() {}

	Cursor();

public:
	static Cursor* getInstance();

	//0 = left; 1 = right; 2 = middle;
	bool getMouseButton(int button) const;

	void setPositionCallback(std::function<void(Cursor*)> func) { posCallback = func; }
	void setMouseButtonCallback(std::function<void(Cursor*, int)> func) { buttCallback = func; }

	void addButton(Sprite* sprite, Button* button);
	void removeButton(Sprite* sprite);

};

}

#endif
