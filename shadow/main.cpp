#include <simpleGL/simpleGL.hpp>
#include <simpleGL/shaderData.hpp>
#include <iostream>

using namespace simpleGL;

GLFWwindow* window;

Light::Source* lamp;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GL_TRUE);

}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	lamp->setPosition(Vector(xpos - getWindowWidth()/2, getWindowHeight()/2 - ypos));
}

int main() {
	window = createFullscreenWindow("Title", true, Color(1));

	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);

	setDefaultFiltering(GL_LINEAR);

	Light* light = new Light({}, -1, getWindowWidth(), getWindowHeight(), {0});
	lamp = new Light::Source(light, {}, {1000}, 0, {0.65f, 0.65f, 0.5f});
	lamp->setFragmentShader(loadShaderSource(simpleShaderData::getLightingPow2Fragment(), GL_FRAGMENT_SHADER));

	draw();
}
