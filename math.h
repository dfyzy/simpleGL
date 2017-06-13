#ifndef SIMPLE_MATH_H
#define SIMPLE_MATH_H

#include <cmath>

namespace simpleGL {

constexpr float PI = 3.14159265358979323846f;

class Angle;

struct Vector {
	float x, y;

	constexpr Vector(float px, float py) : x(px), y(py) {}
	constexpr Vector(float scale) : Vector(scale, scale) {}
	constexpr Vector() : Vector(0, 0) {}

	void set(float px, float py) {
		x = px;	y = py;
	}

	float length() const {
		return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
	}

	const Vector normalize() const {
		if (x || y)	return operator/(length());

		return Vector();
	}

	const Vector rotate(Angle a) const;

	const Vector abs() const {
		return Vector(std::abs(x), std::abs(y));
	}

	const Vector round() const {
		return Vector(std::round(x), std::round(y));
	}

	void load(float* array, int* offset) const {
		array[(*offset)++] = x;
		array[(*offset)++] = y;
	}

	const Vector operator+(const Vector& v) const {

		return Vector(x + v.x, y + v.y);
	}

	const Vector operator-(const Vector& v) const {

		return Vector(x - v.x, y - v.y);
	}

	Vector& operator+=(const Vector& v) {
		x += v.x;
		y += v.y;
		return *this;
	}

	Vector& operator-=(const Vector& v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}

	const Vector operator*(const Vector& v) const {

		return Vector(x * v.x, y * v.y);
	}

	const Vector operator/(const Vector& v) const {

		return Vector(x / v.x, y / v.y);
	}

	Vector& operator*=(const Vector& v) {
		x *= v.x;
		y *= v.y;
		return *this;
	}

	Vector& operator/=(const Vector& v) {
		x /= v.x;
		y /= v.y;
		return *this;
	}

	const Vector operator*(const float& f) const {

		return Vector(x * f, y * f);
	}

	const Vector operator/(const float& f) const {

		return operator*(1/f);
	}

	Vector& operator*=(const float& f) {
		x *= f;
		y *= f;
		return *this;
	}

	Vector& operator/=(const float& f) {
		return operator*=(1/f);
	}

	bool operator==(const Vector& v) const {
		return x == v.x && y == v.y;
	}

	bool operator!=(const Vector& v) const {
		return !operator==(v);
	}

	bool operator<(const Vector& v) const {
		if (x == v.x)	return y < v.y;

		return x < v.x;
	}

};

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

	Angle(Vector v) {
		if (v != Vector()) {
			angle = std::atan2(v.y, v.x);

			float hyp = v.length();
			sinA = v.y/hyp;
			cosA = v.x/hyp;
		}
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

	bool operator==(const Angle& a) const {
		return angle == a.angle;
	}

	bool operator!=(const Angle& a) const {
		return angle != a.angle;
	}

};

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
