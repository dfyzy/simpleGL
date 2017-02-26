#include <simpleGL/simpleGL.hpp>
#include <iostream>

using namespace simpleGL;

Sprite* brimSprite;

GLFWwindow* window;
float rotation = 0;
float scale = 1;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GL_TRUE);
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	Vector sv = glfwToScreen(xpos, ypos);
	brimSprite->setColor(Color(sv.x, sv.y, 0));
}

void update() {
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		rotation += 0.03f;
		setCameraRotation(rotation);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		scale += 0.01f;
		setCameraScale(scale);
	}
}

int main() {
	int width = 1000;
	int height = 700;
	window = createWindowedWindow("Title", width, height, false, true, Color(0.5f));

	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);

	setDefaultFiltering(GL_LINEAR);

	Image bodyFront("example/body_front.png");
	Image brim("example/brim.png");
	Image light("example/light.png");
	Image eye("example/eye.png");

	brimSprite = Sprite::Loader(Texture(&brim)).position({0, 68.6}).load();
	Sprite::Loader(Texture(&bodyFront)).load();
	Sprite::Loader(Texture(&light)).position({0, 108.8}).z(1).load();
	Sprite::Loader(Texture(&eye)).position({50.2, 32.1}).z(-1).load();
	Sprite::Loader(Texture(&eye)).position({-50.2, 32.1}).z(-1).load();

	Sprite::Loader(Texture(Vector(300))).z(5).rotation(0.25f*3.1415927f).load();

	Font sans("example/Oranienbaum.ttf", 26);

	std::string lorem("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
	Text text(&sans, lorem, Vector(0, -100), -10, 1, 0, Color(0), Text::CENTER, 350);

	Light* dark = new Light({0, -200}, -10, 200, 200, {0});
	new Light::Source(dark, Vector(0, -200) + Vector(25), {100}, 0, {0.5f, 0, 0});
	new Light::Source(dark, Vector(0, -200) + Vector(-25), {100}, 0, {0, 0.5f, 0});

	setUpdate(update);

	draw();
}
