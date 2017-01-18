#ifndef SIMPLER_SPRITE_H
#define SIMPLER_SPRITE_H

#include "simpleColor.hpp"
#include "simpleVector.hpp"
#include "glfw.hpp"

class SimplerSprite {
protected:
	bool enabled {true};
	unsigned id;

	GLuint texture;

	GLuint vertexShader;
	GLuint geometryShader;
	GLuint fragmentShader;

	SimplerSprite(GLuint texture, SimpleVector position, SimpleVector bounds, float rotation,
								SimpleColor color, SimpleVector texPosition, SimpleVector texBounds);

public:

	~SimplerSprite() { unload(); }

	unsigned getId() const { return id; }

	/*
	 * If enabled is false the sprite won't be used.
	 */
	bool isEnabled() const { return enabled; }
	virtual void setEnabled(bool b) { enabled = b; }

	GLuint getTexture() const { return texture; }
	virtual void setTexture(GLuint tex) { texture = tex; }

	/*
	 * Changes shader program for this sprite. When drawing this sprite opengl will use these shaders.
	 */
	void setVertexShader(GLuint sh) { vertexShader = sh; }
	void setGeometryShader(GLuint sh) { geometryShader = sh; }
	void setFragmentShader(GLuint sh) { fragmentShader = sh; }

	/*
	 * Changes attributes for this sprite.
	 */
	virtual void setPosition(SimpleVector position) const;
	virtual void setBounds(SimpleVector bounds) const;
	virtual void setRotation(float rotation) const;
	virtual void setColor(SimpleColor c) const;
	virtual void setTexData(SimpleVector texPosition, SimpleVector texBounds) const;

	void draw() const;

	virtual void unload();

};

#endif
