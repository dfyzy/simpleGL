/* Base class for detecting collisions
*/

#ifndef SIMPLE_SHAPE_H
#define SIMPLE_SHAPE_H

#include "point.h"

namespace simpleGL {

class Shape : public Point {
private:
	Shape* mask {nullptr};
	std::list<Shape*> masked;

protected:
	virtual bool isInside(Vector v) const =0;

	void stopUsingMask() {
		if (mask)	mask->masked.remove(this);
	}

	~Shape() {
		stopUsingMask();
		for (Shape* sh : masked)
			sh->mask = nullptr;
	}

public:
	Shape(Point* parent, Vector position, Vector scale, Angle rotation) : Point(parent, position, scale, rotation) {}

	Shape* getMask() const { return mask; }
	void setMask(Shape* shape) {
		if (shape == mask)	return;

		stopUsingMask();

		mask = shape;

		if (mask)	mask->masked.push_back(this);

		onChange();
	}

	bool inBounds(Vector v) {
		if (!isEnabled())	return false;

		return (mask == nullptr || mask->inBounds(v)) && isInside(getModelMatrix().inv() * v);
	}

};

}

#endif
