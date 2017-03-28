#ifndef SIMPLE_COLOR_H
#define SIMPLE_COLOR_H

namespace simpleGL {

struct Color {
	float r, g, b, a;

	constexpr Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
	constexpr Color(float r, float g, float b) : Color(r, g, b, 1) {}
	constexpr Color(float shade) : Color(shade, shade, shade) {}
	constexpr Color() : Color(0, 0, 0, 0) {}

	const Color operator+(const Color& c) const {

		return Color(r + c.r, g + c.g, b + c.b, a);
	}

	const Color operator-(const Color& c) const {

		return Color(r - c.r, g - c.g, b - c.b, a);
	}

	const Color operator*(const float& f) const {

		return Color(r*f, g*f, b*f, a);
	}

	const Color operator/(const float& f) const {

		return operator*(1/f);
	}

};

}

#endif
