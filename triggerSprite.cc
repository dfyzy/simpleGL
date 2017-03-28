#include "triggerSprite.h"

namespace {
	std::list<simpleGL::TriggerSprite*> triggers;
}

namespace simpleGL {

	void TriggerSprite::checkTrigger() {
		for (TriggerSprite* ts : triggers) {
			if (ts == this)	continue;

			if (inBounds(ts)) {
				onTriggerEnter(ts);
				ts->onTriggerEnter(this);
			}
		}
	}

	TriggerSprite::TriggerSprite(Data d) : Sprite(d) {
		triggers.push_back(this);

		checkTrigger();
	}

	void TriggerSprite::updateVertices() {
		Sprite::updateVertices();

		checkTrigger();
	}

}
