#ifndef SIMPLE_UNSORTED_SPRITE_H
#define SIMPLE_UNSORTED_SPRITE_H

#include "color.hpp"
#include "vector.hpp"
#include "glfw.hpp"

namespace simpleGL {

class UnsortedSprite {
protected:
	bool enabled {true};
	unsigned id;

	GLuint textureId;

	GLuint vertexShader;
	GLuint geometryShader;
	GLuint fragmentShader;

	UnsortedSprite(GLuint textureId, Vector position, Vector bounds, float rotation,
								Color color, Vector texPosition, Vector texBounds);

	virtual ~UnsortedSprite();

public:

	unsigned getId() const { return id; }

	/*
	 * If enabled is false the sprite won't be used.
	 */
	bool isEnabled() const { return enabled; }
	virtual void setEnabled(bool b) { enabled = b; }

	GLuint getTextureId() const { return textureId; }
	virtual void setTextureId(GLuint tex) { textureId = tex; }

	/*
	 * Changes shader program for this sprite. When drawing this sprite opengl will use these shaders.
	 */
	void setVertexShader(GLuint sh) { vertexShader = sh; }
	void setGeometryShader(GLuint sh) { geometryShader = sh; }
	void setFragmentShader(GLuint sh) { fragmentShader = sh; }

	/*
	 * Changes attributes for this sprite.
	 */
	virtual void setPosition(Vector position) const;
	virtual void setBounds(Vector bounds) const;
	virtual void setRotation(float rotation) const;
	virtual void setColor(Color c) const;
	virtual void setTexPosition(Vector texPosition) const;
	virtual void setTexBounds(Vector texBounds) const;

	virtual void draw();

};

}

#endif
