/* The base unit for spatial simulation
 * Basically, a 2D model matrix with some usefull stuff
*/

#ifndef SIMPLE_POINT_H
#define SIMPLE_POINT_H

#include <list>

#include "matrix.h"
#include "trigger.h"

namespace simpleGL {

class Point {
public:
	class BaseComponent {
	private:
		Point* owner;

	protected:
		Trigger ownerChanged;

		BaseComponent(Point* owner) : owner(owner) {
			owner->components.push_back(this);
		}

		virtual ~BaseComponent() {
			owner->components.remove(this);
		}

		Point* getOwnerBase() const { return owner; }

	public:
		void onOwnerChange() { ownerChanged.set(); }

		void unload() { delete this; }

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

	std::list<BaseComponent*> components;

protected:
	Trigger changed;

	void onChange() {
		changed.set();

		for (BaseComponent* c : components)
			c->onOwnerChange();
	}

	void onChangeDown() {
		onChange();
		for (Point* ch : children)
			ch->onChangeDown();
	}

	virtual void updateModel() {
		onChange();
		needUpdtModel = true;

		for (Point* obj : children)
			obj->updateModel();
	}

	virtual ~Point();

public:
	Point(Point* parent, Vector position, Vector scale, Angle rotation)
		: parent(parent), position(position), scale(scale), rotation(rotation) {
		if (parent) parent->children.push_back(this);
	}
	Point(Point* parent) : Point(parent, {}, {1.0f}, {}) {}
	Point() : Point(nullptr) {}

	bool isEnabled() const {
		return (parent == nullptr || parent->isEnabled()) && enabled;
	}
	void setEnabled(bool b) {
		if (b == enabled) return;

		enabled = b;

		onChangeDown();
	}

	Vector getPosition() const { return position; }
	Vector getRealPosition() { return getModelMatrix().getPosition(); }
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

	void translate(Vector v) {
		setPosition(getPosition() + v.rotate(rotation));
	}

	void unload() { delete this; }

};

template<typename OwnerClass>
class Component : public Point::BaseComponent
{
public:
	Component(OwnerClass* owner) : Point::BaseComponent(owner) {}

	OwnerClass* getOwner() const { return static_cast<OwnerClass*>(getOwnerBase()); }

};

}

#endif
