#include <simpleGL/simpleGL.hpp>
#include <iostream>

SimpleSprite* brimSprite;

GLFWwindow* window;
float rotation = 0;
float scale = 1;

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

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		scale += 0.01f;
		simpleGL::setCameraScale(scale);
	}
}

int main() {
	int width = 1000;
	int height = 700;
	window = simpleGL::createWindowedWindow("Title", width, height, false, true, SimpleColor(0.5f));

	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);

	simpleGL::setTextureFiltering(GL_LINEAR);

	SimpleTexture bodyFront("example/body_front.png");
	SimpleTexture brim("example/brim.png");
	SimpleTexture light("example/light.png");
	SimpleTexture eye("example/eye.png");

	SimpleSprite::Loader(bodyFront).load();
	brimSprite = SimpleSprite::Loader(brim).position({0, 68.6}).load();
	SimpleSprite* lightSp = SimpleSprite::Loader(light).position({0, 108.8}).z(1).load();
	SimpleSprite::Loader(eye).position({50.2, 32.1}).z(-1).load();
	SimpleSprite::Loader(eye).position({-50.2, 32.1}).z(-1).load();

	SimpleSprite::Loader({}).z(5).bounds({3}).rotation(0.25f*3.1415927f).load();

	GLuint customShader = simpleGL::loadShaderPath("example/custom.glsl", GL_FRAGMENT_SHADER);
	lightSp->setFragmentShader(customShader);
	glProgramUniform4f(customShader, glGetUniformLocation(customShader, "color"), 1, 0, 0, 1);

	SimpleFont sans("example/Oranienbaum.ttf", 26);

	std::string lorem("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
	SimpleText text(&sans, lorem, SimpleVector(0, -100), -10, 1, 0, SimpleColor(0), SimpleText::CENTER, 350);

	SimpleLight* dark = new SimpleLight({0, -200}, -10, 200, 200, {0});
	new SimpleLight::Source(dark, SimpleVector(0, -200) + SimpleVector(25), {100}, 0, {0.5f, 0, 0});
	new SimpleLight::Source(dark, SimpleVector(0, -200) + SimpleVector(-25), {100}, 0, {0, 0.5f, 0});

	simpleGL::setUpdate(update);

	simpleGL::draw();
}
