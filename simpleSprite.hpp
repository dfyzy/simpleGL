#ifndef SIMPLE_SPRITE_H
#define SIMPLE_SPRITE_H

#include "simplerSprite.hpp"
#include "simpleTexture.hpp"

class SimpleSprite : public SimplerSprite {
protected:
	int z;

	~SimpleSprite();

public:
	/*
	 * Creates sprite object and loads attribute data into data buffers.
	 *
	 * returns: sprite handle.
	 */
	SimpleSprite(GLuint texture, SimpleVector position, int z, SimpleVector bounds, float rotation,
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

			return new SimpleSprite(texture.getTexture(), pposition, pz, pbounds, protation, pcolor, ptexPosition, ptexBounds);
		}
	};

	void setEnabled(bool b);

	int getZ() const { return z; }
	void setZ(int pz);

	void setTextureId(GLuint tex);

	void unload() { delete this; }

	struct Comparer {
		bool operator()(const SimpleSprite* lhs, const SimpleSprite* rhs) {
			if (lhs == rhs)	return false;

			if (lhs->z != rhs->z)
				return lhs->z > rhs->z;

			if (lhs->textureId != rhs->textureId)
				return lhs->textureId < rhs->textureId;

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
