#ifndef SIMPLE_LIGHT_H
#define SIMPLE_LIGHT_H

#include <list>
#include "sprite.hpp"
#include "texture.hpp"

namespace simpleGL {

class Light : public Texture, public Sprite {
public:
	class Source : public UnsortedSprite {
	public:
		Light* light;

	public:
		Source(Light* light, Vector position, Vector bounds, float rotation, Rgb rgb);

		Source(const Source& other) =delete;
		Source(Source&& other) =delete;

		~Source() {
			light->sources.remove(this);
		}

		void setPosition(Vector position) const {
			UnsortedSprite::setPosition(position - light->position);
			light->toggleDraw();
		}

		void setBounds(Vector bounds) const {
			UnsortedSprite::setBounds(bounds);
			light->toggleDraw();
		}

		void setRotation(float rotation) const {
			UnsortedSprite::setRotation(rotation);
			light->toggleDraw();
		}

		void setColor(Rgb c) const {
			UnsortedSprite::setColor(c);
			light->toggleDraw();
		}

	};

private:
	Vector position;
	Rgb baseRgb;

	GLuint fbo;
	std::list<Source*> sources;

	bool needToDraw = true;

	~Light();

public:
	Light(Vector position, int z, unsigned width, unsigned height, Rgb baseRgb);

	void toggleDraw() { needToDraw = true; }

	void draw();

	void unload() {
		Texture::unload();
		Sprite::unload();
	}

};

}

#endif
