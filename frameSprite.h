#ifndef SIMPLE_FRAME_SPRITE_H
#define SIMPLE_FRAME_SPRITE_H

#include "sortedSprite.h"
#include "sortedFrame.h"

namespace simpleGL {

class FrameSprite : public SortedSprite {
private:
	SortedFrame imageFrame;

public:
	FrameSprite(Point* parent, Vector position, Vector scale, Angle rotation, Texture texture, Vector pivot, Color color,
					SortedFrame* frame, int z, unsigned width, unsigned height, GLenum format, bool stencil, GLenum filtering)
						: SortedSprite(parent, position, scale, rotation, texture, pivot, color, frame, z),
							imageFrame(width, height, format, stencil, filtering, color) {
								setTexture({imageFrame.getImage()});
							}

	void draw() override {
		imageFrame.draw(getCenter()->getModelMatrix());

		Sprite::draw();
	}

};

}


#endif
