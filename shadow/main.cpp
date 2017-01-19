#include <simpleGL/simpleGL.hpp>
#include <iostream>

GLFWwindow* window;

SimpleLight::Source* lamp0;
SimpleLight::Source* lamp1;
SimpleSprite* sprite;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GL_TRUE);

}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	SimpleVector sv(xpos - simpleGL::getWindowWidth()/2, simpleGL::getWindowHeight()/2 - ypos);
	lamp0->setPosition(sv);
	lamp1->setPosition(sv);
	sprite->setPosition(sv);
}

int main() {
	window = simpleGL::createFullscreenWindow("Title", true, SimpleColor(1));

	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);

	simpleGL::setTextureFiltering(GL_LINEAR);

	SimpleLight* light0 = new SimpleLight(SimpleVector(-0.25f*simpleGL::getWindowWidth(), -0.25f*simpleGL::getWindowHeight()), -1,
															simpleGL::getWindowWidth()/2, simpleGL::getWindowHeight()/2, {0});
	lamp0 = new SimpleLight::Source(light0, {}, {100}, 0, {1, 0, 0});
	SimpleLight* light1 = new SimpleLight(SimpleVector(0.25f*simpleGL::getWindowWidth(), 0), -1, simpleGL::getWindowWidth()/2, simpleGL::getWindowHeight(), {0});
	lamp1 = new SimpleLight::Source(light1, {}, {100}, 0, {0, 0, 1});

	sprite = SimpleSprite::Loader({}).color({0.5f}).load();

	simpleGL::draw();
}
