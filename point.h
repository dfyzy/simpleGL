/* The base unit for spatial simulation
 * Basically, a 2D model matrix with some usefull stuff
 *
 * Component class allows you to bind the lifetime of an object to a lifetime of a point
 *
 * Value of Change class object is set to true every time the point's properties change
*/

#ifndef SIMPLE_POINT_H
#define SIMPLE_POINT_H

#include <list>

#include "matrix.h"

namespace simpleGL {

class Point {
public:
	class Component {
	private:
		Point* point;

	protected:
		virtual ~Component() {
			point->components.remove(this);
		}

	public:
		Component(Point* point) : point(point) {
			point->components.push_back(this);
		}

		Point* getPoint() const { return point; }

		void unload() { delete this; }

	};

	class Change {
	private:
		bool change {false};

	public:
		bool get() const { return change; }
		void set() { change = true; }
		void reset() { change = false; }

	};

private:
	bool enabled {true};

	Point* parent = nullptr;
	std::list<Point*> children;

	Vector position;
	Vector scale;
	Angle rotation;

	Matrix model;
	bool needUpdtModel {true};

	std::list<Component*> components;

	std::list<Change> changes;

protected:
	void setChanges() {
		for (Change& ch : changes)
			ch.set();
	}

	void setChangesDown() {
		setChanges();
		for (Point* ch : children)
			ch->setChangesDown();
	}

	virtual void updateModel() {
		setChanges();
		needUpdtModel = true;

		for (Point* obj : children)
			obj->updateModel();
	}

	virtual ~Point() {
		for (Component* c : components)
			c->unload();

		parent->children.remove(this);
		for (Point* child : children)
			child->unload();
	}

public:
	Point(Point* parent, Vector position, Vector scale, Angle rotation)
		: parent(parent), position(position), scale(scale), rotation(rotation) {
		if (parent) parent->children.push_back(this);
	}
	Point(Point* parent) : Point(parent, {}, {1.0f}, {}) {}
	Point() : Point(nullptr) {}

	bool isEnabled() const {
		bool res = enabled;
		if (parent)	res &= parent->isEnabled();
		return res;
	}
	void setEnabled(bool b) {
		if (b == enabled) return;

		enabled = b;

		setChangesDown();
	}

	Vector getPosition() const { return position; }
	Vector getRealPosition() { return getModelMatrix()*Vector(); }
	void setPosition(Vector v) {
		if (position == v)	return;

		position = v;

		updateModel();
	}

	void addPosition(Vector v) {
		setPosition(getPosition() + v);
	}

	Vector getScale() const { return scale; }
	void setScale(Vector v) {
		if (scale == v)	return;

		scale = v;

		updateModel();
	}

	Angle getRotation() const { return rotation; }
	void setRotation(Angle a) {
		if (rotation == a)	return;

		rotation = a;

		updateModel();
	}

	void addRotation(Angle a) {
		setRotation(getRotation() + a);
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
		if (parent == obj)	return;

		if (parent)	parent->children.remove(this);

		parent = obj;

		if (parent)	parent->children.push_back(this);

		updateModel();
	}

	const std::list<Point*>& getChildren() const { return children; }

	int getChildrenCount() const { return children.size(); }

	Change* getChange() {
		changes.emplace_back();
		return &*(--changes.end());
	}

	void translate(Vector v) {
		setPosition(getPosition() + v.rotate(rotation));
	}

	void unload() { delete this; }

};

}

#endif
