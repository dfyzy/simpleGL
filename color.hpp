#ifndef COLOR_H
#define COLOR_H

struct Color {
	float r, g, b, a;

	Color(float pr, float pg, float pb, float pa) : r(pr), g(pg), b(pb), a(pa) {}
	Color(float pr, float pg, float pb) : Color(pr, pg, pb, 1) {}
	Color(float shade) : Color(shade, shade, shade) {}
	Color() : Color(0, 0, 0, 0) {}

	const Color operator-(const Color& c) const {

		return Color(r - c.r, g - c.g, b - c.b, a);
	}

	const Color operator/(const float f) const {

		return Color(r/f, g/f, b/f, a);
	}

};

#endif
