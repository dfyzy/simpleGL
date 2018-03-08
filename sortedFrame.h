#ifndef SIMPLE_SORTED_FRAME_H
#define SIMPLE_SORTED_FRAME_H

#include <set>

#include "sortedSprite.h"
#include "framebuffer.h"

namespace simpleGL {

class SortedFrame : public Framebuffer {
private:
	std::set<SortedSprite*, SortedSprite::Comparer> sprites;

public:
	SortedFrame(unsigned width, unsigned height, GLenum format, bool stencil, GLenum filtering, Color base)
		: Framebuffer(width, height, format, stencil, filtering, base) {}

	~SortedFrame();

	void addSprite(SortedSprite* sprite) {
		if (sprite->getFrame() != this) return;

		sprites.insert(sprite);
	}
	void removeSprite(SortedSprite* sprite) {
		sprites.erase(sprite);
	}

	void draw(Matrix model) {
		bind(model);

		for (SortedSprite* sprite : sprites)
			sprite->draw();

		unbind();
	}

};

}

#endif
