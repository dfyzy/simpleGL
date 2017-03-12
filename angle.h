#ifndef SIMPLE_ANGLE_H
#define SIMPLE_ANGLE_H

#include <cmath>

namespace simpleGL {

class Angle {
private:
	double angle {0};
	double sinA {0};
	double cosA {1};

	void set(const Angle& a) {
		angle = a.angle; sinA = a.sinA; cosA = a.cosA;
	}

	Angle(double angle, double sinA, double cosA) : angle(angle), sinA(sinA), cosA(cosA) {}

public:
	Angle(double angle) : angle(angle) {
		sinA = std::sin(angle);
		cosA = std::cos(angle);
	}

	Angle() {}

	double get() const { return angle; };
	double sin() const { return sinA; };
	double cos() const { return cosA; };

	const Angle operator+(const Angle& a) const {

		return Angle(angle + a.angle, sinA*a.cosA + cosA*a.sinA, cosA*a.cosA - sinA*a.sinA);
	}

	const Angle operator-(const Angle& a) const {

		return Angle(angle - a.angle, sinA*a.cosA - cosA*a.sinA, cosA*a.cosA + sinA*a.sinA);
	}

	Angle& operator+=(const Angle& a) {
 		set(*this + a);

		return *this;
	}


	Angle& operator-=(const Angle& a) {
		set(*this - a);

		return *this;
	}

};

}

#endif
