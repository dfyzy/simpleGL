#include <simpleGL/simpleGL.hpp>

using namespace simpleGL;

GLFWwindow* window;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GL_TRUE);
}

int main() {
	window = createFullscreenWindow("Title", false, Color(0.5f));

	glfwSetKeyCallback(window, keyCallback);

	//setTextureFiltering(GL_LINEAR); this will fix it

	Texture circle("circle/circle.png");
	Sprite::Loader(circle).position({-30, 0}).load();//bad
	Sprite::Loader(circle).position({30.5, 0.5}).load();//good

	draw();
}
