#ifndef SIMPLE_SPRITE_H
#define SIMPLE_SPRITE_H

#include <memory>
#include "color.hpp"

class SimpleSprite {
private:
	bool enabled = true;

protected:
	unsigned id;

	SimpleSprite(unsigned i) : id(i) {}

public:
	unsigned getId() { return id; }

	void setEnabled(bool b);
	bool isEnabled();

	virtual void changePosition(float x, float y, float z) =0;
	virtual void changeBounds(float width, float height) =0;
	virtual void changeColor(Color c) =0;
	virtual void changeTexData(float texX, float texY, float texW, float texH) =0;

	virtual void unload() =0;
};

#endif
