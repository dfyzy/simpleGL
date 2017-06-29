#include <simpleGL/simpleGL.h>
#include <simpleGL/shaderData.h>
#include <simpleGL/lighting.h>
#include <simpleGL/cursor.h>
#include <iostream>

using namespace simpleGL;

constexpr float WASD_SPEED = 3;

GLFWwindow* window;

int lastX = 0, lastY = 0;

Point* target;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GL_TRUE);
}

void update() {
	int x, y;

	bool d = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
	bool a = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
	bool w = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
	bool s = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;

	if (d && a)	x = lastX;
	else			x = d - a;

	if (w && s)	y = lastY;
	else			y = w - s;

	target->setPosition(target->getPosition() + Vector(x, y).normalize() * WASD_SPEED);
}

int main() {
	window = loadWindow("Title", 1240, 720, false, false, Color(1));

	glfwSetKeyCallback(window, keyCallback);

	Lighting* light = new Lighting({}, -1, getWindowWidth(), getWindowHeight(), {0.2f});
	// Lighting::Source* lamp = new Lighting::Source(light, {1000}, Data().parent(Cursor::getInstance()).color({0.5f}));
	// lamp->setFragmentShader(loadShaderSource(shaderData::getLightingPow2Fragment(), GL_FRAGMENT_SHADER));

	Lighting::Source* lamp2 = new Lighting::Source(light, {1000}, Data().position({300, 0}).color({0.9f, 0, 0}));
	lamp2->setFragmentShader(loadShaderSource(shaderData::getLightingPow2Fragment(), GL_FRAGMENT_SHADER));

	Lighting::Source* lamp3 = new Lighting::Source(light, {1000}, Data().position({-300, 0}).color({0, 0, 0.9f}));
	lamp3->setFragmentShader(loadShaderSource(shaderData::getLightingPow2Fragment(), GL_FRAGMENT_SHADER));

	Lighting::Source* lamp4 = new Lighting::Source(light, {1000}, Data().position({0, 300}).color({0, 0.9f, 0}));
	lamp4->setFragmentShader(loadShaderSource(shaderData::getLightingPow2Fragment(), GL_FRAGMENT_SHADER));

	target = new Lighting::Shadow(light, {50}, {});

	setUpdate(update);

	draw();
}
