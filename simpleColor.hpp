#ifndef SIMPLE_COLOR_H
#define SIMPLE_COLOR_H

struct SimpleRGB {
	float r, g, b;

	SimpleRGB(float r, float g, float b) : r(r), g(g), b(b) {}
	SimpleRGB(float shade) : SimpleRGB(shade, shade, shade) {}

	const SimpleRGB operator+(const SimpleRGB& c) const {

		return SimpleRGB(r + c.r, g + c.g, b + c.b);
	}

	const SimpleRGB operator-(const SimpleRGB& c) const {

		return SimpleRGB(r - c.r, g - c.g, b - c.b);
	}

};

struct SimpleColor {
	float r, g, b, a;

	SimpleColor(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
	SimpleColor(SimpleRGB rgb, float a) : SimpleColor(rgb.r, rgb.g, rgb.b, a) {}
	SimpleColor(float r, float g, float b) : SimpleColor(r, g, b, 1) {}
	SimpleColor(SimpleRGB rgb) : SimpleColor(rgb.r, rgb.g, rgb.b) {}
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
