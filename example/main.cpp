#include <boost/asio.hpp>

#include "../simpleGL.hpp"
#include <iostream>

SimpleTexture* eye;

SimpleSprite* light;

int width, height;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GL_TRUE);

}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	light->changeColor(Color(xpos/width, 1 - ypos/height, 0));
	eye->loadSprite(xpos - width*0.5f, height*0.5f - ypos, -1, Color(1));
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		double x, y;
		glfwGetCursorPos(window, &x, &y);


	}
}

int main() {

	width = 500;
	height = 500;
	GLFWwindow* window = simpleGL::createWindowedWindow("Title", width, height, false, true);

	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	// glfwSetMouseButtonCallback(window, mouseButtonCallback);

	simpleGL::startDrawThread();

	SimpleTexture* bodyFront = simpleGL::loadTexture("example\\body_front.png");
	SimpleTexture* brim = simpleGL::loadTexture("example\\brim.png");
	eye = simpleGL::loadTexture("example\\eye.png");

	bodyFront->loadSprite(0, 0, 0, Color(1));
	light = brim->loadSprite(0, 68.5, 0, Color(1));

	boost::asio::io_service io;
	boost::asio::deadline_timer timer(io);

	while (!glfwWindowShouldClose(window)) {
		timer.expires_from_now(boost::posix_time::milliseconds(25));
		glfwPollEvents();

		timer.wait();
	}

	simpleGL::joinDrawThread();
}
