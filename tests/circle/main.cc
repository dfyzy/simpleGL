#include <simpleGL/simpleGL.h>
#include <simpleGL/image.h>
#include <simpleGL/sortedSprite.h>
#include <simpleGL/window.h>
#include <simpleGL/camera.h>

using namespace simpleGL;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GL_TRUE);
}

int main() {
	Window* window = Window::loadFullscreen("Title", false);
	Camera::getInstance()->setBaseColor(Color(0.5f));

	glfwSetKeyCallback(window->getWindow(), keyCallback);

	Image circle(GL_NEAREST); circle.loadData("tests/circle/circle.png");
	Texture texCircle(&circle);
	SortedSprite::Loader().texture(texCircle).position({-30, 0}).load();
	SortedSprite::Loader().texture(texCircle).position({30.5, 0.5}).load();

	draw();
}
