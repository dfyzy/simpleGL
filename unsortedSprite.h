/* Base sprite class
 * Doesn't draw automatically
*/

#ifndef SIMPLE_UNSORTED_SPRITE_H
#define SIMPLE_UNSORTED_SPRITE_H

#include "boxShape.h"
#include "drawObject.h"
#include "shader.h"

namespace simpleGL {

enum Anchor {	TopRight,		Top,		TopLeft,
					Right,			Center,	Left,
					BottomRight,	Bottom,	BottomLeft};

inline Vector anchorToFactor(Anchor anchor) {
	return (Vector(anchor % 3, anchor / 3) - 1);
}

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
	Point* offset;
	Anchor anchor;

	DrawObject* drawObject;

	Texture texture;

	Color color;

	bool needUpdtVertices {true};
	bool needUpdtTexture {true};
	bool needUpdtColor {true};

	GLuint vertexShader;
	GLuint fragmentShader;

	bool defaultFrag {true};

	GLenum stencilFunc = GL_ALWAYS;
	GLenum stencilOp = GL_KEEP;
	GLint stencilRef {0};

	UnsortedSprite* stencil {nullptr};
	std::list<UnsortedSprite*> stenciled;

	void stopUsingStencil();

	void updateAnchor() { offset->setPosition(anchorToFactor(anchor)*texture.getBounds()*0.5f); }

protected:
	DrawObject* getDrawObject() const { return drawObject; }

	void updateModel() override {
		needUpdtVertices = true;

		BaseBoxShape::updateModel();
	}

	~UnsortedSprite() {
		stopUsingStencil();
		for (UnsortedSprite* st : stenciled)
			st->setStencil(nullptr);

		drawObject->unload();
	}

public:
	UnsortedSprite(Point* parent, Anchor anchor, Texture texture, Vector position, Vector scale, Angle rotation, Color color)
								: BaseBoxShape(parent, position, scale, rotation), anchor(anchor), texture(texture), color(color) {
		offset = new Point(this);
		updateAnchor();

		drawObject = new DrawObject();

		vertexShader = getDefaultVertexShader();
		setDefaultFragmentShader();
	}

	UnsortedSprite(Texture t, Data d)
		: UnsortedSprite(d.pparent, d.panchor, t, d.pposition, d.pscale, d.protation, d.pcolor) {}
	UnsortedSprite(Texture t) : UnsortedSprite(t, {}) {}

	unsigned getId() const { return drawObject->getId(); }

	Anchor getAnchor() const { return anchor; }
	void setAnchor(Anchor a) { anchor = a; updateAnchor(); }

	Matrix getOffsetedModelMatrix() const { return offset->getModelMatrix(); }

	Texture getTexture() const { return texture; }
	virtual void setTexture(Texture t) {
		if (texture == t)	return;

		texture = t;

		needUpdtTexture = true;
		needUpdtVertices = true;

		updateAnchor();

		setChanges();

		if (defaultFrag) setDefaultFragmentShader();
	}

	Vector getBounds() const override { return texture.getBounds(); }

	Color getColor() const { return color; }
	void setColor(Color c) {
		if (color == c)	return;

		color = c;

		needUpdtColor = true;
		setChanges();
	}

	GLuint getVertexShader() const { return vertexShader; }
	void setVertexShader(GLuint sh) { vertexShader = sh; }

	GLuint getFragmentShader() const { return fragmentShader; }
	virtual void setFragmentShader(GLuint sh) { fragmentShader = sh; defaultFrag = false; }

	void setDefaultFragmentShader() {
		fragmentShader = getDefaultFragmentShader(texture.getImage() == nullptr);

		defaultFrag = true;
	}

	void setStencil(UnsortedSprite* spr);
	void setCustomStencil(GLenum func, GLenum ref, GLenum op) {
		stencilFunc = func;
		stencilOp = op;
		stencilRef = ref;
	}

	virtual void draw();

};

}

#endif
