#include <simpleGL/simpleGL.hpp>
#include <iostream>

typedef boost::chrono::duration<double, boost::ratio<1, 30>> duration;

SimpleSprite* brimSprite;

int width, height;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GL_TRUE);

}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	brimSprite->changeColor(SimpleColor(xpos/width, 1 - ypos/height, 0));
}

int main() {

	width = 500;
	height = 500;
	GLFWwindow* window = simpleGL::createWindowedWindow("Title", width, height, false, true);

	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);

	boost::thread thread = simpleGL::startDrawThread();

	simpleGL::changeTextureFiltering(GL_LINEAR);

	SimpleTexture* bodyFront = simpleGL::loadTexture("example\\body_front.png");
	SimpleTexture* brim = simpleGL::loadTexture("example\\brim.png");
	SimpleTexture* light = simpleGL::loadTexture("example\\light.png");
	SimpleTexture* eye = simpleGL::loadTexture("example\\eye.png");

	bodyFront->loadSprite(SimplePosition(), SimpleColor(1));
	brimSprite = brim->loadSprite(SimplePosition(0, 68.6), SimpleColor(1));
	light->loadSprite(SimplePosition(0, 108.8, 1), SimpleColor(1));
	eye->loadSprite(SimplePosition(50.2, 32.1, -1), SimpleColor(1));
	eye->loadSprite(SimplePosition(-50.2, 32.1, -1), SimpleColor(1));

	while (!glfwWindowShouldClose(window)) {
		auto lastTimePoint = boost::chrono::steady_clock::now();

		glfwPollEvents();

		auto thisTimePoint = boost::chrono::steady_clock::now();
		boost::this_thread::sleep_for(duration(1) - (thisTimePoint - lastTimePoint));
	}

	thread.join();
}
