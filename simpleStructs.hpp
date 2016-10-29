#ifndef SIMPLE_STRUCTS_H
#define SIMPLE_STRUCTS_H

struct SimplePosition {
	float x, y;
	int z;

	SimplePosition(float px, float py, int pz) : x(px), y(py), z(pz) {}
	SimplePosition(float px, float py) : SimplePosition(px, py, 0) {}
	SimplePosition() : SimplePosition(0, 0) {}

	const SimplePosition operator+(const SimplePosition& sp) const {

		return SimplePosition(x + sp.x, y + sp.y, z + sp.z);
	}

	const SimplePosition operator-(const SimplePosition& sp) const {

		return SimplePosition(x - sp.x, y - sp.y, z - sp.z);
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

	const SimpleColor operator*(const float f) const {

		return SimpleColor(r*f, g*f, b*f, a);
	}

	const SimpleColor operator/(const float f) const {

		return operator*(1/f);
	}

};

#endif
