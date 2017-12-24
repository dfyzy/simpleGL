#include <set>

#include "cursor.h"
#include "camera.h"
#include "sprite.h"
#include "window.h"
#include "util.h"

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

Cursor* Cursor::instance = nullptr;

Cursor* Cursor::getInstance() {
	if (instance == nullptr) {
		util::println("Cursor:load");

		instance = new Cursor();
	}

	return instance;
}

Cursor::Cursor() : UnsortedSprite(Camera::getInstance(), {}, {1.0f}, {}, {}, {0.0f}, {1}) {
	Window* current = Window::getCurrent();

	glfwSetCursorPosCallback(current->getWindow(), positionCallback);
	glfwSetMouseButtonCallback(current->getWindow(), buttonCallback);
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
	instance->setPosition(glfwToSimple(xpos, ypos));
}

void Cursor::buttonCallback(GLFWwindow* window, int mButton, int action, int mods) {
	bool pressed = action == GLFW_PRESS;
	instance->mouseButtons[mButton] = pressed;

	double xpos, ypos;
	glfwGetCursorPos(Window::getCurrent()->getWindow(), &xpos, &ypos);
	instance->setPosition(glfwToSimple(xpos, ypos));

	std::list<Button*> on;

	bool notBlocked {true};
	for (Button* b : buttons)
		if (b->isEntered() && notBlocked) {
			on.push_back(b);

			if (b->isOpaque())	notBlocked = false;
		}

	instance->changed.reset();

	for (Button* b : on)
		if (pressed) {
			b->onPress(mButton);
			presses[mButton].push_back({b, false, b->getOwner()->getModelMatrix().inv()*instance->getRealPosition()});
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

	if (instance->buttCallback)	instance->buttCallback(mButton, pressed);
}

bool Cursor::getMouseButton(int button) const {
	if (button >= BUTTONS_MAX) {
		util::println("error:Cursor:getMouseButton:not a valid parameter");
		return false;
	}

	return mouseButtons[button];
}

Button::Button(Rectangle* rect, int z) : Component<Rectangle>(rect), z(z) {
	util::println("Button:load");

	Cursor::getInstance();
	buttons.insert(this);
}

Button::Button(Sprite* sprite) : Button(sprite, sprite->getZ()) {}

Button::~Button() {
	util::println("Button:unload");

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
	if (Cursor::getInstance()->hasChanged() || hasChanged())
		entered = getOwner()->inBounds(Cursor::getInstance()->getRealPosition());

	ownerChanged.reset();

	return entered;
}

}
