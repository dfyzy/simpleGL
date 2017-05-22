#ifndef SIMPLE_UNSORTED_SPRITE_H
#define SIMPLE_UNSORTED_SPRITE_H

#include "shape.h"
#include "color.h"
#include "texture.h"

namespace simpleGL {

class UnsortedSprite  : public BaseBoxShape {
private:
	unsigned id;

	Texture texture;

	Color color;

	bool needUpdtVertices {true};
	bool needUpdtTexture {true};
	bool needUpdtColor {true};

	GLuint vertexShader;
	GLuint fragmentShader;

	bool defaultFrag {true};

	GLenum stencilFunc {GL_ALWAYS};
	GLenum stencilOp {GL_KEEP};
	GLint stencilRef {0};

protected:
	virtual void bindVertices();
	virtual void bindTexture();
	virtual void bindColor();

	~UnsortedSprite();

public:
	UnsortedSprite(Point* parent, Texture texture, Anchor anchor, Vector position, Vector scale, Angle rotation, Color color);

	unsigned getId() const { return id; }

	Texture getTexture() const { return texture; }
	virtual void setTexture(Texture tex) {
		if (texture == tex)	return;

		texture = tex;

		if (getAnchor() != C)	updateOffset();
		updateTexture();

		if (defaultFrag) setDefaultFragmentShader();
	}

	Vector getBounds() const { return texture.getBounds(); }

	Color getColor() const { return color; }
	void setColor(Color pcolor) {
		if (color == pcolor)	return;

		color = pcolor;

		updateColor();
	}

	GLuint getVertexShader() const { return vertexShader; }
	virtual void setVertexShader(GLuint sh) { vertexShader = sh; }

	GLuint getFragmentShader() const { return fragmentShader; }
	virtual void setFragmentShader(GLuint sh) { fragmentShader = sh; defaultFrag = false; }

	void setDefaultFragmentShader();

	void updateModel() {
		needUpdtVertices = true;

		BaseBoxShape::updateModel();
	}
	void updateTexture() {
		needUpdtTexture = true;
		needUpdtVertices = true;

		setChanges();
	}
	void updateColor() { needUpdtColor = true; }

	void mask() { stencilFunc = GL_ALWAYS; stencilOp = GL_REPLACE; stencilRef = id; }
	void setMask(UnsortedSprite* spr) { stencilFunc = GL_EQUAL; stencilRef = spr->id; }
	void unmask() { stencilFunc = GL_ALWAYS; stencilOp = GL_KEEP; }

	void bindData() {
		if (needUpdtTexture) {
			needUpdtTexture = false;
			bindTexture();
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
