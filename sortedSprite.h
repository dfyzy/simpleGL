#ifndef SIMPLE_SORTED_SPRITE_H
#define SIMPLE_SORTED_SPRITE_H

#include "sprite.h"
#include "simpleGL.h"

namespace simpleGL {

class SortedFrame;

class SortedSprite : public Sprite {
public:
	struct Comparer {
		bool operator()(const SortedSprite* lhs, const SortedSprite* rhs) {
			if (lhs->z != rhs->z)
				return lhs->z > rhs->z;

			return lhs->getId() < rhs->getId();
		}
	};

private:
	SortedFrame* frame;
	int z;

protected:
	~SortedSprite();

public:
	struct Loader {
		Point* pparent {nullptr};
		Vector pposition;
		Vector pscale {1};
		Angle protation;
		Texture ptexture;
		Vector ppivot;
		Color pcolor {1};
		SortedFrame* pframe {getMainFrame()};
		int pz {0};

		Loader& parent(Point* p) { pparent = p; return *this; }
		Loader& position(Vector v) { pposition = v; return *this; }
		Loader& scale(Vector v) { pscale = v; return *this; }
		Loader& rotation(Angle a) { protation = a; return *this; }
		Loader& texture(Texture t) { ptexture = t; return *this; }
		Loader& pivot(Vector v) { ppivot = v; return *this; }
		Loader& color(Color c) { pcolor = c; return *this; }
		Loader& frame(SortedFrame* f) { pframe = f; return *this; }
		Loader& z(int i) { pz = i; return *this; }

		SortedSprite* load() { return new SortedSprite(pparent, pposition, pscale, protation, ptexture, ppivot, pcolor, pframe, pz); }
	};

	SortedSprite(Point* parent, Vector position, Vector scale, Angle rotation, Texture texture, Vector pivot, Color color,
						SortedFrame* frame, int z);

	int getZ() const { return z; }
	void setZ(int i);

	const SortedFrame* getFrame() const { return frame; }
	void setFrame(SortedFrame* sf);

};

}

#endif
