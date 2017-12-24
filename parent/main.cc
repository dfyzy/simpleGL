#include <simpleGL/simpleGL.h>
#include <simpleGL/sprite.h>
#include <simpleGL/cursor.h>
#include <iostream>

using namespace simpleGL;

class CustomButton : public Button {
private:
	Sprite* sprite;

public:
	CustomButton(Sprite* sprite) : Button(sprite), sprite(sprite) {}

	void onEnter() { sprite->setColor(sprite->getColor() + Color(0, 0, 0.4f)); }
	void onExit() { sprite->setColor(sprite->getColor() - Color(0, 0, 0.4f)); }

	void onClick(int button) { setOpaque(!isOpaque()); }

	void onDragStart(int button) {
		std::cout << "+++++++" << std::endl;
	}

	void onDrag(int button) {
		std::cout << "." << std::endl;
	}

	void onDragEnd(int button) {
		std::cout << "-------" << std::endl;
	}

};

GLFWwindow* window;

Sprite* parent;

Angle speed(0.02f);

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

		pos *= 5;

		parent->addPosition(pos);

		int i = 0;

		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) i += 1;
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) i -= 1;

		parent->addRotation(speed*i);

		Vector scl;
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) scl.x += 1;
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) scl.x -= 1;
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) scl.y += 1;
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) scl.y -= 1;

		scl *= 0.1f;

		parent->setScale(parent->getScale() + scl);
	}
};

int main() {
	window = loadWindow("Title", 700, 700, false, true, Color(0.1f));

	glfwSetKeyCallback(window, keyCallback);

	parent = Sprite::Loader().texture({200}).color({0.6f}).pivot({-1.0f, 1.0f}).load();
	new CustomButton(parent);
	new CustomButton(Sprite::Loader().texture({{75, 50}}).parent(parent).position({50, -50}).color({1, 0, 0}).z(-10).load());
	new CustomButton(Sprite::Loader().texture({50}).parent(parent).position({-100, 50}).rotation(PI/4).color({0, 1, 0}).load());

	MainTick tick;

	draw();
}
