#ifndef SIMPLE_POINT_H
#define SIMPLE_POINT_H

#include <list>

#include "matrix.h"

namespace simpleGL {

class Point {
private:
	bool enabled {true};

	Point* parent = nullptr;
	std::list<Point*> children;

	Vector position;
	Vector scale;
	Angle rotation;

	Matrix model;

	bool needUpdtModel {true};

	bool changed {false};

	Matrix getRealModelMatrix() {
		if (needUpdtModel) {
			model = Matrix::translate(position) * Matrix::rotate(rotation) * Matrix::scale(scale);
			if (parent)	model = parent->getRealModelMatrix() * model;

			needUpdtModel = false;
		}

		return model;
	}

	void setChangeTrue() {
		changed = true;
		for (Point* child : children)
			child->setChangeTrue();
	}

protected:
	virtual ~Point();
	
public:
	Point(Point* parent, Vector position, Vector scale, Angle rotation);
	Point(Point* parent) : Point(parent, {}, {1}, {}) {}
	Point() : Point(nullptr) {}

	Point(const Point& other) =delete;

	bool isEnabled() const {
		bool res = enabled;
		if (parent)	res &= parent->isEnabled();
		return res;
	}
	void setEnabled(bool b) { enabled = b; }

	Vector getPosition() const { return position; }
	Vector getRealPosition() const {
		Vector result = position;

		if (parent)	result = parent->getModelMatrix() * result;

		return result;
	}
	virtual void setPosition(Vector pposition) {
		if (position != pposition)
			setChangeTrue();

		position = pposition;

		updateModel();
	}

	Vector getScale() const { return scale; }
	virtual void setScale(Vector pscale) {
		if (scale != pscale)
			setChangeTrue();

		scale = pscale;

		updateModel();
	}

	Angle getRotation() const { return rotation; }
	Angle getRealRotation() const {
		Angle result = rotation;
		if (parent)	result += parent->getRealRotation();
		return result;
	}
	virtual void setRotation(Angle protation) {
		if (rotation != protation)
			setChangeTrue();

		rotation = protation;

		updateModel();
	}

	virtual Matrix getModelMatrix() {
		return getRealModelMatrix();
	}

	Point* getParent() const { return parent; }
	void setParent(Point* obj) {
		if (parent != obj)
			setChangeTrue();

		if (parent)	parent->children.remove(this);

		parent = obj;

		if (parent)	parent->children.push_back(this);

		updateModel();
	}

	const std::list<Point*>& getChildren() const { return children; }

	int getChildrenCount() const { return children.size(); }

	bool getChanged() const { return changed; }

	virtual void updateModel() {
		needUpdtModel = true;

		for (Point* obj : children)
			obj->updateModel();
	}

	void translate(Vector v) {
		setPosition(getPosition() + v.rotate(rotation));

		updateModel();
	}

	virtual void update() {
		changed = false;
	}

	void unload() { delete this; }

};

}

#endif
