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

	GLuint vertexShader;
	GLuint geometryShader;
	GLuint fragmentShader;

	GLenum stencilFunc {GL_ALWAYS};
	GLint stencilRef {0};
	GLuint stencilMask {0xFF};

	GLenum stencilFail {GL_KEEP};
	GLenum depthFail {GL_KEEP};
	GLenum depthPass {GL_KEEP};

	SimpleSprite(unsigned i, int pz, GLuint tex) : id(i), z(pz), texture(tex) {}

public:
	/*
	 * Creates sprite object and loads attribute data into data buffers.
	 *
	 * returns: sprite handle.
	 */
	static SimpleSprite* load(GLuint texture, SimpleVector position, int z, SimpleVector bounds, float rotation,
													SimpleColor color, SimpleVector texPosition, SimpleVector texBounds);
	class Loader {
	private:
		const SimpleTexture& texture;
		SimpleVector pposition;
		int pz{0};
		SimpleVector pbounds{1};
		float protation{0};
		SimpleColor pcolor{1};
		SimpleVector ptexPosition;
		SimpleVector ptexBounds;

	public:
		Loader(const SimpleTexture& texture) : texture(texture) {}

		Loader& position(SimpleVector sv) { pposition = sv; return *this; }
		Loader& z(int i) { pz = i; return *this; }
		Loader& bounds(SimpleVector sv) { pbounds = sv; return *this; }
		Loader& rotation(float f) { protation = f; return *this; }
		Loader& color(SimpleColor sc) { pcolor = sc; return *this; }
		Loader& texPosition(SimpleVector sv) { ptexPosition = sv; return *this; }
		Loader& texBounds(SimpleVector sv) { ptexBounds = sv; return *this; }

		SimpleSprite* load() {
			if (ptexBounds == SimpleVector())	ptexBounds = SimpleVector(texture.getWidth(), texture.getHeight());

			return SimpleSprite::load(texture.getTexture(), pposition, pz, pbounds, protation, pcolor, ptexPosition, ptexBounds);
		}
	};

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
	void setPosition(SimpleVector position) const;
	void setBounds(SimpleVector bounds) const;
	void setRotation(float rotation) const;
	void setColor(SimpleColor c) const;
	void setTexData(SimpleVector texPosition, SimpleVector texBounds) const;

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
