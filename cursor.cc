#include <map>

#include "cursor.h"

#include "simpleGL.h"
#include "util.h"

namespace {

std::map<simpleGL::Sprite*, simpleGL::Button*, simpleGL::Sprite::Comparer> buttons;//TODO: callback on sprite movement

std::pair<simpleGL::Sprite*, simpleGL::Button*> clicked[simpleGL::Cursor::BUTTONS_MAX];
simpleGL::Button* hovered = nullptr;

}

namespace simpleGL {

Vector glfwToSimple(double xpos, double ypos) { return Vector(xpos - getWindowWidth()/2, getWindowHeight()/2 - ypos); }

Cursor* Cursor::instance = nullptr;

Cursor* Cursor::getInstance() {
	if (instance == nullptr) {
		instance = new Cursor();
		util::addUpdate(update);
	}

	return instance;
}

bool moveButton(Sprite* sprite, Button* button) {
	bool wasOn = button == hovered;
	bool result = sprite->inBounds(Cursor::getInstance()->getRealPosition());

	if (result) {
		if (!wasOn)	{
			button->onEnter();
			if (hovered)	hovered->onExit();
			hovered = button;
		}
	} else {
		if (wasOn) {
			button->onExit();
			hovered = nullptr;
		}
	}

	return result;
}

void Cursor::update() {
	if (instance->isBindingVertices()) {
		for (int i = 0; i < Cursor::BUTTONS_MAX; i++)
			if (clicked[i].first)	clicked[i].second->onDrag(i);

		for (auto it = buttons.rbegin(); it != buttons.rend(); it++)
			if (moveButton(it->first, it->second))	break;

		if (instance->posCallback) instance->posCallback(instance);
	} else {
		for (int i = 0; i < Cursor::BUTTONS_MAX; i++)
			if (clicked[i].first && clicked[i].first->isBindingVertices())	clicked[i].second->onDrag(i);

		for (auto it = buttons.rbegin(); it != buttons.rend(); it++)
			if (it->first->isBindingVertices()) {
				if (moveButton(it->first, it->second))	break;
			}
	}
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

	Vector pos = instance->getRealPosition();

	for (auto it = buttons.rbegin(); it != buttons.rend(); it++) {
		if (it->first->inBounds(pos)) {
			//TODO:alpha stuff

			if (pressed) {
				it->second->onPress(mButton);
				clicked[mButton] = *it;
			} else {
				it->second->onRelease(mButton);

				if (clicked[mButton].first == it->first)
					it->second->onClick(mButton);
			}

			break;
		}
	}

	if (!pressed) {
		if (clicked[mButton].second)	clicked[mButton].second->onDragEnd(mButton);//TOTHINK: ondragend after ondrag
		clicked[mButton] = std::pair<Sprite*, Button*>();
	}

	if (instance->buttCallback)	instance->buttCallback(instance, mButton, pressed);
}

Cursor::Cursor() : UnsortedSprite(Camera::getInstance(), {}, {}, {1}, 0, {1}) {
	glfwSetCursorPosCallback(util::getWindow(), positionCallback);
	glfwSetMouseButtonCallback(util::getWindow(), buttonCallback);
}

bool Cursor::getMouseButton(int button) const {
	if (button >= BUTTONS_MAX) {
		util::print("Not a valid button");
		return false;
	}

	return mouseButtons[button];
}

void Cursor::addButton(Sprite* sprite, Button* button) {
	buttons.insert({sprite, button});
}

void Cursor::removeButton(Sprite* sprite) {
	buttons.erase(sprite);
}

}
