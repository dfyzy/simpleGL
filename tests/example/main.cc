#include <simpleGL/window.h>
#include <simpleGL/camera.h>
#include <simpleGL/image.h>
#include <simpleGL/lighting.h>
#include <simpleGL/font.h>
#include <simpleGL/text.h>
#include <simpleGL/cursor.h>
#include <simpleGL/timer.h>

#include <iostream>

using namespace simpleGL;

class CustomButton : public Button {
private:
	SortedSprite* sprite;

public:
	CustomButton(SortedSprite* sprite) : Button(sprite), sprite(sprite) {}

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

SortedSprite* brimSortedSprite;

float rotation = 0;
float scale = 1;

LerpTimer<SortedSprite, Vector>* timer;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GL_TRUE);
}

void cursorPosCallback() {
	Window* current = Window::getCurrent();
	Vector v = GlobalInstance<Cursor>::get()->getPosition()/Vector(current->getWidth(), current->getHeight()) + Vector(0.5);
	brimSortedSprite->setColor({v.x, v.y, 0, 1});
}

void mouseButtonCallback(int button, bool pressed) {
	if (pressed)	timer->play(false);
}

class MainTick : public Updatable<EUpdateType::Tick> {
	void update() override {
		GLFWwindow* window = Window::getCurrent()->getWindow();

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			rotation += 0.03f;
			GlobalInstance<Camera>::get()->setRotation(rotation);
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			scale -= 0.01f;
			GlobalInstance<Camera>::get()->setScale(scale);
		}
	}
};

int main() {
	int width = 1000;
	int height = 700;
	Window window("Title", width, height, true, true);

	GlobalInstance<Camera>::get()->setBaseColor(Color(0.5f));

	glfwSetKeyCallback(Window::getCurrent()->getWindow(), keyCallback);

	Image bodyFront(GL_LINEAR);	bodyFront.loadData("tests/example/body_front.png");
	Image brim(GL_LINEAR);			brim.loadData("tests/example/brim.png");
	Image light(GL_LINEAR);			light.loadData("tests/example/light.png");
	Image eye(GL_LINEAR);			eye.loadData("tests/example/eye.png");

	brimSortedSprite = SortedSprite::Loader().texture(&brim).position({0, 68.6}).load();
	(new CustomButton(SortedSprite::Loader().texture(&bodyFront).load()))->setOpaque(false);
	(new CustomButton(SortedSprite::Loader().texture(&light).position({0, 58.8}).z(1).load()))->setOpaque(true);
	new CustomButton(SortedSprite::Loader().texture(&eye).position({50.2, 32.1}).z(-1).load());
	new CustomButton(SortedSprite::Loader().texture(&eye).position({-50.2, 32.1}).z(-1).load());

	Font* sans = new Font("tests/example/Oranienbaum.ttf", 26);

	std::string lorem("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
	Text::Loader().font(sans).caption(lorem).width(350).position({0, -100}).z(-10).load();

	Lighting* dark = Lighting::Loader().pivot({1.0f, -1.0f}).position({498, -348}).bounds(500, 350).z(-100).load();
	Lighting::Source::Loader().lighting(dark).texture({100}).position(Vector(0, -150) + Vector(25)).color({0.5f, 0, 0}).load();
	Lighting::Source::Loader().lighting(dark).texture({100}).position(Vector(0, -150) + Vector(-25)).color({0, 0.5f, 0}).load();

	SortedSprite* back = SortedSprite::Loader().texture({300}).rotation(0.25f*3.1415927f).z(5).load();
	new CustomButton(back);

	MainTick mainTick;

	GlobalInstance<Cursor>::get()->setPositionCallback(cursorPosCallback);
	GlobalInstance<Cursor>::get()->setMouseButtonCallback(mouseButtonCallback);

	SortedSprite* red = SortedSprite::Loader().texture({50}).parent(GlobalInstance<Cursor>::get()).pivot({-1.0f}).color({1, 0, 0}).z(-50).load();
	red->setStencil(back);

	timer = new LerpTimer<SortedSprite, Vector>(red, SortedSprite::setPosition, {});
	timer->add(0.1, {50, 0});
	timer->add(0.1, {0, 50});
	timer->add(0.1, {0, -50});
	timer->add(0.1, {-50, 0});
	timer->add(0.1, {});

	window.draw();
}
