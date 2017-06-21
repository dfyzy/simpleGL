#ifndef SIMPLE_SHAPE_H
#define SIMPLE_SHAPE_H

#include "point.h"

namespace simpleGL {

class Shape : public Point {
private:
	Shape* mask {nullptr};

protected:
	virtual bool isInside(Vector v) const =0;

	~Shape() {}

public:
	Shape(Point* parent, Vector position, Vector scale, Angle rotation) : Point(parent, position, scale, rotation) {}

	Shape* getMask() const { return mask; }
	void setMask(Shape* shape) { mask = shape; }

	bool inBounds(Vector v) {
		if (!isEnabled())	return false;

		return (mask == nullptr || mask->inBounds(v)) && isInside(getModelMatrix().inv() * v);
	}

};

enum Anchor {	TopRight,		Top,		TopLeft,
					Right,			Center,	Left,
					BottomRight,	Bottom,	BottomLeft};

inline Vector toFactor(Anchor anchor) {
	return (Vector(anchor % 3, anchor / 3) - 1);
}

class BaseBoxShape : public Shape {
private:
	Anchor anchor;
	Vector offset;

	Matrix offsetModel;

	bool needUpdtOffset {true};
	bool needUpdtOffsetModel {true};

	Vector getOffset() {
		if (needUpdtOffset) {
			needUpdtOffset = false;
			offset = toFactor(anchor) * getBounds() * 0.5f;
		}

		return offset;
	}

protected:
	void updateOffset() {
		needUpdtOffset = true;
		needUpdtOffsetModel = true;

		setChanges();
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
	Vector getRealCenter() { return getModelMatrix() * Vector(); }

	Anchor getAnchor() const { return anchor; }
	void setAnchor(Anchor panchor) {
		if (anchor == panchor)	return;

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
