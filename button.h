#ifndef SIMPLE_BUTTON_H
#define SIMPLE_BUTTON_H

namespace simpleGL {

class Button {
public:

	virtual void onPress() {}
	virtual void onRealease() {}
	virtual void onClick() {}

	virtual void onEnter() {}
	virtual void onExit() {}

};

}

#endif
