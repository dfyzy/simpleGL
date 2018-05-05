#include <simpleGL/simpleGL.h>
#include <simpleGL/window.h>
#include <simpleGL/sprite.h>
#include <simpleGL/cursor.h>
#include <simpleGL/physicalShape.h>
#include <iostream>

using namespace simpleGL;

GLFWwindow* window;

PhysicalShape* shape;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GL_TRUE);
}

class MainTick : public Updatable<EUpdateType::Tick> {
	void update() {
		Vector pos;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) pos.x += 1;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) pos.x -= 1;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) pos.y += 1;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) pos.y -= 1;

		pos *= 2.0f;

		shape->setPositionSpeed(shape->getPositionSpeed() + pos);

		float f = 0;

		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) f -= 1;
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) f += 1;

		f *= 0.05f;

		shape->setRotationSpeed(shape->getRotationSpeed() + f);
	}
};

int main() {
	Window windowObj("Title", 1000, 700, false, true);
	window = windowObj.getWindow();
	getInstance<Camera>()->setBaseColor(Color(0.1f));

	glfwSetKeyCallback(window, keyCallback);

	shape = new PhysicalShape({}, {}, 1.0f, {100.0f});
	shape->addChild(SortedSprite::Loader().texture({100}).color({1.0f, 0.0f, 0.0f}).load());

	PhysicalShape* otherShape = new PhysicalShape({300.0f}, {}, 1.0f, {100.0f});
	otherShape->addChild(SortedSprite::Loader().texture({100}).color({0.0f, 0.0f, 1.0f}).load());

	MainTick tick;

	windowObj.draw();
}
