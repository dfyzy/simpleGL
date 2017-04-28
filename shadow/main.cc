#include <simpleGL/simpleGL.h>
#include <simpleGL/shaderData.h>
#include <simpleGL/light.h>
#include <simpleGL/cursor.h>
#include <iostream>

using namespace simpleGL;

GLFWwindow* window;

Light::Source* lamp;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GL_TRUE);
}

void update() {

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		Camera::getInstance()->setScale(Camera::getInstance()->getScale() + 0.01f);
	}
	
}

int main() {
	window = createFullscreenWindow("Title", true, Color(1));

	glfwSetKeyCallback(window, keyCallback);

	Light* light = new Light(nullptr, {}, -1, getWindowWidth(), getWindowHeight(), {0});
	lamp = new Light::Source(light, Cursor::getInstance(), {}, {1000}, 0, {0.65f, 0.65f, 0.5f});
	lamp->setFragmentShader(loadShaderSource(simpleShaderData::getLightingPow2Fragment(), GL_FRAGMENT_SHADER));

	setUpdate(update);

	draw();
}
