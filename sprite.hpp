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
	Sprite(Texture texture, Vector position, int z, Vector scale, double rotation, Color color);

	class Loader {
	private:
		Texture texture;
		Vector pposition;
		int pz{0};
		Vector pscale{1};
		double protation{0};
		Color pcolor{1};

	public:
		Loader(Texture texture) : texture(texture) {}

		Loader& position(Vector sv) { pposition = sv; return *this; }
		Loader& z(int i) { pz = i; return *this; }
		Loader& scale(Vector sv) { pscale = sv; return *this; }
		Loader& rotation(double f) { protation = f; return *this; }
		Loader& color(Color sc) { pcolor = sc; return *this; }

		Sprite* load() {
			return new Sprite(texture, pposition, pz, pscale, protation, pcolor);
		}
	};

	void setEnabled(bool b);

	int getZ() const { return z; }
	void setZ(int pz);

	void setTexture(Texture tex);

	void unload() { delete this; }

	struct Comparer {
		bool operator()(const Sprite* lhs, const Sprite* rhs) {
			if (lhs == rhs)	return false;

			if (lhs->z != rhs->z)
				return lhs->z > rhs->z;

			if (lhs->texture.getId() != rhs->texture.getId())
				return lhs->texture.getId() < rhs->texture.getId();

			if (lhs->vertexShader != rhs->vertexShader)
				return lhs->vertexShader < rhs->vertexShader;
			if (lhs->fragmentShader != rhs->fragmentShader)
				return lhs->fragmentShader < rhs->fragmentShader;

			return lhs->id < rhs->id;
		}
	};
};

}

#endif
