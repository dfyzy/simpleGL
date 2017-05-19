#include <set>
#include <vector>

#include "cursor.h"

#include "simpleGL.h"
#include "util.h"

namespace {

std::list<simpleGL::Button*> buttons;//TODO: callback on shape movement

std::list<std::pair<simpleGL::Button*, bool>> clicked[simpleGL::Cursor::BUTTONS_MAX];
std::vector<simpleGL::Button*> hovered;

}

namespace simpleGL {

Cursor* Cursor::instance = nullptr;

Vector glfwToSimple(double xpos, double ypos) { return Vector(xpos - getWindowWidth()/2, getWindowHeight()/2 - ypos); }

std::list<Button*> getTopHovers() {
	std::list<Button*> result;
	for (Button* button : buttons)
		if (button->getShape()->inBounds(Cursor::getInstance()->getRealPosition())
			 && (result.empty() || button->getZ() < (*result.begin())->getZ())) {
					if (button->isOpaque())	result.clear();

					result.push_front(button);
				}

	return result;
}

void moveButton(std::list<Button*> top) {
	bool exited[hovered.size()];
	for (unsigned i = 0; i < hovered.size(); i++)
		exited[i] = true;

	for (Button* t : top) {
		bool entered = true;
		for (unsigned i = 0; i < hovered.size(); i++)
			if (t == hovered[i]) {
				entered = false;
				exited[i] = false;
				break;
			}

		if (entered)	t->onEnter();
	}

	for (unsigned i = 0; i < hovered.size(); i++)
		if (exited[i])	hovered[i]->onExit();

	hovered.clear();
	for (Button* t : top)
		hovered.push_back(t);
}

Cursor* Cursor::getInstance() {
	if (instance == nullptr) {
		util::print("Cursor:load");

		instance = new Cursor();
		util::addUpdate(updatePosition);
	}

	return instance;
}

void Cursor::updatePosition() {
	bool instChange = instance->getChanged();

	if (instChange && instance->posCallback) instance->posCallback(instance);

	for (int i = 0; i < Cursor::BUTTONS_MAX; i++)
		for (std::pair<Button*, bool>& cl : clicked[i])
			if (instChange || cl.first->getShape()->getChanged()) {
				if (!cl.second)
					cl.first->onDragStart(i);

				cl.first->onDrag(i);
				cl.second = true;
			}

	moveButton(getTopHovers());
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

	std::list<Button*> top = getTopHovers();
	for (Button* b : top)
		if (pressed) {
			b->onPress(mButton);
			clicked[mButton].push_back({b, false});

			b->getShape()->getChanged();
		} else {
			b->onRelease(mButton);

			for (std::pair<Button*, bool>& cl : clicked[mButton])
				if (b == cl.first) {
					b->onClick(mButton);
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
