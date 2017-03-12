#include "simpleGL.h"
#include "util.h"

namespace simpleGL {

Cursor* Cursor::instance = nullptr;

Cursor* Cursor::getInstance() {
	if (instance == nullptr)	instance = new Cursor();

	return instance;
}

Vector glfwToSimple(double xpos, double ypos) { return Vector(xpos - getWindowWidth()/2, getWindowHeight()/2 - ypos); }

void Cursor::positionCallback(GLFWwindow* window, double xpos, double ypos) {
	Cursor::getInstance()->position = glfwToSimple(xpos, ypos);

	util::moveButtons();
}

void Cursor::buttonCallback(GLFWwindow* window, int button, int action, int mods) {
	Cursor::getInstance()->buttons[button] = action == GLFW_PRESS;

	double xpos, ypos;
	glfwGetCursorPos(util::getWindow(), &xpos, &ypos);
	Cursor::getInstance()->position = glfwToSimple(xpos, ypos);

	util::clickButtons(button);
}

Cursor::Cursor() : UnsortedSprite(Camera::getInstance(), {}, {}, {1}, 0, {1}) {
	glfwSetCursorPosCallback(util::getWindow(), positionCallback);
	glfwSetMouseButtonCallback(util::getWindow(), buttonCallback);
}

bool Cursor::getButton(int button) const {
	if (button >= BUTTONS_MAX) {
		util::print("Not a valid button");
		return false;
	}

	return buttons[button];
}

}
