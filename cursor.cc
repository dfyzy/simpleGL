#include <set>

#include "cursor.h"
#include "globalInstance.h"
#include "camera.h"
#include "sortedSprite.h"
#include "window.h"
#include "log.h"

namespace {

std::set<simpleGL::Button*, simpleGL::Button::Comparer> buttons;

struct Press {
	simpleGL::Button* button;
	bool dragging;
	simpleGL::Vector position;
};

std::list<Press> presses[simpleGL::Cursor::BUTTONS_MAX];


simpleGL::Vector glfwToSimple(double xpos, double ypos) {
	simpleGL::Window* current = simpleGL::Window::getCurrent();
	return simpleGL::Vector(xpos - current->getWidth()/2.0f, current->getHeight()/2.0f - ypos);
}

}

namespace simpleGL {

Cursor::Cursor() : Sprite(GlobalInstance<Camera>::get(), {}, {1.0f}, {}, {}, {0.0f}, {1}) {
	Window* current = Window::getCurrent();

	glfwSetCursorPosCallback(current->getWindow(), positionCallback);
	glfwSetMouseButtonCallback(current->getWindow(), buttonCallback);
}

Cursor::~Cursor() {
	Window* current = Window::getCurrent();
	
	glfwSetCursorPosCallback(current->getWindow(), nullptr);
	glfwSetMouseButtonCallback(current->getWindow(), nullptr);
}

void Cursor::update() {
	if (hasChanged() && posCallback) posCallback();

	std::list<std::pair<Press*, int>> drag;

	for (int i = 0; i < Cursor::BUTTONS_MAX; i++)
		for (Press& cl : presses[i])
			if (hasChanged() || cl.button->hasChanged())
				drag.push_back({&cl, i});

	for (std::pair<Press*, int> dr : drag) {
		if (!dr.first->dragging) {
			if (((dr.first->button->getOwner()->getModelMatrix().inv()*getRealPosition())
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

	changed.reset();

	for (Button* b : buttons)
		b->callback();
}

void Cursor::positionCallback(GLFWwindow* window, double xpos, double ypos) {
	Cursor* cursor = GlobalInstance<Cursor>::get();
	if (!cursor) return;

	cursor->setPosition(glfwToSimple(xpos, ypos));
}

void Cursor::buttonCallback(GLFWwindow* window, int mButton, int action, int mods) {
	Cursor* cursor = GlobalInstance<Cursor>::get();
	if (!cursor) return;

	bool pressed = action == GLFW_PRESS;
	cursor->mouseButtons[mButton] = pressed;

	double xpos, ypos;
	glfwGetCursorPos(Window::getCurrent()->getWindow(), &xpos, &ypos);
	cursor->setPosition(glfwToSimple(xpos, ypos));

	std::list<Button*> on;

	bool notBlocked {true};
	for (Button* b : buttons)
		if (b->isEntered() && notBlocked) {
			on.push_back(b);

			if (b->isOpaque())	notBlocked = false;
		}

	cursor->changed.reset();

	for (Button* b : on)
		if (pressed) {
			b->onPress(mButton);
			presses[mButton].push_back({b, false, b->getOwner()->getModelMatrix().inv()*cursor->getRealPosition()});
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

	if (cursor->buttCallback)	cursor->buttCallback(mButton, pressed);
}

bool Cursor::getMouseButton(int button) const {
	if (button >= BUTTONS_MAX) {
		println("error:Cursor:getMouseButton:not a valid parameter");
		return false;
	}

	return mouseButtons[button];
}

Button::Button(Rectangle* rect, int z) : Component<Rectangle>(rect), z(z) {
	buttons.insert(this);
}

Button::Button(SortedSprite* sprite) : Button(sprite, sprite->getZ()) {}

Button::~Button() {
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

bool Button::isEntered() {
	Cursor* cursor = GlobalInstance<Cursor>::get();
	if (!cursor)	return false;

	if (cursor->hasChanged() || hasChanged())
		entered = getOwner()->inBounds(cursor->getRealPosition());

	ownerChanged.reset();

	return entered;
}

}
