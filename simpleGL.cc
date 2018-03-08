#include "simpleGL.h"
#include "window.h"
#include "globalInstance.h"
#include "camera.h"

namespace simpleGL {

double getDeltaTime() {
	if (Window* current = Window::getCurrent())
		return current->getDeltaTime();

	return 0.0;
}

SortedFrame* getMainFrame() {
	if (Camera* camera = GlobalInstance<Camera>::get())
		return camera->getFrame();

	return nullptr;
}

}
