#include <simpleGL/simpleGL.h>
#include <simpleGL/camera.h>
#include <simpleGL/sprite.h>
#include <simpleGL/image.h>
#include <simpleGL/updatable.h>
#include <random>
#include <functional>
#include <ctime>

using namespace simpleGL;

std::mt19937 randEngine(time(0));
GLFWwindow* window;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GL_TRUE);

}

class MainTick : public Updatable<EUpdateType::Tick> {
	void update() {
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			Camera::getInstance()->setRotation(Camera::getInstance()->getRotation() + 0.05f);
		}
	}
};

int main() {
	window = loadFullscreenWindow("Title", true, Color(1));

	glfwSetKeyCallback(window, keyCallback);

	Image* st = (new Image(GL_LINEAR))->loadData("sprites/eye.png");

	std::uniform_int_distribution<int> positionDist(-500, 500);
	auto randomPosition = std::bind(positionDist, randEngine);

	for (int i = 0; i < 10000; i++)
		Sprite::Loader().texture(st).position(Vector(randomPosition(), randomPosition())).color({1, 0, 0, 1}).load();

	MainTick tick;

	draw();
}
