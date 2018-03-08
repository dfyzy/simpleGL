#ifndef SIMPLE_RECTANGLE_H
#define SIMPLE_RECTANGLE_H

#include "point.h"

namespace simpleGL {

class Rectangle : public Point {
public:
	class Anchor : public Point {
	private:
		Rectangle* const owner;
		Vector anchor;

	protected:
		~Anchor() {
			owner->anchors.remove(this);
		}

	public:
		Anchor(Rectangle* owner, Vector anchor) : Point(owner->center), owner(owner), anchor(anchor) {
			owner->anchors.push_back(this);
			setPositionToAnchor();
		}

		const Rectangle* getOwner() const { return owner; }

		Vector getAnchor() const { return anchor; }
		void setAnchor(Vector v) {
			if (anchor == v)	return;

			anchor = v;

			setPositionToAnchor();
		}

		void setPositionToAnchor() {
			setPosition(owner->getLocalPosition(anchor));
		}

	};

private:
	Vector bounds;

	Point* center;
	Vector pivot;

	std::list<Anchor*> anchors;

	Rectangle* mask {nullptr};
	std::list<Rectangle*> masked;

	Vector getLocalPosition(Vector relative) const { return relative*bounds*0.5f; }

	void updatePivot() {
		center->setPosition(-getLocalPosition(pivot));
	}

protected:
	void stopUsingMask() {
		if (mask)	mask->masked.remove(this);
	}

	~Rectangle();

public:
	Rectangle(Point* parent, Vector position, Vector scale, Angle rotation, Vector bounds, Vector pivot)
		: Point(parent, position, scale, rotation), bounds(bounds), center(new Point(this)), pivot(pivot) {
			updatePivot();
		}

	Vector getBounds() const { return bounds; }
	void setBounds(Vector v) {
		if (bounds == v)	return;

		bounds = v;

		updatePivot();

		for (Anchor* anchor : anchors) {
			anchor->setPositionToAnchor();
		}

		onChange();
	}

	Vector getPivot() const { return pivot; }
	void setPivot(Vector v) {
		if (pivot == v)	return;

		pivot = v;

		updatePivot();
		onChange();
	}

	Point* getCenter() const { return center; }

	bool inBounds(Vector v) {
		if (isEnabled() && (mask == nullptr || mask->inBounds(v))) {
			Vector localV = center->getModelMatrix().inv() * v;

			Vector dist = localV.abs();
			Vector hBounds = bounds/2.0f;

			return (dist.x < hBounds.x) && (dist.y < hBounds.y);//do some virtual func for texture lookups
		}

		return false;
	}

	Rectangle* getMask() const { return mask; }
	void setMask(Rectangle* rect) {
		if (rect == mask)	return;

		stopUsingMask();

		mask = rect;

		if (mask)	mask->masked.push_back(this);

		onChange();
	}

};

}

#endif
