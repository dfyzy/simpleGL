#ifndef SIMPLE_ANGLE_H
#define SIMPLE_ANGLE_H

#include <cmath>

namespace simpleGL {

class Angle {
private:
	float angle {0};
	float sinA {0};
	float cosA {1};

	Angle(float angle, float sinA, float cosA) : angle(angle), sinA(sinA), cosA(cosA) {}

public:
	Angle(float angle) : angle(angle) {
		sinA = std::sin(angle);
		cosA = std::cos(angle);
	}

	Angle() {}

	float get() const { return angle; };
	float sin() const { return sinA; };
	float cos() const { return cosA; };

	const Angle operator-() const {

		return Angle(-angle, -sinA, cosA);
	}

	const Angle operator+(const Angle& a) const {

		return Angle(angle + a.angle, sinA*a.cosA + cosA*a.sinA, cosA*a.cosA - sinA*a.sinA);
	}

	const Angle operator-(const Angle& a) const {

		return operator+(-a);
	}

	Angle& operator+=(const Angle& a) {

		return *this = *this + a;
	}


	Angle& operator-=(const Angle& a) {

		return *this = *this - a;
	}

	const Angle operator*(int i) const {
		Angle a {*this};

		if (i < 0) {
			a = -a;
			i = -i;
		}

		Angle result;
		for (int j = 0; j < i; j++) {
			result += a;
		}

		return result;
	}

};

}

#endif
