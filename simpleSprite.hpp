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

	bool enabled {true};
	unsigned id;

	int z;

	SimpleTexture* texture;

	GLuint vertexShader;
	GLuint geometryShader;
	GLuint fragmentShader;

	GLenum stencilFunc {GL_ALWAYS};
	GLint stencilRef {0};
	GLuint stencilMask {0xFF};

	GLenum stencilFail {GL_KEEP};
	GLenum depthFail {GL_KEEP};
	GLenum depthPass {GL_KEEP};

public:

	SimpleSprite(unsigned i, int pz, SimpleTexture* tex) : id(i), z(pz), texture(tex) {}
	~SimpleSprite();

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

	void setZ(int pz);

	SimpleTexture* getTexture() const {
		boost::lock_guard<boost::mutex> lock(mutex);

		return texture;
	}

	void setTexture(SimpleTexture* tex);

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
	void setPosition(SimpleVector position);
	void setBounds(SimpleVector bounds);
	void setRotation(float rotation);
	void setColor(SimpleColor c);
	void setTexData(float x, float y, float width, float height);

	void draw() const;

	struct Comparer {
		bool operator()(const SimpleSprite* lhs, const SimpleSprite* rhs) {
			if (lhs == rhs)	return false;

			boost::lock_guard<boost::mutex> llock(lhs->mutex);
			boost::lock_guard<boost::mutex> rlock(rhs->mutex);

			if (lhs->z != rhs->z)
				return lhs->z > rhs->z;

			if (lhs->texture->getTexture() != rhs->texture->getTexture())
				return lhs->texture->getTexture() < rhs->texture->getTexture();

			if (lhs->vertexShader != rhs->vertexShader)
				return lhs->vertexShader < rhs->vertexShader;
			if (lhs->geometryShader != rhs->geometryShader)
				return lhs->geometryShader < rhs->geometryShader;
			if (lhs->fragmentShader != rhs->fragmentShader)
				return lhs->fragmentShader < rhs->fragmentShader;

			return lhs->id < rhs->id;
		}
	};
};

#endif
