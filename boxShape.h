/* Shape that represents rectangle
*/

#ifndef SIMPLE_BOX_SHAPE
#define SIMPLE_BOX_SHAPE

#include "shape.h"

namespace simpleGL {

class BaseBoxShape : public Shape {
protected:
	bool isInside(Vector v) const override {
		Vector dist = v.abs();
		Vector hBounds = getBounds()/2;

		return (dist.x < hBounds.x) && (dist.y < hBounds.y);
	}

	~BaseBoxShape() {}

public:
	BaseBoxShape(Point* parent, Vector position, Vector scale, Angle rotation)
				: Shape(parent, position, scale, rotation) {}

	virtual Vector getBounds() const =0;
};

class BoxShape : public BaseBoxShape {
private:
	Vector bounds;

protected:
	~BoxShape() {}

public:
	BoxShape(Point* parent, Vector position, Vector scale, Angle rotation)
				: BaseBoxShape(parent, position, scale, rotation) {}

	Vector getBounds() const override { return bounds; }
	void setBounds(Vector v) { bounds = v; }

};

}

#endif
