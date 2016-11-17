#ifndef SIMPLE_SPRITE_H
#define SIMPLE_SPRITE_H

#include <boost/thread.hpp>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "simpleStructs.hpp"

class SimpleSprite {
protected:
	boost::mutex mutex;
	GLuint vertexShader;
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
	 * Changes shader programs for sprite. When drawing this sprite opengl will use these shaders.
	 */
	void changeVertexShader(GLuint sh) {
		boost::lock_guard<boost::mutex> lock(mutex);

		vertexShader = sh;
	}
	void changeFragmentShader(GLuint sh) {
		boost::lock_guard<boost::mutex> lock(mutex);

		fragmentShader = sh;
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
