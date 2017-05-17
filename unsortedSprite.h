#ifndef SIMPLE_UNSORTED_SPRITE_H
#define SIMPLE_UNSORTED_SPRITE_H

#include "point.h"
#include "color.h"
#include "texture.h"

namespace simpleGL {

class UnsortedSprite  : public Point {
public:
	enum Anchor {	UR,	U,		UL,
						R,		C,		L,
						DR,	D,		DL};

private:
	unsigned id;

	Texture texture;

	Anchor anchor;
	Vector offset;

	Matrix offsetModel;

	Color color;

	bool needUpdtOffsetModel {true};
	bool needUpdtVertices {true};
	bool needUpdtTexture {true};
	bool needUpdtColor {true};

	GLuint vertexShader;
	GLuint fragmentShader;

	bool defaultFrag {true};

	GLenum stencilFunc {GL_ALWAYS};
	GLenum stencilOp {GL_KEEP};
	GLint stencilRef {0};

	void setOffset() {
		offset = (Vector(anchor % 3, anchor / 3) - 1) * texture.getBounds() * 0.5f;
	}

	void updateOffset() {
		setOffset();
		updateModel();
	}

protected:
	virtual void bindVertices();
	virtual void bindTexture();
	virtual void bindColor();

	~UnsortedSprite();

public:
	UnsortedSprite(Point* parent, Texture texture, Anchor anchor, Vector position, Vector scale, Angle rotation, Color color);

	unsigned getId() const { return id; }

	Texture getTexture() const { return texture; }
	virtual void setTexture(Texture tex);

	Anchor getAnchor() const { return anchor; }
	void setAnchor(Anchor panchor) {
		anchor = panchor;

		updateOffset();
	}

	Vector getCenter() const { return getPosition() + offset; }

	Matrix getModelMatrix() {
		if (needUpdtOffsetModel)
			offsetModel = Point::getModelMatrix() * Matrix::translate(offset);

		return offsetModel;
	}

	Color getColor() const { return color; }
	void setColor(Color pcolor) {
		color = pcolor;

		updateColor();
	}

	GLuint getVertexShader() const { return vertexShader; }
	virtual void setVertexShader(GLuint sh) { vertexShader = sh; }

	GLuint getFragmentShader() const { return fragmentShader; }
	virtual void setFragmentShader(GLuint sh) { fragmentShader = sh; defaultFrag = false; }

	void setDefaultFragmentShader();

	void updateModel() {
		needUpdtOffsetModel = true;
		needUpdtVertices = true;

		Point::updateModel();
	}
	void updateTexture() { needUpdtTexture = true; }
	void updateColor() { needUpdtColor = true; }

	void mask() { stencilFunc = GL_ALWAYS; stencilOp = GL_REPLACE; stencilRef = id; }
	void setMask(UnsortedSprite* spr) { stencilFunc = GL_EQUAL; stencilRef = spr->id; }
	void unmask() { stencilFunc = GL_ALWAYS; stencilOp = GL_KEEP; }

	void bindData() {
		if (needUpdtTexture) {
			needUpdtTexture = false;

			bindTexture();
			if (!needUpdtVertices)	bindVertices();
		}

		if (needUpdtVertices) {
			needUpdtVertices = false;

			bindVertices();
		}

		if (needUpdtColor) {
			needUpdtColor = false;

			bindColor();
		}
	}

	virtual void draw();

};

}

#endif
