#ifndef SIMPLE_MATH_HPP
#define SIMPLE_MATH_HPP

#include "vector.hpp"

namespace simpleMath {

inline float distance(simpleGL::Vector lhs, simpleGL::Vector rhs) {
	return (lhs - rhs).length();
}

}

#endif
