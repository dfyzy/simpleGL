#include "math.h"

namespace simpleGL {

const Vector Vector::rotate(Angle a) const {
	return Vector(a.cos()*x - a.sin()*y, a.sin()*x + a.cos()*y);
}

}
