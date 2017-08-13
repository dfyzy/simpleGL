/* Cursor sprite(without actual drawing for now)
 * Also, callbacks for mouse buttons
*/

#ifndef SIMPLE_CURSOR_H
#define SIMPLE_CURSOR_H

#include <functional>

#include "simpleGL.h"

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

	std::function<void()> posCallback;
	std::function<void(int, bool)> buttCallback;

	Point::Change* change;

	//TODO: custom cursor images

	Cursor() : UnsortedSprite(Camera::getInstance(), {}, {1.0f}, {}, {}, Center, {1}), change(getChange()) {
		glfwSetCursorPosCallback(getWindow(), positionCallback);
		glfwSetMouseButtonCallback(getWindow(), buttonCallback);
	}

	~Cursor() {}

public:
	static Cursor* getInstance();

	bool changed() const { return change->get(); }

	//0 = left; 1 = right; 2 = middle;
	bool getMouseButton(int button) const;

	void setPositionCallback(std::function<void()> func) { posCallback = func; }
	void setMouseButtonCallback(std::function<void(int, bool)> func) { buttCallback = func; }//int button, bool pressed

};

class Button : public Point::Component {
private:
	Shape* shape;
	Point::Change* change;

	bool opaque {true};

	int z;

	bool lastOn {false};
	bool on {false};
	bool entered {false};

	float dragBound {8};

protected:
	~Button();

public:
	Button(Shape* shape, int z);
	Button(Sprite* sprite) : Button(sprite->getBoxShape(), sprite->getZ()) {}

	Shape* getShape() const { return shape; }

	bool changed() const { return change->get(); }

	bool isOpaque() const { return opaque; }
	void setOpaque(bool b) { opaque = b; }

	int getZ() const { return z; }
	void setZ(int i);

	float getDragBound() const { return dragBound; }
	void setDragBound(float f) { dragBound = f; }

	bool isEntered() {
		if (Cursor::getInstance()->changed() || changed())
			entered = shape->inBounds(Cursor::getInstance()->getRealPosition());

		change->reset();

		return entered;
	}

	bool isOn() const { return on; }
	void setOn(bool notBlocked) { on = isEntered() && notBlocked; }

	void callback() {
		if (lastOn != on) {
			if (on)	onEnter();
			else		onExit();

			lastOn = on;
		}
	}

	virtual void onPress(int mouseButton) {}
	virtual void onRelease(int mouseButton) {}
	virtual void onClick(int mouseButton) {}

	virtual void onDragStart(int mouseButton) {}
	virtual void onDrag(int mouseButton) {}
	virtual void onDragEnd(int mouseButton) {}

	virtual void onEnter() {}
	virtual void onExit() {}

	struct Comparer {
		bool operator()(const Button* lhs, const Button* rhs) {
			if (lhs->z != rhs->z)
				return lhs->z < rhs->z;

			return lhs < rhs;
		}
	};
};

}

#endif
