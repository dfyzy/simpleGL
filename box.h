/* Shape that represents rectangle
*/

#ifndef SIMPLE_BOX_SHAPE
#define SIMPLE_BOX_SHAPE

#include "shape.h"

namespace simpleGL {

class Box : public Shape {
private:
	Vector bounds;

protected:
	bool isInside(Vector v) const override {
		Vector dist = v.abs();
		Vector hBounds = bounds/2.0f;

		return (dist.x < hBounds.x) && (dist.y < hBounds.y);
	}

	~Box() {}

public:
	Box(Point* parent, Vector position, Vector scale, Angle rotation, Vector bounds)
		: Shape(parent, position, scale, rotation), bounds(bounds) {}
	Box(Point* parent, Vector bounds) : Box(parent, {}, {1.0f}, {}, bounds) {}
	Box(Vector bounds) : Box(nullptr, bounds) {}

	Vector getBounds() const { return bounds; }
	void setBounds(Vector v) {
		if (bounds == v)	return;

		bounds = v;

		setChanges();
	}

};

}

#endif
