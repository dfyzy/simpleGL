/* Stores an angle(in radians) with it's cosine and sine
 * This allows to do basic operations on the angle without recalculating them after each operation
*/

#ifndef SIMPLE_ANGLE_H
#define SIMPLE_ANGLE_H

#include <cmath>

namespace simpleGL {

constexpr float PI = 3.14159265358979323846f;

class Vector;

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

	Angle(Vector v);

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
		for (int j = 0; j < i; j++)
			result += a;

		return result;
	}

	bool operator==(const Angle& a) const {
		return angle == a.angle;
	}

	bool operator!=(const Angle& a) const {
		return angle != a.angle;
	}

};

}

#endif
