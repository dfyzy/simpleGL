#include <simpleGL/simpleGL.h>

using namespace simpleGL;

GLFWwindow* window;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GL_TRUE);
}

int main() {
	window = loadFullscreenWindow("Title", false, Color(0.5f));

	glfwSetKeyCallback(window, keyCallback);

	Image* circle = (new Image(GL_NEAREST))->loadData("circle/circle.png");
	Texture texCircle(circle);
	Sprite::Loader().texture(texCircle).position({-30, 0}).load();
	Sprite::Loader().texture(texCircle).position({30.5, 0.5}).load();

	draw();
}
