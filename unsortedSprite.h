#ifndef SIMPLE_UNSORTED_SPRITE_H
#define SIMPLE_UNSORTED_SPRITE_H

#include "shape.h"
#include "drawObject.h"
#include "shader.h"

namespace simpleGL {

struct Data {
	Point* pparent {nullptr};
	Anchor panchor{Center};
	Vector pposition;
	Vector pscale{1};
	Angle protation;
	Color pcolor{1};

	Data() {}

	Data& parent(Point* p) { pparent = p; return *this; }
	Data& anchor(Anchor a) { panchor = a; return *this; }
	Data& position(Vector sv) { pposition = sv; return *this; }
	Data& scale(Vector sv) { pscale = sv; return *this; }
	Data& rotation(Angle f) { protation = f; return *this; }
	Data& color(Color sc) { pcolor = sc; return *this; }
};

class UnsortedSprite  : public BaseBoxShape {
private:
	DrawObject* drawObject;

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

	GLint getStencil();

protected:
	DrawObject* getDrawObject() const { return drawObject; }

	virtual void bindVertices();
	virtual void bindTexture();
	virtual void bindColor();

	~UnsortedSprite();

public:
	UnsortedSprite(Point* parent, Texture texture, Anchor anchor, Vector position, Vector scale, Angle rotation, Color color);

	UnsortedSprite(Texture t, Data d) : UnsortedSprite(d.pparent, t, d.panchor, d.pposition, d.pscale, d.protation, d.pcolor) {}
	UnsortedSprite(Texture t) : UnsortedSprite(t, {}) {}

	unsigned getId() const { return drawObject->getId(); }

	Texture getTexture() const { return texture; }
	virtual void setTexture(Texture tex) {
		if (texture == tex)	return;

		texture = tex;

		if (getAnchor() != Center)	updateOffset();
		updateTexture();

		if (defaultFrag) setDefaultFragmentShader();
	}

	void setTexturePosition(Vector v) {
		if (texture.getPosition() == v)	return;

		texture.setPosition(v);

		updateTexture();
	}

	void setTextureBounds(Vector v) {
		if (texture.getBounds() == v)	return;

		texture.setBounds(v);

		if (getAnchor() != Center)	updateOffset();
		updateTexture();
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

	void setDefaultFragmentShader() {
		fragmentShader = getDefaultFragmentShader(texture.getImage() == nullptr);

		defaultFrag = true;
	}

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

	void setStencil(UnsortedSprite* spr) {
		setMask(spr);

		if (spr) {
			stencilFunc = GL_EQUAL;
			stencilRef = spr->getStencil();
		} else {
			stencilFunc = GL_ALWAYS;
		}
	}

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
