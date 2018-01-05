#include "sortedSprite.h"
#include "sortedFrame.h"
#include "camera.h"

namespace simpleGL {

SortedSprite::SortedSprite(Point* parent, Vector position, Vector scale, Angle rotation, Texture texture, Vector pivot, Color color,
		SortedFrame* frame, int z)
			: Sprite(parent, position, scale, rotation, texture, pivot, color), frame(frame), z(z) {
				if (frame)	frame->addSprite(this);
}

SortedSprite::~SortedSprite() {
	frame->removeSprite(this);
}

void SortedSprite::setZ(int i) {
	if (frame)	frame->removeSprite(this);
	z = i;
	if (frame)	frame->addSprite(this);
}

void SortedSprite::setFrame(SortedFrame* sf) {
	if (frame)	frame->removeSprite(this);
	frame = sf;
	if (frame)	frame->addSprite(this);
}

}
