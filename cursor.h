#ifndef SIMPLE_CURSOR_H
#define SIMPLE_CURSOR_H

#include <functional>
#include <memory>

#include "sprite.h"

namespace simpleGL {

class Cursor : public UnsortedSprite {
public:
	static constexpr int BUTTONS_MAX = 8;

private:
	static Cursor* instance;

	static void updatePosition();

	static void positionCallback(GLFWwindow* window, double xpos, double ypos);
	static void buttonCallback(GLFWwindow* window, int button, int action, int mods);

	bool mouseButtons[BUTTONS_MAX];

	std::function<void(Cursor*)> posCallback;
	std::function<void(Cursor*, int, bool)> buttCallback;

	//TODO: custom cursor images
	void bindVertices() {}
	void bindTexture() {}

	Cursor();
	~Cursor() {}

public:
	static Cursor* getInstance();

	//0 = left; 1 = right; 2 = middle;
	bool getMouseButton(int button) const;

	void setPositionCallback(std::function<void(Cursor*)> func) { posCallback = func; }
	void setMouseButtonCallback(std::function<void(Cursor*, int, bool)> func) { buttCallback = func; }//int button, bool pressed

};

class Button {
private:
	Shape* shape;
	bool opaque {true};

public:
	Button(Shape* shape);
	virtual ~Button();

	Shape* getShape() const { return shape; }

	bool isOpaque() const { return opaque; }
	void setOpaque(bool b) { opaque = b; }

	virtual int getZ() const =0;

	virtual void onPress(int mouseButton) {}
	virtual void onRelease(int mouseButton) {}
	virtual void onClick(int mouseButton) {}

	virtual void onDragStart(int mouseButton) {}
	virtual void onDrag(int mouseButton) {}
	virtual void onDragEnd(int mouseButton) {}

	virtual void onEnter() {}
	virtual void onExit() {}
};

class ShapeButton : public Button {
private:
	int z;

public:
	ShapeButton(Shape* shape, int z) : Button(shape), z(z) {}

	int getZ() const { return z; }

};

class SpriteButton : public Button {
private:
	Sprite* sprite;

public:
	SpriteButton(Sprite* sprite) : Button(sprite), sprite(sprite) {}

	int getZ() const { return sprite->getZ(); }

	Sprite* getSprite() const { return sprite; }

};

}

#endif
