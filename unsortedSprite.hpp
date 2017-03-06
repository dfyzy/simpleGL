#ifndef SIMPLE_UNSORTED_SPRITE_H
#define SIMPLE_UNSORTED_SPRITE_H

#include <list>

#include "color.hpp"
#include "texture.hpp"

namespace simpleGL {

class UnsortedSprite {
protected:
	bool enabled {true};
	unsigned id;

	Texture texture;

	Vector position;
	Vector scale;
	double rotation;

	double sinRot;
	double cosRot;

	Color color;

	GLuint vertexShader;
	GLuint fragmentShader;

	UnsortedSprite* parent = nullptr;
	std::list<UnsortedSprite*> children;

	void bindVertexData();
	void bindTextureData();

	virtual ~UnsortedSprite();

public:

	UnsortedSprite(Texture texture, Vector position, Vector scale, double rotation, Color color);

	unsigned getId() const { return id; }

	/*
	 * If enabled is false the sprite won't be used.
	 */
	bool isEnabled() const { return enabled; }
	virtual void setEnabled(bool b) { enabled = b; }

	Texture getTexture() const { return texture; }
	virtual void setTexture(Texture tex) {
		texture = tex;

		bindVertexData();
		bindTextureData();
	}

	/*
	 * Changes shader program for this sprite. When drawing this sprite opengl will use these shaders.
	 */
	virtual void setVertexShader(GLuint sh) { vertexShader = sh; }
	virtual void setFragmentShader(GLuint sh) { fragmentShader = sh; }

	Vector getPosition() { return position; }
	Vector getRealPosition() {
		Vector result = position;
		if (parent)	result = parent->getRealPosition() + position.rotate(parent->sinRot, parent->cosRot);
		return result;
	}
	virtual void setPosition(Vector pposition) {
		position = pposition;

		bindVertexData();
	}

	Vector getScale() { return scale; }
	virtual void setScale(Vector pscale) {
		scale = pscale;

		bindVertexData();
	}

	double getRotation() { return rotation; }
	double getRealRotation() {
		double result = rotation;
		if (parent)	result += parent->getRealRotation();
		return result;
	}
	virtual void setRotation(double protation) {
		rotation = protation;
		sinRot = std::sin(rotation);
		cosRot = std::cos(rotation);

		bindVertexData();
	}

	Color getColor() { return color; }
	virtual void setColor(Color pcolor) { color = pcolor; }

	void setParent(UnsortedSprite* us) {
		parent = us;
		parent->children.push_back(this);

		bindVertexData();
	}

	void translate(Vector v) {
		position += v.rotate(sinRot, cosRot);
	}

	virtual void draw();

};

}

#endif
