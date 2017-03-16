#include <simpleGL/simpleGL.h>
#include <iostream>

using namespace simpleGL;

class CustomButton : public Button {
private:
	int ind;

public:
	CustomButton (int i) : ind(i) {}

	void onEnter() { std::cout << "(" << ind << ") Enter" << std::endl; }
	void onExit() { std::cout << "(" << ind << ") Exit" << std::endl; }
};

Sprite* brimSprite;

GLFWwindow* window;
float rotation = 0;
float scale = 1;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GL_TRUE);
}

void cursorPosCallback(Cursor* instance) {
	Vector v = instance->getPosition()/Vector(getWindowWidth(), getWindowHeight()) + Vector(0.5);
	brimSprite->setColor({v.x, v.y, 0, 1});
}

void update() {
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		rotation += 0.03f;
		Camera::getInstance()->setRotation(rotation);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		scale -= 0.01f;
		Camera::getInstance()->setScale(scale);
	}
}

int main() {
	int width = 1000;
	int height = 700;
	window = createWindowedWindow("Title", width, height, false, true, Color(0.5f));

	glfwSetKeyCallback(window, keyCallback);

	Image bodyFront("example/body_front.png", GL_LINEAR);
	Image brim("example/brim.png", GL_LINEAR);
	Image light("example/light.png", GL_LINEAR);
	Image eye("example/eye.png", GL_LINEAR);

	brimSprite = Sprite::Loader(Texture(&brim)).position({0, 68.6}).load();
	Cursor::getInstance()->addButton(Sprite::Loader(Texture(&bodyFront)).load(), new CustomButton(0));
	Cursor::getInstance()->addButton(Sprite::Loader(Texture(&light)).position({0, 108.8}).z(1).load(), new CustomButton(1));
	Cursor::getInstance()->addButton(Sprite::Loader(Texture(&eye)).position({50.2, 32.1}).z(-1).load(), new CustomButton(2));
	Cursor::getInstance()->addButton(Sprite::Loader(Texture(&eye)).position({-50.2, 32.1}).z(-1).load(), new CustomButton(3));

	Cursor::getInstance()->addButton(Sprite::Loader(Texture(Vector(300))).z(5).rotation(0.25f*3.1415927f).load(), new CustomButton(5));

	Font sans("example/Oranienbaum.ttf", 26);

	std::string lorem("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
	Text text(&sans, lorem, Vector(0, -100), -10, 1, 0, Color(0), Text::CENTER, 350);

	Light* dark = new Light(nullptr, {0, -200}, -10, 200, 200, {0});
	new Light::Source(dark, nullptr, Vector(0, -200) + Vector(25), {100}, 0, {0.5f, 0, 0});
	new Light::Source(dark, nullptr, Vector(0, -200) + Vector(-25), {100}, 0, {0, 0.5f, 0});

	setUpdate(update);

	Cursor::getInstance()->setPositionCallback(cursorPosCallback);
	Sprite::Loader({50}).parent(Cursor::getInstance()).z(-10).color({1, 0, 0}).load();

	draw();
}
