#include "simpleGL/simpleGL.h"
#include "simpleGL/window.h"

//TODO file(s) as argument
int main(int argc, char* argv[]) {
	simpleGL::Window* window = simpleGL::Window::load("Preset Editor", 1000, 700, true, true);
	//window-maximize(); when you do resizing

	unsigned width = window->getWidth();
	unsigned height = window->getHeight();

	simpleGL::draw();

	return 0;
}
