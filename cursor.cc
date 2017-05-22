#include <set>
#include <vector>

#include "cursor.h"

#include "simpleGL.h"
#include "util.h"

namespace {

std::set<simpleGL::Button*, simpleGL::Button::Comparer> buttons;

std::list<std::pair<simpleGL::Button*, bool>> clicked[simpleGL::Cursor::BUTTONS_MAX];

}

namespace simpleGL {

Cursor* Cursor::instance = nullptr;

Vector glfwToSimple(double xpos, double ypos) { return Vector(xpos - getWindowWidth()/2, getWindowHeight()/2 - ypos); }

Cursor* Cursor::getInstance() {
	if (instance == nullptr) {
		util::print("Cursor:load");

		instance = new Cursor();
		util::addPreUpdate(updatePosition);
	}

	return instance;
}

void Cursor::updatePosition() {
	if (instance->changed() && instance->posCallback) instance->posCallback(instance);

	std::list<std::pair<std::pair<Button*, bool>*, int>> drag;

	for (int i = 0; i < Cursor::BUTTONS_MAX; i++)
		for (std::pair<Button*, bool>& cl : clicked[i])
			if (instance->changed() || cl.first->changed())
				drag.push_back({&cl, i});

	for (std::pair<std::pair<Button*, bool>*, int> dr : drag) {
		if (!dr.first->second)
			dr.first->first->onDragStart(dr.second);

		dr.first->first->onDrag(dr.second);
		dr.first->second = true;
	}

	bool notBlocked {true};
	for (Button* b : buttons) {
		b->setOn(notBlocked);
		if (b->isOn() && b->isOpaque())	notBlocked = false;
	}

	instance->change.reset();

	for (Button* b : buttons)
		b->callback();
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

	std::list<Button*> on;

	bool notBlocked {true};
	for (Button* b : buttons)
		if (b->isEntered() && notBlocked) {
			on.push_back(b);

			if (b->isOpaque())	notBlocked = false;
		}

	instance->change.reset();

	for (Button* b : on)
		if (pressed) {
			b->onPress(mButton);
			clicked[mButton].push_back({b, false});
		} else {
			b->onRelease(mButton);

			for (std::pair<Button*, bool>& cl : clicked[mButton])
				if (b == cl.first) {
					if (!cl.second)	b->onClick(mButton);
					break;
				}
		}

	if (!pressed) {
		for (std::pair<Button*, bool>& cl : clicked[mButton])
			if (cl.second)	cl.first->onDragEnd(mButton);

		clicked[mButton].clear();
	}

	if (instance->buttCallback)	instance->buttCallback(instance, mButton, pressed);
}

Cursor::Cursor() : UnsortedSprite(Camera::getInstance(), {}, {C}, {}, {1}, 0, {1}), change(getChange()) {
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

Button::Button(Shape* shape, int z) : shape(shape), change(shape->getChange()), z(z) {
	util::print("Button:load");

	Cursor::getInstance();
	buttons.insert(this);
}

Button::~Button() {
	util::print("Button:unload");

	buttons.erase(this);
	for (int i = 0; i < simpleGL::Cursor::BUTTONS_MAX; i++)
		for (auto it = clicked[i].begin(); it != clicked[i].end(); it++)
			if (it->first == this) {
				clicked[i].erase(it);
				break;
			}
}

void Button::setZ(int i) {
	buttons.erase(this);

	z = i;

	buttons.insert(this);
}

}
