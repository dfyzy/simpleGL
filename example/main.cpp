#include <simpleGL/simpleGL.hpp>
#include <iostream>

SimpleSprite* brimSprite;

GLFWwindow* window;
float rotation = 0;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GL_TRUE);
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	SimpleVector sv = simpleGL::glfwToScreen(xpos, ypos);
	brimSprite->setColor(SimpleColor(sv.x, sv.y, 0));
}

void update() {
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		rotation += 0.03f;
		simpleGL::setCameraRotation(rotation);
	}
}

int main() {
	int width = 700;
	int height = 700;
	window = simpleGL::createWindowedWindow("Title", width, height, false, true, SimpleColor(0.5f));

	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);

	simpleGL::setTextureFiltering(GL_LINEAR);

	SimpleTexture* bodyFront = simpleGL::loadTexture("example\\body_front.png");
	SimpleTexture* brim = simpleGL::loadTexture("example\\brim.png");
	SimpleTexture* light = simpleGL::loadTexture("example\\light.png");
	SimpleTexture* eye = simpleGL::loadTexture("example\\eye.png");

	simpleGL::loadSprite(bodyFront, SimpleVector(), 0, SimpleColor(1));
	brimSprite = simpleGL::loadSprite(brim, SimpleVector(0, 68.6), 0, SimpleColor(1));
	SimpleSprite* lightSp = simpleGL::loadSprite(light, SimpleVector(0, 108.8), 1, SimpleColor(1));
	simpleGL::loadSprite(eye, SimpleVector(50.2, 32.1), -1, SimpleColor(1));
	simpleGL::loadSprite(eye, SimpleVector(-50.2, 32.1), -1, SimpleColor(1));

	simpleGL::loadSprite(simpleGL::getEmptyTexture(), SimpleVector(0, 0), 5, SimpleVector(3), 0.25f*3.1415927f, SimpleColor(1));

	SimpleShader customShader = simpleGL::loadShaderPath("example/custom.glsl", GL_FRAGMENT_SHADER);
	lightSp->setShader(customShader);
	customShader.setUniformf("color", {1, 0, 0, 1});

	simpleGL::setUpdate(update);

	simpleGL::draw();
}
