#ifndef SIMPLE_PHYSICS_H
#define SIMPLE_PHYSICS_H

#include <vector>

#include "updatable.h"

namespace simpleGL {

class PhysicalShape;

struct CollisionData {
	float time;//from 0.0f to 1.0f

	PhysicalShape* firstShape;
	PhysicalShape* secondShape;

	CollisionData() {}
	CollisionData(float time, PhysicalShape* firstShape, PhysicalShape* secondShape)
		: time(time), firstShape(firstShape), secondShape(secondShape) {}
};

class Physics : public Updatable<EUpdateType::Physics> {
private:
	std::list<PhysicalShape*> shapes;

	std::vector<CollisionData> futureCollisions;

public:
	Physics() {}

	void removeShape(PhysicalShape* shape) { shapes.remove(shape); }
	void addShape(PhysicalShape* shape) { shapes.push_back(shape); }

	void update() override;

};

}

#endif
