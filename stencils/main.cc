#include <simpleGL/simpleGL.h>
#include <random>
#include <functional>
#include <ctime>
#include <iostream>

using namespace simpleGL;

constexpr float SQRT_2 = std::sqrt(2);

constexpr int WIN_SIZE = 700;
constexpr int H_WIN_SIZE = WIN_SIZE / 2;
constexpr float H_H_WIN_SIZE = H_WIN_SIZE / 2;

constexpr int SPRITES_NUM = 200;

std::mt19937 posEngine(time(0));
std::uniform_int_distribution<int> posDist(-H_WIN_SIZE, H_WIN_SIZE);
auto posRand = std::bind(posDist, posEngine);

std::mt19937 speedEngine(time(0) + 1);
std::uniform_int_distribution<int> speedDist(5, 10);
auto speedRand = std::bind(speedDist, speedEngine);

std::mt19937 scaleEngine(time(0) + 2);
std::uniform_real_distribution<float> scaleDist(0.05f, 0.1f);
auto scaleRand = std::bind(scaleDist, scaleEngine);

std::mt19937 colorEngine(time(0) + 3);
std::uniform_real_distribution<float> colorDist(0, 1);
auto colorRand = std::bind(colorDist, colorEngine);

GLFWwindow* window;

Point* anchor;

std::pair<Sprite*, int> sprites[4*SPRITES_NUM];

Angle angles[4] {{PI}, {PI*1.5f}, {0}, {PI*0.5f}};

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GL_TRUE);

}

void update() {
	int dir = (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) - (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
	if (dir) {
		Angle newRot = anchor->getRotation() - 0.025f*dir;
		anchor->setRotation(newRot);

		float ang = newRot.get()/(PI*0.5f);
		float part = ang - std::floor(ang);
		if (part > 0.5f)	part = 1 - part;
		part = std::pow(1 - part*2, 2);

		anchor->setScale(2/(SQRT_2 + 1) + part*(SQRT_2 - 1)/(SQRT_2 + 1));
	}

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < SPRITES_NUM; j++) {
			int ind = j + i*SPRITES_NUM;

			sprites[ind].first->translate({sprites[ind].second, 0});

			if (sprites[ind].first->getPosition().x > H_WIN_SIZE) {
				sprites[ind].first->setPosition({-H_WIN_SIZE, posRand()});
				sprites[ind].first->setScale({scaleRand()});
				sprites[ind].second = speedRand();
			}
		}
}

int main() {
	window = loadWindow("Title", WIN_SIZE, WIN_SIZE, false, true, {0});

	glfwSetKeyCallback(window, keyCallback);

	Image* st = new Image("stencils/big_circle.png", GL_LINEAR);

	Vector offsets[4] {{1, 1}, {-1, 1}, {-1, -1}, {1, -1}};

	anchor = new Point();

	for (int i = 0; i < 4; i++) {
		Color c{colorRand(), colorRand(), colorRand()};
		Sprite* back = new Sprite(st, Data().parent(anchor).position(Vector(H_H_WIN_SIZE) * offsets[i]).color(c), 0);

		Point* par = new Point(nullptr, {}, {1}, angles[i]);

		for (int j = 0; j < SPRITES_NUM; j++) {
			Sprite* sprite = new Sprite(st, Data().parent(par).position({posRand(), posRand()})
																		.scale({scaleRand()}).color(Color(1) - c), 0);
			sprite->setStencil(back);
			sprites[j + i*SPRITES_NUM] = {sprite, speedRand()};
		}
	}

	setUpdate(update);

	draw();
}
