/* Some usefull math functions
*/

#ifndef SIMPLE_MATH_H
#define SIMPLE_MATH_H

namespace simpleGL::math {

template <typename T>
inline T lerp(T lhs, T rhs, float t) {
	return lhs*(1 - t) + rhs*t;
}

}

#endif
