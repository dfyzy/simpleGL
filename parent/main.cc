#include <simpleGL/simpleGL.h>
#include <iostream>

using namespace simpleGL;

constexpr double PI = 3.14159265358979323846;

class CustomButton : public Button {
private:
	Sprite* sprite;

public:
	CustomButton(Sprite* sprite) : sprite(sprite) {
		Cursor::getInstance()->addButton(sprite, this);
	}

	void onEnter() { sprite->setColor(sprite->getColor() + Color(0, 0, 0.4f)); }
	void onExit() { sprite->setColor(sprite->getColor() - Color(0, 0, 0.4f)); }

};


GLFWwindow* window;

Sprite* parent;

Angle speed(0.02f);

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GL_TRUE);
}

void update() {
	Vector pos;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) pos.x += 1;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) pos.x -= 1;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) pos.y += 1;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) pos.y -= 1;

	pos *= 5;

	parent->setPosition(parent->getPosition() + pos);

	int i = 0;

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) i += 1;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) i -= 1;

	parent->setRotation(parent->getRotation() + speed*i);

	Vector scl;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) scl.x += 1;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) scl.x -= 1;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) scl.y += 1;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) scl.y -= 1;

	scl *= 0.1f;

	parent->setScale(parent->getScale() + scl);
}

int main() {
	window = createFullscreenWindow("Title", true, Color(0.1f));

	glfwSetKeyCallback(window, keyCallback);

	parent = Sprite::Loader({200}).color({0.6f}).load();
	new CustomButton(parent);
	new CustomButton(Sprite::Loader(Texture({75, 50})).parent(parent).position({50, 50}).color({1, 0, 0}).load());
	new CustomButton(Sprite::Loader({50}).parent(parent).position({-100, 50}).rotation(PI/4).color({0, 1, 0}).load());

	setUpdate(update);

	draw();
}
