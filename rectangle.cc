#include "rectangle.h"
#include "util.h"

namespace simpleGL {

Rectangle::~Rectangle() {
	stopUsingMask();
	for (Rectangle* m : masked)
		m->mask = nullptr;

	util::unloadList(anchors);
	center->unload();
}

}
