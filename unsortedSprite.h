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

		updateTexture();
	}

	GLuint getVertexShader() const { return vertexShader; }
	virtual void setVertexShader(GLuint sh) { vertexShader = sh; }

	GLuint getFragmentShader() const { return fragmentShader; }
	virtual void setFragmentShader(GLuint sh) { fragmentShader = sh; }

	Color getColor() const { return color; }
	virtual void setColor(Color pcolor) { color = pcolor; }

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

	virtual bool inBounds(Vector pos) {
		if (!isEnabled())	return false;

		Vector dist = (getModelMatrix().inv() * pos).abs();
		Vector bounds = texture.getBounds()/2;

		return (dist.x < bounds.x) && (dist.y < bounds.y);
	}

	bool inBounds(UnsortedSprite* us);

	virtual void draw();

};

}

#endif
