#include <simpleGL/simpleGL.h>
#include <simpleGL/window.h>
#include <iostream>

using namespace simpleGL;

//TODO file(s) as argument
int main(int argc, char *argv[]) {
	Window* window = Window::load("Preset Editor", 1000, 700, true, true);
	//window->maximize(); when you do resizing

	unsigned width = window->getWidth();
	unsigned height = window->getHeight();

	draw();
}
