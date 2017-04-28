#ifndef SIMPLE_TRIGGER_SPRITE_H
#define SIMPLE_TRIGGER_SPRITE_H

#include "sprite.h"

namespace simpleGL {

class TriggerSprite : public Sprite {
private:
	void checkTrigger();
	
protected:
	void updateVertices();

public:
	TriggerSprite(Data d);

	virtual void onTriggerEnter(TriggerSprite* other) =0;
	virtual void onTriggerExit(TriggerSprite* other) =0;

};

}

#endif
