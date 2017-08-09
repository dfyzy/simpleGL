/* Vector class with 2 elements and some utility
 * Used primarily for position and bounds
*/

#ifndef SIMPLE_VECTOR_H
#define SIMPLE_VECTOR_H

#include <cmath>

namespace simpleGL {

class Angle;

struct Vector {
	float x, y;

	constexpr Vector(float x, float y) : x(x), y(y) {}
	constexpr Vector(float scale) : Vector(scale, scale) {}
	constexpr Vector() : Vector(0, 0) {}

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

	const Vector ceil() const {
		return Vector(std::ceil(x), std::ceil(y));
	}

	const float dot(const Vector& v) const {
		return x*v.x + y*v.y;
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

	const Vector operator-() const {

		return Vector(-x, -y);
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

}

#endif
