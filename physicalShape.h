#ifndef SIMPLE_PHYSICAL_SHAPE_H
#define SIMPLE_PHYSICAL_SHAPE_H

#include "point.h"

namespace simpleGL {

class PhysicalShape : public Point {
private:
	static std::list<PhysicalShape*> shapes;//todo: manager

	//todo: aabb
	
protected:
	~PhysicalShape() {
		shapes.remove(this);
	}

public:
	static void update();

	PhysicalShape(Point* parent, Vector position, Vector scale, Angle rotation) : Point(parent, position, scale, rotation) {
		shapes.push_back(this);
	}


	//virtual

};


}

#endif
