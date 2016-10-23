#include <boost/asio.hpp>

#include "../simpleGL.hpp"
#include <iostream>


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GL_TRUE);

}

int main() {

	GLFWwindow* window = simpleGL::createWindowedWindow("Title", 500, 500, false, true);

	glfwSetKeyCallback(window, keyCallback);

	simpleGL::startDrawThread();

	SimpleTexture* bodyFront = simpleGL::addTexture("example\\body_front.png");
	SimpleTexture* brim = simpleGL::addTexture("example\\brim.png");

	bodyFront->createSprite(0, 0, 0, Color(1));
	brim->createSprite(0, 68.5, 0, Color(1));

	boost::asio::io_service io;
	boost::asio::deadline_timer timer(io);

	while (!glfwWindowShouldClose(window)) {
		timer.expires_from_now(boost::posix_time::milliseconds(25));
		glfwPollEvents();

		timer.wait();
	}

	simpleGL::joinDrawThread();
}
