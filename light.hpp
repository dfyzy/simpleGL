#ifndef SIMPLE_LIGHT_H
#define SIMPLE_LIGHT_H

#include "sprite.hpp"
#include "texture.hpp"

namespace simpleGL {

class Light : public Image, public Sprite {
public:
	class Source : public UnsortedSprite {
	public:
		Light* light;

		GLint centreLoc;
		GLint boundsLoc;

	public:
		Source(Light* light, Vector position, Vector bounds, float rotation, Color color);

		Source(const Source& other) =delete;
		Source(Source&& other) =delete;

		~Source() {
			light->sources.remove(this);
		}

		void setFragmentShader(GLuint sh) {
			UnsortedSprite::setFragmentShader(sh);
			centreLoc = glGetUniformLocation(sh, "centre");
			boundsLoc = glGetUniformLocation(sh, "bounds");
		}

		void setPosition(Vector position) {
			UnsortedSprite::setPosition(position - light->position);
			light->toggleDraw();
		}

		void setScale(Vector scale) {
			UnsortedSprite::setScale(scale);
			light->toggleDraw();
		}

		void setRotation(float rotation) {
			UnsortedSprite::setRotation(rotation);
			light->toggleDraw();
		}

		void setColor(Color color) {
			UnsortedSprite::setColor(color);
			light->toggleDraw();
		}

		void draw();

	};

private:
	Color base;

	GLuint fbo;
	std::list<Source*> sources;

	bool needToDraw = true;

	~Light();

public:
	Light(Vector position, int z, unsigned width, unsigned height, Color base);

	void toggleDraw() { needToDraw = true; }

	void draw();

	void unload() {
		Image::unload();
		Sprite::unload();
	}

};

}

#endif
