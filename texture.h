/* Represents rectangle cut-out from an image
*/

#ifndef SIMPLE_TEXTURE_H
#define SIMPLE_TEXTURE_H

#include "matrix.h"

namespace simpleGL {

class Image;

class Texture {
protected:
	const Image* image {nullptr};
	Vector position;
	Vector bounds {1};

public:
	Texture() {}
	Texture(Vector bounds) : bounds(bounds) {}

	Texture(const Image* image, Vector position, Vector bounds) : image(image), position(position), bounds(bounds) {}
	Texture(const Image* image);

	const Image* getImage() const { return image; }

	Vector getPosition() const { return position; }
	void setPosition(Vector v) { position = v; }

	Vector getBounds() const { return bounds; }
	void setBounds(Vector v) { bounds = v; }

	Matrix getMatrix() const {
		return Matrix::translate(position + bounds*0.5f) * Matrix::scale(bounds);
	}

	void bind();

	bool operator==(const Texture& t) const {
		return image == t.image && position == t.position && bounds == t.bounds;
	}

	bool operator!=(const Texture& t) const {
		return !operator==(t);
	}

};

}

#endif
