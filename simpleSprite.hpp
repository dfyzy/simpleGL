#ifndef SIMPLE_SPRITE_H
#define SIMPLE_SPRITE_H

#include <boost/thread.hpp>

#include "simpleStructs.hpp"
#include "simpleShader.hpp"
#include "simpleTexture.hpp"

class SimpleSprite {
protected:
	boost::mutex mutex;

	bool enabled = true;
	unsigned id;

	SimpleTexture* texture;

	GLuint vertexShader;
	GLuint geometryShader;
	GLuint fragmentShader;

	GLenum stencilFunc = GL_ALWAYS;
	GLint stencilRef = 0;
	GLuint stencilMask = 0xFF;

	GLenum stencilFail = GL_KEEP;
	GLenum depthFail = GL_KEEP;
	GLenum depthPass = GL_KEEP;

	SimpleSprite(unsigned i, SimpleTexture* tex) : id(i), texture(tex) {}

	virtual void resort() =0;

public:
	virtual ~SimpleSprite() =0;

	unsigned getId() const { return id; }
	void setId(unsigned u) { id = u; }

	/*
	 * If enabled is false the sprite won't draw.
	 */
	bool isEnabled() const { return enabled; }

	void setEnabled(bool b) {
		boost::lock_guard<boost::mutex> lock(mutex);

		enabled = b;
	}

	SimpleTexture* getTexture() const { return texture; }

	void changeTexture(SimpleTexture* tex) {
		boost::lock_guard<boost::mutex> lock(mutex);

		texture = tex;
		resort();
	}

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
	 * Sets data to use for glStencilFunc calls.
	 */
	void setStencilFunc(GLenum func, GLint ref, GLuint mask) {
		boost::lock_guard<boost::mutex> lock(mutex);

		stencilFunc = func;
		stencilRef = ref;
		stencilMask = mask;
	}

	/*
	 * Sets data to use for glStencilOp calls.
	 */
	void setStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass) {
		boost::lock_guard<boost::mutex> lock(mutex);

		stencilFail = sfail;
		depthFail = dpfail;
		depthPass = dppass;
	}

	/*
	 * Changes attributes for this sprite.
	 */
	virtual void changePosition(SimplePosition sp) =0;
	virtual void changeBounds(float width, float height) =0;
	virtual void changeColor(SimpleColor c) =0;
	virtual void changeTexData(float x, float y, float width, float height) =0;
	
};
inline SimpleSprite::~SimpleSprite() {}

#endif
