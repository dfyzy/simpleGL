#ifndef SIMPLE_STRUCTS_H
#define SIMPLE_STRUCTS_H

struct SimpleVector {
	float x, y;

	SimpleVector(float px, float py) : x(px), y(py) {}
	SimpleVector(float scale) : SimpleVector(scale, scale) {}
	SimpleVector() : SimpleVector(0, 0) {}

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

struct SimpleColor {
	float r, g, b, a;

	SimpleColor(float pr, float pg, float pb, float pa) : r(pr), g(pg), b(pb), a(pa) {}
	SimpleColor(float pr, float pg, float pb) : SimpleColor(pr, pg, pb, 1) {}
	SimpleColor(float shade) : SimpleColor(shade, shade, shade) {}
	SimpleColor() : SimpleColor(0, 0, 0, 0) {}

	const SimpleColor operator+(const SimpleColor& c) const {

		return SimpleColor(r + c.r, g + c.g, b + c.b, a);
	}

	const SimpleColor operator-(const SimpleColor& c) const {

		return SimpleColor(r - c.r, g - c.g, b - c.b, a);
	}

	const SimpleColor operator*(const float& f) const {

		return SimpleColor(r*f, g*f, b*f, a);
	}

	const SimpleColor operator/(const float& f) const {

		return operator*(1/f);
	}

};

#endif
