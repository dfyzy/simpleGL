#ifndef SIMPLE_TRIGGER_H
#define SIMPLE_TRIGGER_H

namespace simpleGL {

class Trigger {
private:
	bool value;

public:
	Trigger() {}

	bool get() const { return value; }
	void set() { value = true; }
	void reset() { value = false; }

};

}

#endif
