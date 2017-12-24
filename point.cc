#include "point.h"
#include "util.h"

namespace simpleGL {

Point::~Point() {
	util::unloadList(components);

	parent->children.remove(this);
	util::unloadList(children);
}

}
