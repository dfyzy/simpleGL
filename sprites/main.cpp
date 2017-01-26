#include <simpleGL/simpleGL.hpp>
#include <random>
#include <functional>
#include <ctime>

using namespace simpleGL;

std::mt19937 randEngine(time(0));
float rotation = 0;
GLFWwindow* window;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GL_TRUE);

}

void update() {
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		rotation += 0.05f;
		setCameraRotation(rotation);
	}
}

int main() {
	window = createFullscreenWindow("Title", true, Color(1));

	glfwSetKeyCallback(window, keyCallback);

	setTextureFiltering(GL_LINEAR);

	Texture st("sprites/eye.png");

	std::uniform_int_distribution<int> positionDist(-500, 500);
	auto randomPosition = std::bind(positionDist, randEngine);

	for (int i = 0; i < 10000; i++)
		Sprite::Loader(st).position({(float)randomPosition(), (float)randomPosition()}).color({1, 0, 0, 1}).load();

	setUpdate(update);

	draw();
}
