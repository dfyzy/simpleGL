#ifndef SIMPLE_POINT_H
#define SIMPLE_POINT_H

#include <list>

#include "matrix.h"

namespace simpleGL {

class Point {
public:
	class Component {
	protected:
		virtual ~Component() {}

	public:
		Component(Point* p) { p->components.push_back(this); }

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

	Matrix getRealModelMatrix() {
		if (needUpdtModel) {
			model = Matrix::translate(position) * Matrix::rotate(rotation) * Matrix::scale(scale);
			if (parent)	model = parent->getRealModelMatrix() * model;

			needUpdtModel = false;
		}

		return model;
	}

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
	Point(Point* parent) : Point(parent, {}, {1}, {}) {}
	Point() : Point(nullptr) {}

	Point(const Point& other) =delete;

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
	Vector getRealPosition() const {
		Vector result = position;

		if (parent)	result = parent->getModelMatrix() * result;

		return result;
	}
	virtual void setPosition(Vector pposition) {
		if (position == pposition)	return;

		position = pposition;

		updateModel();
	}

	Vector getScale() const { return scale; }
	virtual void setScale(Vector pscale) {
		if (scale == pscale)	return;

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
		if (rotation == protation)	return;

		rotation = protation;

		updateModel();
	}

	virtual Matrix getModelMatrix() {
		return getRealModelMatrix();
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

	virtual void updateModel() {
		setChanges();
		needUpdtModel = true;

		for (Point* obj : children)
			obj->updateModel();
	}

	void translate(Vector v) {
		setPosition(getPosition() + v.rotate(rotation));
	}

	void unload() { delete this; }

};

}

#endif
