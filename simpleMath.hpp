#ifndef SIMPLE_MATH_HPP
#define  SIMPLE_MATH_HPP

#include "simpleVector.hpp"

namespace simpleMath {

	inline float distance(SimpleVector lhs, SimpleVector rhs) {
		return (lhs - rhs).length();
	}

}

#endif
