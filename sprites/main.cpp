#include <simpleGL/simpleGL.hpp>
#include <random>

typedef boost::chrono::duration<double, boost::ratio<1, 30>> duration;

std::mt19937 randEngine(time(0));
float rotation = 0;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GL_TRUE);

}

int main() {
	GLFWwindow* window = simpleGL::createFullscreenWindow("Title", true);

	glfwSetKeyCallback(window, keyCallback);

	boost::thread thread = simpleGL::startDrawThread(SimpleColor(1));
	simpleGL::setTextureFiltering(GL_LINEAR);

	SimpleTexture* st = simpleGL::loadTexture("sprites/eye.png");

	std::uniform_int_distribution<int> positionDist(-500, 500);
	auto randomPosition = std::bind(positionDist, randEngine);

	for (int i = 0; i < 10000; i++)
		simpleGL::loadSprite(st, SimpleVector(randomPosition(), randomPosition()), 0, SimpleColor(1, 0, 0, 1));

	while (!glfwWindowShouldClose(window)) {
		auto lastTime = boost::chrono::steady_clock::now();

		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			rotation += 0.05f;
			simpleGL::setCameraRotation(rotation);
		}

		auto thisTime = boost::chrono::steady_clock::now();
		boost::this_thread::sleep_for(duration(1) - (thisTime - lastTime));
	}

	thread.join();
}
