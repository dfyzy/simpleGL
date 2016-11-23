#ifndef SIMPLE_SPRITE_H
#define SIMPLE_SPRITE_H

#include <boost/thread.hpp>

#include "simpleShader.hpp"
#include "simpleStructs.hpp"

class SimpleSprite {
protected:
	boost::mutex mutex;
	GLuint vertexShader;
	GLuint geometryShader;
	GLuint fragmentShader;

	bool enabled = true;
	unsigned id;

	SimpleSprite() {}
	SimpleSprite(unsigned i) : id(i) {}

public:
	unsigned getId() { return id; }
	void setId(unsigned u) { id = u; }

	/*
	 * If enabled is false sprite won't draw.
	 */
	void setEnabled(bool b) {
		boost::lock_guard<boost::mutex> lock(mutex);

		enabled = b;
	}

	bool isEnabled() { return enabled; }

	/*
	 * Changes shader program for this sprite. When drawing this sprite opengl will use these shaders.
	 */
	void changeShader(SimpleShader ssh) {
		boost::lock_guard<boost::mutex> lock(mutex);

		if (ssh.getType() == GL_VERTEX_SHADER)
			vertexShader = ssh.getShader();
		else if (ssh.getType() == GL_GEOMETRY_SHADER)
			geometryShader = ssh.getShader();
		else if (ssh.getType() == GL_FRAGMENT_SHADER)
			fragmentShader = ssh.getShader();
	}

	/*
	 * Changes attributes for this sprite.
	 */
	virtual void changePosition(SimplePosition sp) =0;
	virtual void changeBounds(float width, float height) =0;
	virtual void changeColor(SimpleColor c) =0;
	virtual void changeTexData(float x, float y, float width, float height) =0;

	/*
	 * Deletes this sprite object. Unlocks data on buffers to use when creating new sprites.
	 */
	virtual void unload() =0;
};

#endif
