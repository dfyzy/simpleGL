#ifndef SIMPLE_MATH_h
#define SIMPLE_MATH_h

#include "vector.h"

namespace simpleGL {

namespace math {

inline float distance(Vector lhs, Vector rhs) {
	return (lhs - rhs).length();
}

template <typename T>
inline T lerp(T lhs, T rhs, float t) {
	return lhs*(1 - t) + rhs*t;
}

}

}

#endif
