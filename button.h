#ifndef SIMPLE_BUTTON_H
#define SIMPLE_BUTTON_H

#include "sprite.h"

namespace simpleGL {

class Button {
public:

	virtual void onPress(int mouseButton) {}
	virtual void onRelease(int mouseButton) {}
	virtual void onClick(int mouseButton) {}

	virtual void onDrag(int mouseButton) {}

	virtual void onEnter() {}
	virtual void onExit() {}

};

}

#endif
