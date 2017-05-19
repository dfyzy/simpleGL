#ifndef SIMPLE_SHAPE_H
#define SIMPLE_SHAPE_H

#include "point.h"

namespace simpleGL {

class Shape : public Point {
protected:
	virtual bool isInside(Vector v) const =0;

	~Shape() {}

public:
	Shape(Point* parent, Vector position, Vector scale, Angle rotation) : Point(parent, position, scale, rotation) {}

	bool inBounds(Vector v) {
		if (!isEnabled())	return false;

		return isInside(getModelMatrix().inv() * v);
	}

};

class BaseBoxShape : public Shape {
public:
	enum Anchor {	UR,	U,		UL,
						R,		C,		L,
						DR,	D,		DL};

private:
	Anchor anchor;
	Vector offset;

	Matrix offsetModel;

	bool needUpdtOffset {true};
	bool needUpdtOffsetModel {true};

	Vector getOffset() {
		if (needUpdtOffset) {
			needUpdtOffset = false;
			offset = (Vector(anchor % 3, anchor / 3) - 1) * getBounds() * 0.5f;
		}

		return offset;
	}

protected:
	void updateOffset() {
		needUpdtOffset = true;
		needUpdtOffsetModel = true;
	}

	bool isInside(Vector v) const {
		Vector dist = v.abs();
		Vector hBounds = getBounds()/2;

		return (dist.x < hBounds.x) && (dist.y < hBounds.y);
	}

	~BaseBoxShape() {}

public:
	BaseBoxShape(Point* parent, Anchor anchor, Vector position, Vector scale, Angle rotation)
				: Shape(parent, position, scale, rotation), anchor(anchor) {}

	virtual Vector getBounds() const =0;

	Vector getCenter() { return getPosition() + getOffset(); }

	Anchor getAnchor() const { return anchor; }
	void setAnchor(Anchor panchor) {
		anchor = panchor;

		updateOffset();
	}

	Matrix getModelMatrix() {
		if (needUpdtOffsetModel) {
			needUpdtOffsetModel = false;
			offsetModel = Point::getModelMatrix() * Matrix::translate(getOffset());
		}

		return offsetModel;
	}

	void updateModel() {
		needUpdtOffsetModel = true;

		Point::updateModel();
	}

};

class BoxShape : public BaseBoxShape {
private:
	Vector bounds;

protected:
	~BoxShape() {}

public:
	BoxShape(Point* parent, Anchor anchor, Vector position, Vector scale, Angle rotation, Vector bounds)
		: BaseBoxShape(parent, anchor, position, scale, rotation), bounds(bounds) {}

	Vector getBounds() const { return bounds; }

};

}

#endif
