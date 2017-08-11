/* Parent of a box, anchored on one of 9 points relative to the box's bounds
*/

#ifndef SIMPLE_ANCHORED_BOX_H
#define SIMPLE_ANCHORED_BOX_H

#include "box.h"

namespace simpleGL {

enum Anchor {	TopRight,		Top,		TopLeft,
					Right,			Center,	Left,
					BottomRight,	Bottom,	BottomLeft};

inline Vector anchorToFactor(Anchor anchor) {
	return (Vector(anchor % 3, anchor / 3) - 1);
}

class AnchoredBox : public Point {
private:
	Box* box;
	Anchor anchor;

	void updateAnchor() {
		box->setPosition(anchorToFactor(anchor)*box->getBounds()*0.5f);
		setChanges();
	}

protected:
	~AnchoredBox() {}

public:
	AnchoredBox(Point* parent, Vector position, Vector scale, Angle rotation, Vector bounds, Anchor anchor)
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

	Anchor getAnchor() const { return anchor; }
	void setAnchor(Anchor a) {
		if (anchor == a)	return;

		anchor = a;
		updateAnchor();
	}

};

}

#endif
