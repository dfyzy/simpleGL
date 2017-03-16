#ifndef SIMPLE_LIGHT_H
#define SIMPLE_LIGHT_H

#include "sprite.h"
#include "texture.h"

namespace simpleGL {

class Light : public Image, public Sprite {
public:
	class Source : public UnsortedSprite {
	public:
		Light* light;

		GLint centreLoc;
		GLint boundsLoc;

		void bindVertices() {
			UnsortedSprite::bindVertices();

			light->toggleDraw();
		}

	public:
		Source(Light* light, UnsortedSprite* parent, Vector position, Vector bounds, Angle rotation, Color color);

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
	Light(UnsortedSprite* parent, Vector position, int z, unsigned width, unsigned height, Color base);

	void toggleDraw() { needToDraw = true; }

	void draw();

	void unload() {
		Image::unload();
		Sprite::unload();
	}

};

}

#endif
