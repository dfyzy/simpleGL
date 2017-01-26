#ifndef SIMPLE_SPRITE_H
#define SIMPLE_SPRITE_H

#include "unsortedSprite.hpp"
#include "texture.hpp"

namespace simpleGL {

class Sprite : public UnsortedSprite {
protected:
	int z;

	~Sprite();

public:
	/*
	 * Creates sprite object and loads attribute data into data buffers.
	 *
	 * returns: sprite handle.
	 */
	Sprite(GLuint texture, Vector position, int z, Vector bounds, float rotation,
 								Color color, Vector texPosition, Vector texBounds);

	class Loader {
	private:
		const Texture& texture;
		Vector pposition;
		int pz{0};
		Vector pbounds{1};
		float protation{0};
		Color pcolor{1};
		Vector ptexPosition;
		Vector ptexBounds;

	public:
		Loader(const Texture& texture) : texture(texture) {}

		Loader& position(Vector sv) { pposition = sv; return *this; }
		Loader& z(int i) { pz = i; return *this; }
		Loader& bounds(Vector sv) { pbounds = sv; return *this; }
		Loader& rotation(float f) { protation = f; return *this; }
		Loader& color(Color sc) { pcolor = sc; return *this; }
		Loader& texPosition(Vector sv) { ptexPosition = sv; return *this; }
		Loader& texBounds(Vector sv) { ptexBounds = sv; return *this; }

		Sprite* load() {
			if (ptexBounds == Vector())	ptexBounds = Vector(texture.getWidth(), texture.getHeight());

			return new Sprite(texture.getTexture(), pposition, pz, pbounds, protation, pcolor, ptexPosition, ptexBounds);
		}
	};

	void setEnabled(bool b);

	int getZ() const { return z; }
	void setZ(int pz);

	void setTextureId(GLuint tex);

	void unload() { delete this; }

	struct Comparer {
		bool operator()(const Sprite* lhs, const Sprite* rhs) {
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

}

#endif
