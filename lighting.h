#ifndef SIMPLE_LIGHTING_H
#define SIMPLE_LIGHTING_H

#include "sprite.h"
#include "framebuffer.h"

namespace simpleGL {

class Lighting : public Sprite {
public:
	class Source : public UnsortedSprite {
	private:
		static GLuint lightingFragmentShader;

		Lighting* lighting;

		Framebuffer* framebuffer;

		GLint centreLoc;
		GLint boundsLoc;

		Change* change;

		void bindColor() {
			UnsortedSprite::bindColor();

			setChanges();
		}

	protected:
		~Source();

	public:
		static GLuint getDefaultFragment();

		Source(Lighting* lighting, Texture t, Data d);

		bool getChanged() const { return change->get(); }

		void setFragmentShader(GLuint sh) {
			UnsortedSprite::setFragmentShader(sh);
			centreLoc = glGetUniformLocation(sh, "centre");
			boundsLoc = glGetUniformLocation(sh, "bounds");
		}

		void draw();

	};

	class Shadow : public Point {
	private:
		Lighting* lighting;

		DrawObject* bottom;
		DrawObject* middle;

		Vector bounds;

		Anchor anchor;
		Color color;

	protected:
		~Shadow();

	public:
		Shadow(Lighting* lighting, Vector bounds, Data d);

		Anchor getAnchor() const { return anchor; }
		void setAnchor(Anchor a) {
			if (anchor == a)	return;

			anchor = a;
			setChanges();
		}

		//TODO: color and transparency
		Color getColor() const { return color; }
		void setColor(Color c) { color = c; }

		void draw(Source* source);
	};

private:
	Framebuffer* framebuffer;

	std::list<Source*> sources;
	std::list<Shadow*> shadows;

	std::list<Change*> shadowChanges;

	bool first {true};
	void init();

protected:
	~Lighting();

public:
	Lighting(Data d, int z, unsigned width, unsigned height, Color base);

	Image* getImage() const { return framebuffer->getImage(); }

	void draw();

};

}

#endif
