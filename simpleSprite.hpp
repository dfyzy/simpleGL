#ifndef SIMPLE_SPRITE_H
#define SIMPLE_SPRITE_H

#include <boost/thread.hpp>

#include "simpleColor.hpp"
#include "SimpleVector.hpp"
#include "simpleShader.hpp"
#include "simpleTexture.hpp"

class SimpleSprite {
protected:
	mutable boost::mutex mutex;

	bool enabled = true;
	unsigned id;

	int z;

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

	SimpleSprite(unsigned i, int pz, SimpleTexture* tex) : id(i), z(pz), texture(tex) {}

public:
	virtual ~SimpleSprite() =0;

	unsigned getId() const { return id; }

	/*
	 * If enabled is false the sprite won't draw.
	 */
	bool isEnabled() const {
		boost::lock_guard<boost::mutex> lock(mutex);

		return enabled;
	 }

	void setEnabled(bool b) {
		boost::lock_guard<boost::mutex> lock(mutex);

		enabled = b;
	}

	int getZ() const {
		boost::lock_guard<boost::mutex> lock(mutex);

		return z;
	}

	virtual void setZ(int pz) =0;

	SimpleTexture* getTexture() const {
		boost::lock_guard<boost::mutex> lock(mutex);

		return texture;
	}

	virtual void setTexture(SimpleTexture* tex) =0;

	/*
	 * Changes shader program for this sprite. When drawing this sprite opengl will use these shaders.
	 */
	void setShader(SimpleShader ssh) {
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
	virtual void setPosition(SimpleVector position) =0;
	virtual void setBounds(SimpleVector bounds) =0;
	virtual void setRotation(float rotation) =0;
	virtual void setColor(SimpleColor c) =0;
	virtual void setTexData(float x, float y, float width, float height) =0;

};
inline SimpleSprite::~SimpleSprite() {}

#endif
