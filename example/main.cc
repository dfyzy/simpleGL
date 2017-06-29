#include <simpleGL/simpleGL.h>
#include <simpleGL/lighting.h>
#include <simpleGL/text.h>
#include <simpleGL/cursor.h>
#include <simpleGL/timer.h>

#include <iostream>

using namespace simpleGL;

class CustomButton : public Button {
private:
	Sprite* sprite;

public:
	CustomButton(Sprite* sprite) : Button(sprite), sprite(sprite) {}

	void onEnter() {
		Color c = sprite->getColor();
		c.a -= 0.5f;
		sprite->setColor(c);
	}
	void onExit() {
		Color c = sprite->getColor();
		c.a += 0.5f;
		sprite->setColor(c);
	}
};

Sprite* brimSprite;

GLFWwindow* window;
float rotation = 0;
float scale = 1;

LerpTimer<Sprite, Vector>* timer;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GL_TRUE);
}

void cursorPosCallback(Cursor* instance) {
	Vector v = instance->getPosition()/Vector(getWindowWidth(), getWindowHeight()) + Vector(0.5);
	brimSprite->setColor({v.x, v.y, 0, 1});
}

void mouseButtonCallback(Cursor* instance, int button, bool pressed) {
	if (pressed)	timer->play(false);
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
	window = loadWindow("Title", width, height, false, true, Color(0.5f));

	glfwSetKeyCallback(window, keyCallback);

	Image* bodyFront = new Image("example/body_front.png", GL_LINEAR);
	Image* brim = new Image("example/brim.png", GL_LINEAR);
	Image* light = new Image("example/light.png", GL_LINEAR);
	Image* eye = new Image("example/eye.png", GL_LINEAR);

	brimSprite = new Sprite(Texture(brim), Data().position({0, 68.6}), 0);
	(new CustomButton(new Sprite(Texture(bodyFront), Data(), 0)))->setOpaque(false);
	(new CustomButton(new Sprite(Texture(light), Data().position({0, 58.8}), 1)))->setOpaque(true);
	new CustomButton(new Sprite(Texture(eye), Data().position({50.2, 32.1}), -1));
	new CustomButton(new Sprite(Texture(eye), Data().position({-50.2, 32.1}), -1));

	Font* sans = new Font("example/Oranienbaum.ttf", 26);

	std::string lorem("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
	new Text(sans, lorem, Text::CENTER, 350, nullptr, Vector(0, -100), -10, 1, 0, Color(0));

	Lighting* dark = new Lighting(Data().anchor(Left).position({0, -200}), -100, 200, 200, {0});
	new Lighting::Source(dark, {100}, Data().position(Vector(0, -200) + Vector(25)).color({0.5f, 0, 0}));
	new Lighting::Source(dark, {100}, Data().position(Vector(0, -200) + Vector(-25)).color({0, 0.5f, 0}));

	Sprite* back = new Sprite({300}, Data().rotation(0.25f*3.1415927f), 5);
	new CustomButton(back);

	setUpdate(update);

	Cursor::getInstance()->setPositionCallback(cursorPosCallback);
	Cursor::getInstance()->setMouseButtonCallback(mouseButtonCallback);

	Sprite* red = new Sprite({50}, Data().parent(Cursor::getInstance()).color({1, 0, 0}), -50);
	red->setStencil(back);
	timer = new LerpTimer<Sprite, Vector>(red, Sprite::setPosition, {});
	timer->add(0.1, {50, 0});
	timer->add(0.1, {0, 50});
	timer->add(0.1, {0, -50});
	timer->add(0.1, {-50, 0});
	timer->add(0.1, {});

	draw();
}
