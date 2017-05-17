#ifndef SIMPLE_LIGHT_H
#define SIMPLE_LIGHT_H

#include "sprite.h"
#include "texture.h"

namespace simpleGL {

class Light : public Sprite {
public:
	class Source : public UnsortedSprite {
	private:
		static GLuint lightingFragmentShader;

		Light* light;

		GLint centreLoc;
		GLint boundsLoc;

		void bindVertices() {
			UnsortedSprite::bindVertices();

			light->toggleDraw();
		}

		void bindColor() {
			UnsortedSprite::bindColor();

			light->toggleDraw();
		}

	protected:
		~Source();

	public:
		static GLuint getDefaultFragment();

		Source(Light* light, UnsortedSprite* parent, Anchor anchor, Vector position, Vector bounds, Angle rotation, Color color);

		void setFragmentShader(GLuint sh) {
			UnsortedSprite::setFragmentShader(sh);
			centreLoc = glGetUniformLocation(sh, "centre");
			boundsLoc = glGetUniformLocation(sh, "bounds");
		}

		void draw();

	};

private:
	Image* image;

	Color base;

	GLuint fbo;
	std::list<Source*> sources;

	bool needToDraw = true;

	bool first {true};
	void init();

protected:
	~Light();

public:
	Light(UnsortedSprite* parent, Anchor anchor, Vector position, int z, unsigned width, unsigned height, Color base);

	Image* getImage() const { return image; }

	void toggleDraw() { needToDraw = true; }

	void draw();

};

}

#endif
