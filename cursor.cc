#include <set>
#include <vector>

#include "cursor.h"

#include "simpleGL.h"
#include "util.h"

namespace {

std::set<simpleGL::Button*, simpleGL::Button::Comparer> buttons;

struct Press {
	simpleGL::Button* button;
	bool dragging;
	simpleGL::Vector position;
};

std::list<Press> presses[simpleGL::Cursor::BUTTONS_MAX];

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

	std::list<std::pair<Press*, int>> drag;

	for (int i = 0; i < Cursor::BUTTONS_MAX; i++)
		for (Press& cl : presses[i])
			if (instance->changed() || cl.button->changed())
				drag.push_back({&cl, i});

	for (std::pair<Press*, int> dr : drag) {
		if (!dr.first->dragging) {
			if (((dr.first->button->getShape()->getModelMatrix().inv()*instance->getRealPosition())
					- dr.first->position).length() < dr.first->button->getDragBound())
						continue;

			dr.first->button->onDragStart(dr.second);
		}

		dr.first->button->onDrag(dr.second);
		dr.first->dragging = true;
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
			presses[mButton].push_back({b, false, b->getShape()->getModelMatrix().inv()*instance->getRealPosition()});
		} else {
			b->onRelease(mButton);

			for (Press& cl : presses[mButton])
				if (b == cl.button) {
					if (!cl.dragging)	b->onClick(mButton);
					break;
				}
		}

	if (!pressed) {
		for (Press& cl : presses[mButton])
			if (cl.dragging)	cl.button->onDragEnd(mButton);

		presses[mButton].clear();
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

Button::Button(Shape* shape, int z) : Point::Component(shape), shape(shape), change(shape->getChange()), z(z) {
	util::print("Button:load");

	Cursor::getInstance();
	buttons.insert(this);
}

Button::~Button() {
	util::print("Button:unload");

	buttons.erase(this);
	for (int i = 0; i < simpleGL::Cursor::BUTTONS_MAX; i++)
		for (auto it = presses[i].begin(); it != presses[i].end(); it++)
			if (it->button == this) {
				presses[i].erase(it);
				break;
			}
}

void Button::setZ(int i) {
	buttons.erase(this);

	z = i;

	buttons.insert(this);
}

}
