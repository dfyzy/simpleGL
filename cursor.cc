#include <set>

#include "cursor.h"

#include "simpleGL.h"
#include "util.h"

namespace {

std::list<simpleGL::Button*> buttons;//TODO: callback on shape movement

simpleGL::Button* clicked[simpleGL::Cursor::BUTTONS_MAX];
simpleGL::Button* hovered = nullptr;

}

namespace simpleGL {

Cursor* Cursor::instance = nullptr;

Vector glfwToSimple(double xpos, double ypos) { return Vector(xpos - getWindowWidth()/2, getWindowHeight()/2 - ypos); }

Button* getTopHover() {
	Button* top = nullptr;
	for (Button* button : buttons)
		if (button->getShape()->inBounds(Cursor::getInstance()->getRealPosition()) &&
			(top == nullptr || button->getZ() < top->getZ()))
				top = button;

	return top;
}

void moveButton(Button* top) {
	if (top != hovered) {
		if (top)			top->onEnter();
		if (hovered)	hovered->onExit();

		hovered = top;
	}
}

Cursor* Cursor::getInstance() {
	if (instance == nullptr) {
		util::print("Cursor:load");

		instance = new Cursor();
		util::addUpdate(update);
	}

	return instance;
}

void Cursor::update() {
	if (instance->hasChanged()) {
		for (int i = 0; i < Cursor::BUTTONS_MAX; i++)
			if (clicked[i])	clicked[i]->onDrag(i);

		if (instance->posCallback) instance->posCallback(instance);
	} else
		for (int i = 0; i < Cursor::BUTTONS_MAX; i++)
			if (clicked[i] && clicked[i]->getShape()->getPoint()->hasChanged())	clicked[i]->onDrag(i);

	moveButton(getTopHover());
}

void Cursor::positionCallback(GLFWwindow* window, double xpos, double ypos) {
	instance->setPosition(glfwToSimple(xpos, ypos));
}

void Cursor::buttonCallback(GLFWwindow* window, int mButton, int action, int mods) {
	bool pressed = action == GLFW_PRESS;
	instance->mouseButtons[mButton] = pressed;

	double xpos, ypos;
	glfwGetCursorPos(util::getWindow(), &xpos, &ypos);
	instance->setPosition(glfwToSimple(xpos, ypos));

	Button* top = getTopHover();
	if (top) {
		if (pressed) {
			top->onPress(mButton);
			clicked[mButton] = top;
		} else {
			top->onRelease(mButton);

			if (clicked[mButton] == top)
				top->onClick(mButton);
		}
	}

	if (!pressed) {
		if (clicked[mButton])	clicked[mButton]->onDragEnd(mButton);//TOTHINK: ondragend after ondrag
		clicked[mButton] = nullptr;
	}

	if (instance->buttCallback)	instance->buttCallback(instance, mButton, pressed);
}

Cursor::Cursor() : UnsortedSprite(Camera::getInstance(), {}, {C}, {}, {1}, 0, {1}) {
	glfwSetCursorPosCallback(util::getWindow(), positionCallback);
	glfwSetMouseButtonCallback(util::getWindow(), buttonCallback);
}

bool Cursor::getMouseButton(int button) const {
	if (button >= BUTTONS_MAX) {
		util::print("error:Cursor:getMouseButton:not a valid parameter");
		return false;
	}

	return mouseButtons[button];
}

Button::Button(Shape* shape) : shape(shape) {
	util::print("Button:load");

	Cursor::getInstance();
	buttons.push_back(this);
}

Button::~Button() {
	util::print("Button:unload");

	buttons.remove(this);
}

}
