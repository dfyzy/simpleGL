#ifndef SIMPLE_PHYSICS_H
#define SIMPLE_PHYSICS_H

#include "updatable.h"
#include "point.h"
#include "globalInstance.h"

namespace simpleGL {

class Physics : public Updatable<EUpdateType::Physics> {
public:
	class Shape : public Point {
	private:
		//todo: aabb

	protected:
		~Shape() {
			GlobalInstance<Physics>::get()->removeShape(this);
		}

	public:
		Shape(Point* parent, Vector position, Vector scale, Angle rotation) : Point(parent, position, scale, rotation) {
			GlobalInstance<Physics>::get()->addShape(this);
		}


		//virtual

	};

private:
	std::list<Shape*> shapes;

	void removeShape(Shape* shape) { shapes.remove(shape); }
	void addShape(Shape* shape) { shapes.push_back(shape); }

public:
	Physics() {}

	void update() override;

};

}

#endif
