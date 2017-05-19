#include "point.h"
#include "util.h"

namespace {

std::list<simpleGL::Point*> points;

}

namespace simpleGL {

void util::pointUpdate() {
	for (Point* p : points)
		p->update();
}

Point::Point(Point* parent, Vector position, Vector scale, Angle rotation)
	: parent(parent), position(position), scale(scale), rotation(rotation) {

		if (parent) parent->children.push_back(this);
		points.push_back(this);
	}

Point::~Point() {
	parent->children.remove(this);
	for (Point* child : children)
		child->unload();

	points.remove(this);
}


}
