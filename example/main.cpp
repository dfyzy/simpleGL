#include <boost/asio.hpp>

#include "../simpleGL.hpp"
#include <iostream>

SimpleSprite* brimSprite;

int width, height;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GL_TRUE);

}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	brimSprite->changeColor(Color(xpos/width, 1 - ypos/height, 0));
}

int main() {

	width = 500;
	height = 500;
	GLFWwindow* window = simpleGL::createWindowedWindow("Title", width, height, false, true);

	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);

	simpleGL::startDrawThread();

	SimpleTexture* bodyFront = simpleGL::loadTexture("example\\body_front.png");
	SimpleTexture* brim = simpleGL::loadTexture("example\\brim.png");
	SimpleTexture* light = simpleGL::loadTexture("example\\light.png");
	SimpleTexture* eye = simpleGL::loadTexture("example\\eye.png");

	bodyFront->loadSprite(0, 0, 0, Color(1));//338/101.25*(0.499 + ) + 123/2 + /2
	brimSprite = brim->loadSprite(0, 68.6, 0, Color(1));
	light->loadSprite(0, 108.8, 1, Color(1));
	eye->loadSprite(50.2, 32.1, -1, Color(1));
	eye->loadSprite(-50.2, 32.1, -1, Color(1));

	boost::asio::io_service io;
	boost::asio::deadline_timer timer(io);

	while (!glfwWindowShouldClose(window)) {
		timer.expires_from_now(boost::posix_time::milliseconds(25));
		glfwPollEvents();

		timer.wait();
	}

	simpleGL::joinDrawThread();
}
