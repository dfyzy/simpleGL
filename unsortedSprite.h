#ifndef SIMPLE_UNSORTED_SPRITE_H
#define SIMPLE_UNSORTED_SPRITE_H

#include <list>

#include "color.h"
#include "matrix.h"
#include "texture.h"

namespace simpleGL {

class UnsortedSprite {
private:
	bool enabled {true};
	unsigned id;

	UnsortedSprite* parent = nullptr;
	std::list<UnsortedSprite*> children;

	Texture texture;

	Vector position;
	Vector scale;
	Angle rotation;
	Color color;

	Matrix model;

	bool needUpdtModel;
	bool needUpdtVertices;
	bool needUpdtTexture;

	GLuint vertexShader;
	GLuint fragmentShader;

protected:
	void updateVertices() {
		needUpdtModel = true;
		needUpdtVertices = true;

		for (UnsortedSprite* us : children)
			us->updateVertices();
	}
	void updateTexture() { needUpdtTexture = true; }

	virtual void bindVertices();
	virtual void bindTexture();

	virtual ~UnsortedSprite();

public:
	UnsortedSprite(UnsortedSprite* parent, Texture texture, Vector position, Vector scale, Angle rotation, Color color);

	unsigned getId() const { return id; }

	/*
	 * If enabled is false the sprite won't be used.
	 */
	bool isEnabled() const { return enabled; }
	virtual void setEnabled(bool b) { enabled = b; }

	Texture getTexture() const { return texture; }
	virtual void setTexture(Texture tex) {
		texture = tex;

		updateTexture();
	}

	/*
	 * Changes shader program for this sprite. When drawing this sprite opengl will use these shaders.
	 */
	GLuint getVertexShader() const { return vertexShader; }
	virtual void setVertexShader(GLuint sh) { vertexShader = sh; }

	GLuint getFragmentShader() const { return fragmentShader; }
	virtual void setFragmentShader(GLuint sh) { fragmentShader = sh; }

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

	Color getColor() const { return color; }
	virtual void setColor(Color pcolor) { color = pcolor; }

	Matrix getModelMatrix() {
		if (needUpdtModel) {
			model = Matrix::translate(position) * Matrix::rotate(rotation) * Matrix::scale(scale);
			if (parent)	model = parent->getModelMatrix() * model;

			needUpdtModel = false;
		}

		return model;
	}

	void setParent(UnsortedSprite* us) {
		parent = us;
		if (parent)	parent->children.push_back(this);

		updateVertices();
	}

	bool isBindingVertices() const { return needUpdtVertices; }

	void bindData() {
		if (needUpdtTexture) {
			bindTexture();
			if (!needUpdtVertices)	bindVertices();

			needUpdtTexture = false;
		}

		if (needUpdtVertices) {
			bindVertices();

			needUpdtVertices = false;
		}
	}

	void translate(Vector v) {
		position += v.rotate(rotation);

		updateVertices();
	}

	bool inBounds(Vector pos) {
		Vector dist = (getModelMatrix().inv() * pos).abs();
		Vector bounds = texture.getBounds()/2;

		return (dist.x < bounds.x) && (dist.y < bounds.y);
	}

	bool inBounds(UnsortedSprite* us);

	virtual void draw();

	virtual void unload() { delete this; }

};

}

#endif
