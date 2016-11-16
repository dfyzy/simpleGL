#ifndef SIMPLE_SPRITE_H
#define SIMPLE_SPRITE_H

#include <memory>
#include "simpleStructs.hpp"

class SimpleSprite {
private:
	bool enabled = true;

protected:
	unsigned id;

	SimpleSprite() {}

public:
	unsigned getId() { return id; }
	void setId(unsigned u) { id = u; }

	void setEnabled(bool b);
	bool isEnabled();

	virtual void changePosition(SimplePosition sp) =0;
	virtual void changeBounds(float width, float height) =0;
	virtual void changeColor(SimpleColor c) =0;
	virtual void changeTexData(float x, float y, float width, float height) =0;

	virtual void unload() =0;
};

#endif
