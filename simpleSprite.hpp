#ifndef SIMPLE_SPRITE_H
#define SIMPLE_SPRITE_H

#include <boost/thread.hpp>
#include <boost/atomic.hpp>

#include "simpleColor.hpp"
#include "SimpleVector.hpp"
#include "simpleShader.hpp"
#include "simpleTexture.hpp"

class SimpleSprite {
protected:
	boost::atomic<bool> enabled {true};
	unsigned id;

	boost::atomic<int> z;

	boost::atomic<SimpleTexture*> texture;

	boost::atomic<GLuint> vertexShader;
	boost::atomic<GLuint> geometryShader;
	boost::atomic<GLuint> fragmentShader;

	boost::atomic<GLenum> stencilFunc {GL_ALWAYS};
	boost::atomic<GLint> stencilRef {0};
	boost::atomic<GLuint> stencilMask {0xFF};

	boost::atomic<GLenum> stencilFail {GL_KEEP};
	boost::atomic<GLenum> depthFail {GL_KEEP};
	boost::atomic<GLenum> depthPass {GL_KEEP};

	SimpleSprite(unsigned i, int pz, SimpleTexture* tex) : id(i), z(pz), texture(tex) {}

public:
	virtual ~SimpleSprite() =0;

	unsigned getId() const { return id; }

	/*
	 * If enabled is false the sprite won't draw.
	 */
	bool isEnabled() const {

		return enabled;
	}

	void setEnabled(bool b) {
		enabled = b;

	}

	int getZ() const {

		return z;
	}

	virtual void setZ(int pz) =0;

	SimpleTexture* getTexture() const {

		return texture;
	}

	virtual void setTexture(SimpleTexture* tex) =0;

	/*
	 * Changes shader program for this sprite. When drawing this sprite opengl will use these shaders.
	 */
	void setShader(SimpleShader ssh) {
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
		stencilFunc = func;
		stencilRef = ref;
		stencilMask = mask;
	}

	/*
	 * Sets data to use for glStencilOp calls.
	 */
	void setStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass) {
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
