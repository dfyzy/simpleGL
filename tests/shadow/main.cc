#include <simpleGL/simpleGL.h>
#include <simpleGL/shader.h>
#include <simpleGL/shaderData.h>
#include <simpleGL/lighting.h>
#include <simpleGL/cursor.h>
#include <simpleGL/camera.h>
#include <simpleGL/window.h>
#include <iostream>

using namespace simpleGL;

constexpr float WASD_SPEED = 3;
const Angle ROT_SPEED {0.05f};

Window* window;

int lastX = 0, lastY = 0, lastZ = 0;

Point* target;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GL_TRUE);
}

class MainTick : public Updatable<EUpdateType::Tick> {
	void update() {
		int x, y, z;

		bool d = glfwGetKey(window->getWindow(), GLFW_KEY_D) == GLFW_PRESS;
		bool a = glfwGetKey(window->getWindow(), GLFW_KEY_A) == GLFW_PRESS;
		bool w = glfwGetKey(window->getWindow(), GLFW_KEY_W) == GLFW_PRESS;
		bool s = glfwGetKey(window->getWindow(), GLFW_KEY_S) == GLFW_PRESS;
		bool q = glfwGetKey(window->getWindow(), GLFW_KEY_Q) == GLFW_PRESS;
		bool e = glfwGetKey(window->getWindow(), GLFW_KEY_E) == GLFW_PRESS;

		if (d && a)	x = lastX;
		else			x = d - a;

		if (w && s)	y = lastY;
		else			y = w - s;

		if (q && e)	z = lastZ;
		else			z = q - e;

		target->addPosition(Vector(x, y).normalize() * WASD_SPEED);
		target->addRotation(ROT_SPEED * z);
	}
};

int main() {
	window = Window::load("Title", 1240, 720, false, false);
	Camera::getInstance()->setBaseColor(Color(1));

	glfwSetKeyCallback(window->getWindow(), keyCallback);

	Lighting* light = Lighting::Loader().z(-1).bounds(window->getWidth(), window->getHeight()).base({0.2f}).load();

	Lighting::Source* lamp2 = Lighting::Source::Loader().lighting(light).texture({1000}).position({250, -250}).color({0.9f, 0, 0}).load();
	lamp2->setFragmentShader(loadShaderSource(shaderData::getLightingPow2Fragment(), GL_FRAGMENT_SHADER));

	Lighting::Source* lamp3 = Lighting::Source::Loader().lighting(light).texture({1000}).position({-250, -250}).color({0, 0, 0.9f}).load();
	lamp3->setFragmentShader(loadShaderSource(shaderData::getLightingPow2Fragment(), GL_FRAGMENT_SHADER));

	Lighting::Source* lamp4 = Lighting::Source::Loader().lighting(light).texture({1000}).position({0, 300}).color({0, 0.9f, 0}).load();
	lamp4->setFragmentShader(loadShaderSource(shaderData::getLightingPow2Fragment(), GL_FRAGMENT_SHADER));

	Lighting::Source* lamp5 = Lighting::Source::Loader().lighting(light).texture({1000}).position({250, 250}).color({0, 0.9f, 0.9f}).load();
	lamp5->setFragmentShader(loadShaderSource(shaderData::getLightingPow2Fragment(), GL_FRAGMENT_SHADER));

	Lighting::Source* lamp6 = Lighting::Source::Loader().lighting(light).texture({1000}).position({-250, 250}).color({0.9f, 0.9f, 0}).load();
	lamp6->setFragmentShader(loadShaderSource(shaderData::getLightingPow2Fragment(), GL_FRAGMENT_SHADER));

	Lighting::Source* lamp7 = Lighting::Source::Loader().lighting(light).texture({1000}).position({0, -300}).color({0.9f, 0, 0.9f}).load();
	lamp7->setFragmentShader(loadShaderSource(shaderData::getLightingPow2Fragment(), GL_FRAGMENT_SHADER));

	target = Lighting::Shadow::Loader().lighting(light).bounds({50}).pivot({-1.0f, 1.0f}).load();

	MainTick tick;

	draw();
}
