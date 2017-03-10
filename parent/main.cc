#include <simpleGL/simpleGL.h>

using namespace simpleGL;

constexpr double PI = 3.14159265358979323846;

GLFWwindow* window;

Sprite* parent;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GL_TRUE);
}

void update() {
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		parent->setRotation(parent->getRotation() + 0.02f);
	}
}

int main() {
	window = createFullscreenWindow("Title", true, Color(0.1f));

	glfwSetKeyCallback(window, keyCallback);

	parent = Sprite::Loader({50}).rotation(PI/4).load();
	Sprite* child = Sprite::Loader({50}).parent(parent).position({0, 50}).rotation(PI/4).load();
	Sprite* child2 = Sprite::Loader({50}).parent(child).position({0, 50}).rotation(PI/4).load();
	Sprite* child3 = Sprite::Loader({50}).parent(child2).position({0, 50}).rotation(PI/4).load();

	setUpdate(update);

	draw();
}
