#ifndef SIMPLE_UNSORTED_SPRITE_H
#define SIMPLE_UNSORTED_SPRITE_H

#include "point.h"
#include "color.h"
#include "texture.h"

namespace simpleGL {

class UnsortedSprite  : public Point {
private:
	unsigned id;

	Texture texture;

	Color color;

	bool needUpdtTexture {true};
	bool needUpdtVertices {true};

	GLuint vertexShader;
	GLuint fragmentShader;

	Vector bounds;
	bool customBounds {false};

	GLenum stencilFunc {GL_ALWAYS};
	GLenum stencilOp {GL_KEEP};
	GLint stencilRef {0};

protected:
	virtual void bindVertices();
	virtual void bindTexture();

	virtual ~UnsortedSprite();

public:
	UnsortedSprite(Point* parent, Texture texture, Vector position, Vector scale, Angle rotation, Color color);

	void updateVertices() {
		needUpdtVertices = true;
		Point::updateVertices();
	}
	void updateTexture() { needUpdtTexture = true; }

	unsigned getId() const { return id; }

	Texture getTexture() const { return texture; }
	virtual void setTexture(Texture tex) {
		texture = tex;
		if (!customBounds)
			bounds = texture.getBounds();

		updateTexture();
	}

	GLuint getVertexShader() const { return vertexShader; }
	virtual void setVertexShader(GLuint sh) { vertexShader = sh; }

	GLuint getFragmentShader() const { return fragmentShader; }
	virtual void setFragmentShader(GLuint sh) { fragmentShader = sh; }

	Color getColor() const { return color; }
	virtual void setColor(Color pcolor) { color = pcolor; }

	Vector getBounds() const { return bounds; }
	void setBounds(Vector vec) { bounds = vec; customBounds = true; }
	void setTextureBounds() { bounds = texture.getBounds(); customBounds = false; }

	bool isBindingVertices() const { return needUpdtVertices; }

	void mask() { stencilFunc = GL_ALWAYS; stencilOp = GL_REPLACE; stencilRef = id; }
	void setMask(UnsortedSprite* spr) { stencilFunc = GL_EQUAL; stencilRef = spr->id; }
	void unmask() { stencilFunc = GL_ALWAYS; stencilOp = GL_KEEP; }

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

	virtual bool inBounds(Vector pos) {
		if (!isEnabled())	return false;

		Vector dist = (getModelMatrix().inv() * pos).abs();
		Vector hBounds = bounds/2;

		return (dist.x < hBounds.x) && (dist.y < hBounds.y);
	}

	bool inBounds(UnsortedSprite* us);

	virtual void draw();

};

}

#endif
