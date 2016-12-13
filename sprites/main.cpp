#include <simpleGL/simpleGL.hpp>
#include <random>

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
		simpleGL::setCameraRotation(rotation);
	}
}

int main() {
	window = simpleGL::createFullscreenWindow("Title", true);

	glfwSetKeyCallback(window, keyCallback);
	simpleGL::setBackground(SimpleColor(1));

	simpleGL::init();

	simpleGL::setTextureFiltering(GL_LINEAR);

	SimpleTexture* st = simpleGL::loadTexture("sprites/eye.png");

	std::uniform_int_distribution<int> positionDist(-500, 500);
	auto randomPosition = std::bind(positionDist, randEngine);

	for (int i = 0; i < 10000; i++)
		simpleGL::loadSprite(st, SimpleVector(randomPosition(), randomPosition()), 0, SimpleColor(1, 0, 0, 1));

	simpleGL::setUpdate(update);

	simpleGL::draw();
}
