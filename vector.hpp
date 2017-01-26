#ifndef SIMPLE_VECTOR_H
#define SIMPLE_VECTOR_H

#include <cmath>

namespace simpleGL {

struct Vector {
	float x, y;

	Vector(float px, float py) : x(px), y(py) {}
	Vector(float scale) : Vector(scale, scale) {}
	Vector() : Vector(0, 0) {}

	void set(float px, float py) {
		x = px;	y = py;
	}

	float length() const {
		return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
	}

	Vector normalize() const {
		if (x || y)	return operator/(length());

		return Vector();
	}

	Vector rotate(float rotation) const {
		float sinRot = std::sin(rotation);
		float cosRot = std::cos(rotation);

		return Vector(cosRot*x - sinRot*y, sinRot*x + cosRot*y);
	}

	Vector abs() const {
		return Vector(std::abs(x), std::abs(y));
	}

	const Vector operator+(const Vector& sp) const {

		return Vector(x + sp.x, y + sp.y);
	}

	const Vector operator-(const Vector& sp) const {

		return Vector(x - sp.x, y - sp.y);
	}

	Vector& operator+=(const Vector& sp) {
		x += sp.x;
		y += sp.y;
		return *this;
	}

	Vector& operator-=(const Vector& sp) {
		x -= sp.x;
		y -= sp.y;
		return *this;
	}

	const Vector operator*(const Vector& sp) const {

		return Vector(x * sp.x, y * sp.y);
	}

	const Vector operator/(const Vector& sp) const {

		return Vector(x / sp.x, y / sp.y);
	}

	Vector& operator*=(const Vector& sp) {
		x *= sp.x;
		y *= sp.y;
		return *this;
	}

	Vector& operator/=(const Vector& sp) {
		x /= sp.x;
		y /= sp.y;
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

	bool operator==(const Vector& sv) const {
		return x == sv.x && y == sv.y;
	}

	bool operator!=(const Vector& sv) const {
		return !operator==(sv);
	}

	bool operator<(const Vector& sv) const {
		if (x == sv.x)	return y < sv.y;

		return x < sv.x;
	}

};

}

#endif
