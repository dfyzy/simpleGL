#ifndef SIMPLE_MATH_h
#define SIMPLE_MATH_h

#include "vector.h"

namespace simpleMath {

inline float distance(simpleGL::Vector lhs, simpleGL::Vector rhs) {
	return (lhs - rhs).length();
}

}

#endif
