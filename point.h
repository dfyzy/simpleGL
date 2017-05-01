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

protected:
	virtual ~Point() {
		parent->children.remove(this);
		for (Point* child : children)
			child->unload();
	}

public:
	Point(Point* parent, Vector position, Vector scale, Angle rotation)
		: parent(parent), position(position), scale(scale), rotation(rotation) {
			if (parent) parent->children.push_back(this);
		}

	virtual void updateVertices() {
		needUpdtModel = true;

		for (Point* obj : children)
			obj->updateVertices();
	}

	bool isEnabled() const {
		bool res = enabled;
		if (parent)	res &= parent->isEnabled();
		return res;
	}
	void setEnabled(bool b) { enabled = b; }

	Vector getPosition() const { return position; }
	Vector getRealPosition() const {
		Vector result = position;

		if (parent)	result = parent->model * result;

		return result;
	}
	virtual void setPosition(Vector pposition) {
		position = pposition;

		updateVertices();
	}

	Vector getScale() const { return scale; }
	virtual void setScale(Vector pscale) {
		scale = pscale;

		updateVertices();
	}

	Angle getRotation() const { return rotation; }
	Angle getRealRotation() const {
		Angle result = rotation;
		if (parent)	result += parent->getRealRotation();
		return result;
	}
	virtual void setRotation(Angle protation) {
		rotation = protation;

		updateVertices();
	}

	Matrix getModelMatrix() {
		if (needUpdtModel) {
			model = Matrix::translate(position) * Matrix::rotate(rotation) * Matrix::scale(scale);
			if (parent)	model = parent->getModelMatrix() * model;

			needUpdtModel = false;
		}

		return model;
	}

	Point* getParent() const { return parent; }
	void setParent(Point* obj) {
		parent = obj;
		if (parent)	parent->children.push_back(this);

		updateVertices();
	}

	std::list<Point*> getChildren() const { return children; }

	void translate(Vector v) {
		position += v.rotate(rotation);

		updateVertices();
	}

	virtual void unload() { delete this; }

};

}

#endif
