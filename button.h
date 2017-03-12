#ifndef SIMPLE_BUTTON_H
#define SIMPLE_BUTTON_H

#include "sprite.h"

namespace simpleGL {

class Button {
public:

	virtual void onPress(int button) {}
	virtual void onRelease(int button) {}
	virtual void onClick(int button) {}

	virtual void onDrag(int button) {}

	virtual void onEnter() {}
	virtual void onExit() {}

};

void addButton(Sprite* sprite, Button* button);
void removeButton(Sprite* sprite);

}

#endif
