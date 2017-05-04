#include <simpleGL/simpleGL.h>

using namespace simpleGL;

GLFWwindow* window;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GL_TRUE);
}

int main() {
	window = createFullscreenWindow("Title", false, Color(0.5f));

	glfwSetKeyCallback(window, keyCallback);

	Image* circle = new Image("circle/circle.png", GL_NEAREST);
	Texture texCircle(circle);
	new Sprite(Sprite::Data(texCircle).position({-30, 0}));
	new Sprite(Sprite::Data(texCircle).position({30.5, 0.5}));

	draw();
}
