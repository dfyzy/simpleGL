#ifndef SIMPLE_BOOL_VECTOR_H
#define SIMPLE_BOOL_VECTOR_H

struct BoolVector {
	bool x, y;

	BoolVector(bool x, bool y) : x(x), y(y) {}
	BoolVector(bool b) : BoolVector(b, b) {}
	BoolVector() : BoolVector(false) {}

	bool getOr() const { return x || y; }
	bool getXor() const { return x != y; }
	bool getAnd() const { return x && y; }

};

#endif
