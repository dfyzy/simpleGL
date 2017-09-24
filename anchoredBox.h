/* Parent of a box, anchored on one of 9 points relative to the box's bounds
*/

#ifndef SIMPLE_ANCHORED_BOX_H
#define SIMPLE_ANCHORED_BOX_H

#include "box.h"

namespace simpleGL {

enum class EAnchor {TopRight,		Top,		TopLeft,
							Right,			Center,	Left,
							BottomRight,	Bottom,	BottomLeft};

inline Vector anchorToFactor(EAnchor anchor) {
	return (Vector((int)anchor % 3, (int)anchor / 3) - 1);
}

class AnchoredBox : public Point {
private:
	Box* box;
	EAnchor anchor;

	void updateAnchor() {
		box->setPosition(anchorToFactor(anchor)*box->getBounds()*0.5f);
		setChanges();
	}

protected:
	~AnchoredBox() {}

public:
	AnchoredBox(Point* parent, Vector position, Vector scale, Angle rotation, Vector bounds, EAnchor anchor)
		: Point(parent, position, scale, rotation), box(new Box(this, bounds)), anchor(anchor) {
		updateAnchor();
	}

	Shape* getBoxShape() const { return box; }

	Vector getBounds() const { return box->getBounds(); }
	void setBounds(Vector bounds) {
		if (bounds == box->getBounds())	return;

		box->setBounds(bounds);
		updateAnchor();
	}

	EAnchor getAnchor() const { return anchor; }
	void setAnchor(EAnchor a) {
		if (anchor == a)	return;

		anchor = a;
		updateAnchor();
	}

};

}

#endif
