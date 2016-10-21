#ifndef SIMPLE_SPRITE_H
#define SIMPLE_SPRITE_H

class SimpleSprite {
private:
	unsigned id;
	bool enabled = true;
	//mutex?

public:
	SimpleSprite(unsigned i) : id(i) {}

	unsigned getId() { return id; }

	void setEnabled(bool b) { enabled = b; }
	bool isEnabled() { return enabled; }

};

#endif
