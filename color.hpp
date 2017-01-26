#ifndef SIMPLE_COLOR_H
#define SIMPLE_COLOR_H

namespace simpleGL {

struct Rgb {
	float r, g, b;

	Rgb(float r, float g, float b) : r(r), g(g), b(b) {}
	Rgb(float shade) : Rgb(shade, shade, shade) {}

	const Rgb operator+(const Rgb& c) const {

		return Rgb(r + c.r, g + c.g, b + c.b);
	}

	const Rgb operator-(const Rgb& c) const {

		return Rgb(r - c.r, g - c.g, b - c.b);
	}

};

struct Color {
	float r, g, b, a;

	Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
	Color(Rgb rgb, float a) : Color(rgb.r, rgb.g, rgb.b, a) {}
	Color(float r, float g, float b) : Color(r, g, b, 1) {}
	Color(Rgb rgb) : Color(rgb.r, rgb.g, rgb.b) {}
	Color(float shade) : Color(shade, shade, shade) {}
	Color() : Color(0, 0, 0, 0) {}

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
