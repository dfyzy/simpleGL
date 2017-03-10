#ifndef SIMPLE_UNSORTED_SPRITE_H
#define SIMPLE_UNSORTED_SPRITE_H

#include <list>

#include "color.h"
#include "texture.h"

namespace simpleGL {

class UnsortedSprite {
protected:
	UnsortedSprite* parent = nullptr;
	std::list<UnsortedSprite*> children;

	bool enabled {true};
	unsigned id;

	Texture texture;

	Vector position;
	Vector scale;
	double rotation, sinRotation, cosRotation;
	Color color;

	GLuint vertexShader;
	GLuint fragmentShader;

	virtual void bindVertexData();
	virtual void bindTextureData();

	void bindChildData() {
		for (UnsortedSprite* child : children)
			child->bindVertexData();
	}

	virtual ~UnsortedSprite();

public:

	UnsortedSprite(UnsortedSprite* parent, Texture texture, Vector position, Vector scale, double rotation, Color color);

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

		UnsortedSprite* nextParent = parent;
		while (nextParent) {
			result = nextParent->position + result.rotate(nextParent->sinRotation, nextParent->cosRotation);
			nextParent = nextParent->parent;
		}

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
		sinRotation = std::sin(rotation);
		cosRotation = std::cos(rotation);

		bindVertexData();
	}

	Color getColor() { return color; }
	virtual void setColor(Color pcolor) { color = pcolor; }

	void setParent(UnsortedSprite* us) {
		parent = us;
		if (parent)	parent->children.push_back(this);

		bindVertexData();
	}

	void translate(Vector v) {
		position += v.rotate(sinRotation, cosRotation);
	}

	virtual void draw();

};

}

#endif
