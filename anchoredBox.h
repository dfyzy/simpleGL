/* Parent of a box, anchored on one of 9 points relative to the box's bounds
*/

#ifndef SIMPLE_ANCHORED_BOX_H
#define SIMPLE_ANCHORED_BOX_H

#include "point.h"

namespace simpleGL {

enum class EAnchor {TopRight,		Top,		TopLeft,
							Right,			Center,	Left,
							BottomRight,	Bottom,	BottomLeft};

inline Vector anchorToFactor(EAnchor anchor) {
	return (Vector((int)anchor % 3, (int)anchor / 3) - 1);
}

class AnchoredBox : public Point {
private:
	class Box* box;
	EAnchor anchor;

	void updateAnchor();

protected:
	~AnchoredBox() {}

public:
	AnchoredBox(Point* parent, Vector position, Vector scale, Angle rotation, Vector bounds, EAnchor anchor);

	Box* getBoxShape() const { return box; }

	Vector getBounds() const;
	void setBounds(Vector bounds);

	EAnchor getAnchor() const { return anchor; }
	void setAnchor(EAnchor a) {
		if (anchor == a)	return;

		anchor = a;
		updateAnchor();
	}

};

}

#endif
