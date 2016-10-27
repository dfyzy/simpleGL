#ifndef SIMPLE_COLOR_H
#define SIMPLE_COLOR_H

struct SimpleColor {
	float r, g, b, a;

	SimpleColor(float pr, float pg, float pb, float pa) : r(pr), g(pg), b(pb), a(pa) {}
	SimpleColor(float pr, float pg, float pb) : SimpleColor(pr, pg, pb, 1) {}
	SimpleColor(float shade) : SimpleColor(shade, shade, shade) {}
	SimpleColor() : SimpleColor(0, 0, 0, 0) {}

	const SimpleColor operator-(const SimpleColor& c) const {

		return SimpleColor(r - c.r, g - c.g, b - c.b, a);
	}

	const SimpleColor operator/(const float f) const {

		return SimpleColor(r/f, g/f, b/f, a);
	}

};

#endif
