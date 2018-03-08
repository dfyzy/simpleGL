#ifndef SIMPLE_PHYSICAL_BOX_H
#define SIMPLE_PHYSICAL_BOX_H

#include "physicalShape.h"

namespace simpleGL {

class PhysicalBox : public PhysicalShape {
private:
	Vector bounds;

public:
	PhysicalBox(Point* parent, Vector position, Vector scale, Angle rotation, Vector bounds)
		: PhysicalShape(parent, position, scale, rotation), bounds(bounds) {}



};


}

#endif
