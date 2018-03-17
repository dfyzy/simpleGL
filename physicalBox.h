#ifndef SIMPLE_PHYSICAL_BOX_H
#define SIMPLE_PHYSICAL_BOX_H

#include "physics.h"

namespace simpleGL {

class PhysicalBox : public Physics::Shape {
private:
	Vector bounds;

public:
	PhysicalBox(Point* parent, Vector position, Vector scale, Angle rotation, Vector bounds)
		: Physics::Shape(parent, position, scale, rotation), bounds(bounds) {}



};


}

#endif
