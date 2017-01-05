#ifndef SIMPLE_VECTOR_H
#define SIMPLE_VECTOR_H

#include <cmath>

struct SimpleVector {
	float x, y;

	SimpleVector(float px, float py) : x(px), y(py) {}
	SimpleVector(float scale) : SimpleVector(scale, scale) {}
	SimpleVector() : SimpleVector(0, 0) {}

	void set(float px, float py) {
		x = px;	y = py;
	}

	float length() const {
		return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
	}

	SimpleVector normalize() const {
		if (x || y)	return operator/(length());

		return SimpleVector();
	}

	SimpleVector rotate(float rotation) const {
		float sinRot = std::sin(rotation);
		float cosRot = std::cos(rotation);

		return SimpleVector(cosRot*x - sinRot*y, sinRot*x + cosRot*y);
	}

	const SimpleVector operator+(const SimpleVector& sp) const {

		return SimpleVector(x + sp.x, y + sp.y);
	}

	const SimpleVector operator-(const SimpleVector& sp) const {

		return SimpleVector(x - sp.x, y - sp.y);
	}

	SimpleVector& operator+=(const SimpleVector& sp) {
		x += sp.x;
		y += sp.y;
		return *this;
	}

	SimpleVector& operator-=(const SimpleVector& sp) {
		x -= sp.x;
		y -= sp.y;
		return *this;
	}

	const SimpleVector operator*(const SimpleVector& sp) const {

		return SimpleVector(x * sp.x, y * sp.y);
	}

	const SimpleVector operator/(const SimpleVector& sp) const {

		return SimpleVector(x / sp.x, y / sp.y);
	}

	SimpleVector& operator*=(const SimpleVector& sp) {
		x *= sp.x;
		y *= sp.y;
		return *this;
	}

	SimpleVector& operator/=(const SimpleVector& sp) {
		x /= sp.x;
		y /= sp.y;
		return *this;
	}

	const SimpleVector operator*(const float& f) const {

		return SimpleVector(x * f, y * f);
	}

	const SimpleVector operator/(const float& f) const {

		return operator*(1/f);
	}

	SimpleVector& operator*=(const float& f) {
		x *= f;
		y *= f;
		return *this;
	}

	SimpleVector& operator/=(const float& f) {
		return operator*=(1/f);
	}

	bool operator==(const SimpleVector& sv) const {
		return x == sv.x && y == sv.y;
	}

	bool operator!=(const SimpleVector& sv) const {
		return !operator==(sv);
	}

	bool operator<(const SimpleVector& sv) const {
		if (x == sv.x)	return y < sv.y;

		return x < sv.x;
	}

};

#endif
