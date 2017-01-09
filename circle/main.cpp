#include <simpleGL/simpleGL.hpp>

GLFWwindow* window;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GL_TRUE);
}

int main() {
	window = simpleGL::createFullscreenWindow("Title", false, SimpleColor(0.5f));

	glfwSetKeyCallback(window, keyCallback);

	//simpleGL::setTextureFiltering(GL_LINEAR); this will fix it

	SimpleTexture circle("circle/circle.png");
	SimpleSprite::Loader(circle).position({-30, 0}).load();//bad
	SimpleSprite::Loader(circle).position({30.5, 0.5}).load();//good

	simpleGL::draw();
}
