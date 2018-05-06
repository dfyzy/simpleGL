#ifndef SIMPLE_PHYSICAL_SHAPE_H
#define SIMPLE_PHYSICAL_SHAPE_H

#include "vector.h"
#include "angle.h"

namespace simpleGL {

struct MovingEdge {
	Vector pointA;
	Vector deltaA;

	Vector diff;
	Vector deltaDiff;

	float cross;
	float deltaCross;

	float addedCross;

	MovingEdge() {}
	MovingEdge(Vector pointA, Vector pointB, Vector deltaA, Vector deltaB);

	bool inBounds(float bounds, float pointDiff);

	bool checkRoot(float root, Vector collidingPoint, Vector deltaCollidingPoint, float minRoot);

	float findLowestRoot(Vector collidingPoint, Vector deltaCollidingPoint, float minRoot);

};

class Point;

class PhysicalShape {//this just will be a box for now
private:
	constexpr static Vector staticPoints[4] {{0.5f, 0.5f}, {0.5f, -0.5f}, {-0.5f, -0.5f}, {-0.5f, 0.5f}};

	Vector position;
	Angle rotation;

	Vector positionSpeed;
	float rotationSpeed = 0.0f;

	float mass = 1.0f;

	Point* childPoint;

	Vector nextPosition;
	Angle nextRotation;

	float timeOffset = 0.0f;
	float timeDuration = 1.0f;

	//todo: bounding box

	Vector bounds;

	Vector points[4];

	Vector nextPoints[4];
	Vector deltaPoints[4];
	MovingEdge cachedEdges[4];

public:
	PhysicalShape(Vector position, Angle rotation, float mass, Vector bounds);

	~PhysicalShape();

	Vector getPosition() const { return position; }

	Angle getRotation() const { return rotation; }

	void setPositionSpeed(Vector v) { positionSpeed = v; }
	Vector getPositionSpeed() const { return positionSpeed; }

	void setRotationSpeed(float f) { rotationSpeed = f; }
	float getRotationSpeed() const { return rotationSpeed; }

	void addChild(Point* child);

	void cacheNextTimeStep(float deltaTime);
	void applyNextTimeStep();

	//temp
	void stop(float offset) {
		timeOffset = offset;
		timeDuration = 1.0f - offset;

		position = position*timeDuration + nextPosition*timeOffset;
		rotation = Angle(rotation.get()*timeDuration + nextRotation.get()*timeOffset);

		positionSpeed = Vector(0.0f);
		rotationSpeed = 0.0f;
	}

	void tryCollision(PhysicalShape* other, float minRoot, float& inOutLowestRoot);

};

}

#endif
