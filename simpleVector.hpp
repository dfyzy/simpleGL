#ifndef SIMPLE_VECTOR_H
#define SIMPLE_VECTOR_H

#include <cmath>

struct SimpleVector {
	float x, y;

	SimpleVector(float px, float py) : x(px), y(py) {}
	SimpleVector(float scale) : SimpleVector(scale, scale) {}
	SimpleVector() : SimpleVector(0, 0) {}

	float length() const {
		return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
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

	const SimpleVector operator*(const SimpleVector& sp) const {

		return SimpleVector(x * sp.x, y * sp.y);
	}

	const SimpleVector operator/(const SimpleVector& sp) const {

		return SimpleVector(x / sp.x, y / sp.y);
	}

	const SimpleVector operator*(const float& f) const {

		return SimpleVector(x * f, y * f);
	}

	const SimpleVector operator/(const float& f) const {

		return operator*(1/f);
	}
};

#endif
