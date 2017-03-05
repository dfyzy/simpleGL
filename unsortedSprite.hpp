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
	float rotation;

	float sinRot;
	float cosRot;

	Color color;

	GLuint vertexShader;
	GLuint fragmentShader;

	UnsortedSprite* parent = nullptr;
	std::list<UnsortedSprite*> children;

	void bindVertexData();
	void bindTextureData();

	virtual ~UnsortedSprite();

public:

	UnsortedSprite(Texture texture, Vector position, Vector scale, float rotation, Color color);

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

	/*
	 * Changes attributes for this sprite.
	 */
	virtual void setPosition(Vector pposition) {
		position = pposition;

		bindVertexData();
	}

	virtual void setScale(Vector pscale) {
		scale = pscale;

		bindVertexData();
	}

	virtual void setRotation(float protation) {
		rotation = protation;
		sinRot = std::sin(rotation);
		cosRot = std::cos(rotation);

		bindVertexData();
	}

	virtual void setColor(Color pcolor) {
		color = pcolor;
	}

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
