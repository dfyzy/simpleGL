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

	float lengthSquared() const {
		return x*x + y*y;
	}

	float length() const {
		return std::sqrt(lengthSquared());
	}

	Vector normalize() const {
		if (x || y)	return operator/(length());

		return Vector();
	}

	Vector rotate(Angle a) const;

	Vector abs() const {
		return Vector(std::abs(x), std::abs(y));
	}

	Vector round() const {
		return Vector(std::round(x), std::round(y));
	}

	Vector ceil() const {
		return Vector(std::ceil(x), std::ceil(y));
	}

	float dot(const Vector& v) const {
		return x*v.x + y*v.y;
	}

	float cross(const Vector& v) const {
		return x*v.y - y*v.x;
	}

	Vector perp() const {
		return Vector(-y, x);
	}

	void load(float* array, int* offset) const {
		array[(*offset)++] = x;
		array[(*offset)++] = y;
	}

	Vector operator+(const Vector& v) const {

		return Vector(x + v.x, y + v.y);
	}

	Vector operator-(const Vector& v) const {

		return Vector(x - v.x, y - v.y);
	}

	Vector operator-() const {

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

	Vector operator*(const Vector& v) const {

		return Vector(x * v.x, y * v.y);
	}

	Vector operator/(const Vector& v) const {

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

	Vector operator*(const float& f) const {

		return Vector(x * f, y * f);
	}

	Vector operator/(const float& f) const {

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
