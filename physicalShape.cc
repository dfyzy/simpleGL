#include <cmath>

#include "physicalShape.h"
#include "physics.h"
#include "globalInstance.h"
#include "point.h"
#include <iostream>
#include <windows.h>

namespace simpleGL {

constexpr Vector PhysicalShape::staticPoints[4];

MovingEdge::MovingEdge(Vector pointA, Vector pointB, Vector deltaA, Vector deltaB) {
	this->pointA = pointA;
	this->deltaA = deltaA;

	diff = pointA - pointB;
	deltaDiff = deltaA - deltaB;

	cross = pointB.cross(pointA);
	deltaCross = deltaB.cross(deltaA);

	addedCross = pointB.cross(deltaA) + deltaB.cross(pointA);
}

bool MovingEdge::inBounds(float bounds, float pointDiff) {
	float sign = std::copysign(1.0f, bounds);
	pointDiff *= sign;
	bounds *= sign;
	return pointDiff >= 0.0f && pointDiff <= bounds;
}

bool MovingEdge::checkRoot(float root, Vector collidingPoint, Vector deltaCollidingPoint, float minRoot) {
	if (root < minRoot || root > 1.0f)	return false;

	float targetDiffX = diff.x + deltaDiff.x*root;
	if (std::abs(targetDiffX) > 0.0001f)
		return inBounds(targetDiffX, pointA.x - collidingPoint.x + (deltaA.x - deltaCollidingPoint.x)*root);
	else
		return inBounds(diff.y + deltaDiff.y*root, pointA.y - collidingPoint.y + (deltaA.y - deltaCollidingPoint.y)*root);
}

float MovingEdge::findLowestRoot(Vector collidingPoint, Vector deltaCollidingPoint, float minRoot) {
	float a = deltaCollidingPoint.cross(deltaDiff) - deltaCross;
	float b = collidingPoint.cross(deltaDiff) + deltaCollidingPoint.cross(diff) - addedCross;
	float c = collidingPoint.cross(diff) - cross;

	if (a == 0.0f) {
		if (b != 0.0f) {//in theory shouldn't happen
			float root = -c/b;

			if (checkRoot(root, collidingPoint, deltaCollidingPoint, minRoot))	return root;
		}

		return -1.0f;
	}

	float disc = b*b - 4*a*c;
	if (disc < 0.0f)	return -1.0f;

	float sqdisc = std::sqrt(disc);//todo: fast sqrt

	float firstRoot = (sqdisc - b)/(2*a);
	float secondRoot = (-sqdisc - b)/(2*a);

	bool checkedFirst = checkRoot(firstRoot, collidingPoint, deltaCollidingPoint, minRoot);
	bool checkedSecond = checkRoot(secondRoot, collidingPoint, deltaCollidingPoint, minRoot);

	if (checkedFirst && checkedSecond)	return std::min(firstRoot, secondRoot);
	else if (checkedFirst)					return firstRoot;
	else if (checkedSecond)					return secondRoot;
	else											return -1.0f;
}

PhysicalShape::PhysicalShape(Vector position, Angle rotation, float mass, Vector bounds)
	: position(position), rotation(rotation), mass(mass), bounds(bounds) {
		getInstance<Physics>()->addShape(this);

		childPoint = new Point(nullptr, position, {1}, rotation);

		for (int i = 0; i < 4; i++) {
			points[i] = (staticPoints[i]*bounds).rotate(rotation) + position;
		}
	}

PhysicalShape::~PhysicalShape() {
	getInstance<Physics>()->removeShape(this);
}

void PhysicalShape::addChild(Point* child) {
	child->setParent(childPoint);
}

void PhysicalShape::cacheNextTimeStep(float deltaTime) {
	nextPosition = position + positionSpeed*deltaTime;
	nextRotation = Angle(rotation.get() + rotationSpeed*deltaTime);

	for (int i = 0; i < 4; i++) {
		nextPoints[i] = (staticPoints[i]*bounds).rotate(nextRotation) + nextPosition;
		deltaPoints[i] = nextPoints[i] - points[i];
	}

	for (int i = 0; i < 4; i++) {
		int next = i != 3 ? i + 1 : 0;
		cachedEdges[i] = MovingEdge(points[i], points[next], deltaPoints[i], deltaPoints[next]);
	}
}

void PhysicalShape::applyNextTimeStep() {
	timeOffset = 0.0f;
	timeDuration = 1.0f;

	position = nextPosition;
	rotation = nextRotation;

	childPoint->setPosition(position);
	childPoint->setRotation(rotation);

	for (int i = 0; i < 4; i++) {
		points[i] = nextPoints[i];
	}
}

void PhysicalShape::tryCollision(PhysicalShape* other, float minRoot, float& inOutLowestRoot) {
	minRoot = (minRoot - timeOffset)/timeDuration;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float edgeRoot = cachedEdges[i].findLowestRoot(other->points[j], other->deltaPoints[j], minRoot)*timeDuration + timeOffset;

			if (edgeRoot != -1.0f) {
				std::cout << edgeRoot << ": " << i << ", " << j << std::endl;
				inOutLowestRoot = std::min(inOutLowestRoot, edgeRoot);
			}
		}
	}
}

}
