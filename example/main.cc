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

void cursorPosCallback() {
	Vector v = Cursor::getInstance()->getPosition()/Vector(getWindowWidth(), getWindowHeight()) + Vector(0.5);
	brimSprite->setColor({v.x, v.y, 0, 1});
}

void mouseButtonCallback(int button, bool pressed) {
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

	Image* bodyFront = (new Image(GL_LINEAR))->loadData("example/body_front.png");
	Image* brim = (new Image(GL_LINEAR))->loadData("example/brim.png");
	Image* light = (new Image(GL_LINEAR))->loadData("example/light.png");
	Image* eye = (new Image(GL_LINEAR))->loadData("example/eye.png");

	brimSprite = Sprite::Loader().texture(brim).position({0, 68.6}).load();
	(new CustomButton(Sprite::Loader().texture(bodyFront).load()))->setOpaque(false);
	(new CustomButton(Sprite::Loader().texture(light).position({0, 58.8}).z(1).load()))->setOpaque(true);
	new CustomButton(Sprite::Loader().texture(eye).position({50.2, 32.1}).z(-1).load());
	new CustomButton(Sprite::Loader().texture(eye).position({-50.2, 32.1}).z(-1).load());

	Font* sans = new Font("example/Oranienbaum.ttf", 26);

	std::string lorem("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
	Text::Loader().font(sans).caption(lorem).width(350).position({0, -100}).z(-10).load();

	Lighting* dark = Lighting::Loader().anchor(EAnchor::Left).position({0, -150}).bounds(200, 200).z(-100).load();
	Lighting::Source::Loader().lighting(dark).texture({100}).position(Vector(0, -150) + Vector(25)).color({0.5f, 0, 0}).load();
	Lighting::Source::Loader().lighting(dark).texture({100}).position(Vector(0, -150) + Vector(-25)).color({0, 0.5f, 0}).load();

	Sprite* back = Sprite::Loader().texture({300}).rotation(0.25f*3.1415927f).z(5).load();
	new CustomButton(back);

	setUpdate(update);

	Cursor::getInstance()->setPositionCallback(cursorPosCallback);
	Cursor::getInstance()->setMouseButtonCallback(mouseButtonCallback);

	Sprite* red = Sprite::Loader().texture({50}).parent(Cursor::getInstance()).anchor(EAnchor::BottomLeft).color({1, 0, 0}).z(-50).load();
	red->setStencil(back);

	timer = new LerpTimer<Sprite, Vector>(red, Sprite::setPosition, {});
	timer->add(0.1, {50, 0});
	timer->add(0.1, {0, 50});
	timer->add(0.1, {0, -50});
	timer->add(0.1, {-50, 0});
	timer->add(0.1, {});

	draw();
}
