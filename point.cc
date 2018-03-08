#include "point.h"
#include "util.h"

namespace simpleGL {

Point::~Point() {
	util::unloadContainer(components);

	if (parent)	parent->children.remove(this);
	util::unloadContainer(children);
}

}
