#ifndef SIMPLE_SHAPE_H
#define SIMPLE_SHAPE_H

#include "sprite.h"

namespace simpleGL {

class Shape {
protected:
	Point* point;

	virtual bool isInside(Vector v) const =0;

public:
	Shape(Point* point) : point(point) {}
	virtual ~Shape() {}

	Point* getPoint() const { return point; }

	bool inBounds(Vector v) const {
		if (!point->isEnabled())	return false;

		return isInside(point->getModelMatrix().inv() * v);
	}
};

class BaseBoxShape : public Shape {
protected:
	bool isInside(Vector v) const {
		Vector dist = v.abs();
		Vector hBounds = getBounds()/2;

		return (dist.x < hBounds.x) && (dist.y < hBounds.y);
	}

public:
	BaseBoxShape(Point* point) : Shape(point) {}

	virtual Vector getBounds() const =0;

};

class BoxShape : public BaseBoxShape {
private:
	Vector bounds;

public:
	BoxShape(Point* parent, Vector position, Vector scale, Vector rotation, Vector bounds)
		: BaseBoxShape(new Point(parent, position, scale, rotation)), bounds(bounds) {}

	~BoxShape() { getPoint()->unload(); }

	Vector getBounds() const { return bounds; }

};

class SpriteShape : public BaseBoxShape {
private:
	Sprite* sprite;

	Vector getBounds() const { return sprite->getTexture().getBounds(); }

public:
	SpriteShape(Sprite* sprite) : BaseBoxShape(sprite), sprite(sprite) {}

};

}

#endif
