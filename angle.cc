#include "angle.h"
#include "vector.h"

namespace simpleGL {

Angle::Angle(Vector v) {
	if (v != Vector()) {
		angle = std::atan2(v.y, v.x);

		float hyp = v.length();
		sinA = v.y/hyp;
		cosA = v.x/hyp;
	}
}

}
