#ifndef SIMPLE_SPRITE_H
#define SIMPLE_SPRITE_H

#include "simpleColor.hpp"
#include "SimpleVector.hpp"
#include "simpleTexture.hpp"

class SimpleSprite {
private:
	bool enabled {true};
	unsigned id;

	int z;

	GLuint texture;
	SimpleVector textureBounds;

	GLuint vertexShader;
	GLuint geometryShader;
	GLuint fragmentShader;

	GLenum stencilFunc {GL_ALWAYS};
	GLint stencilRef {0};
	GLuint stencilMask {0xFF};

	GLenum stencilFail {GL_KEEP};
	GLenum depthFail {GL_KEEP};
	GLenum depthPass {GL_KEEP};

	SimpleSprite(unsigned i, int pz, GLuint tex, SimpleVector texBounds) : id(i), z(pz), texture(tex), textureBounds(texBounds) {}

public:
	/*
	 * Creates sprite object and loads attribute data into data buffers.
	 *
	 * returns: sprite handle.
	 */
	static SimpleSprite* load(SimpleTexture* tex, SimpleVector position, int z, SimpleVector bounds, float rotation, SimpleColor c,
													SimpleVector texPosition, SimpleVector texBounds);

	inline static SimpleSprite* load(SimpleTexture* tex, SimpleVector position, int z, SimpleVector bounds, float rotation, SimpleColor c) {
		return load(tex, position, z, bounds, rotation, c, SimpleVector(), SimpleVector(tex->getWidth(), tex->getHeight()));
	}
	inline static SimpleSprite* load(SimpleTexture* tex, SimpleVector position, int z, SimpleVector bounds, SimpleColor c) {
		return load(tex, position, z, bounds, 0, c);
	}
	inline static SimpleSprite* load(SimpleTexture* tex, SimpleVector position, int z, SimpleColor c) {
		return load(tex, position, z, 1, c);
	}

	~SimpleSprite() { unload(); }

	unsigned getId() const { return id; }

	/*
	 * If enabled is false the sprite won't be used.
	 */
	bool isEnabled() const { return enabled; }

	void setEnabled(bool b);

	int getZ() const { return z; }

	void setZ(int pz);

	GLuint getTexture() const { return texture; }

	void setTexture(GLuint tex);

	/*
	 * Changes shader program for this sprite. When drawing this sprite opengl will use these shaders.
	 */
	void setVertexShader(GLuint sh) { vertexShader = sh; }
	void setGeometryShader(GLuint sh) { geometryShader = sh; }
	void setFragmentShader(GLuint sh) { fragmentShader = sh; }

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
	void setPosition(SimpleVector position);
	void setBounds(SimpleVector bounds);
	void setRotation(float rotation);
	void setColor(SimpleColor c);
	void setTexData(SimpleVector texPosition, SimpleVector texBounds);

	void draw() const;

	void unload();

	struct Comparer {
		bool operator()(const SimpleSprite* lhs, const SimpleSprite* rhs) {
			if (lhs == rhs)	return false;

			if (lhs->z != rhs->z)
				return lhs->z > rhs->z;

			if (lhs->texture != rhs->texture)
				return lhs->texture < rhs->texture;

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
