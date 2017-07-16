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

		GLint centreLoc;
		GLint boundsLoc;

		Change* change;

		void bindColor() {
			UnsortedSprite::bindColor();

			setChanges();
		}

	protected:
		~Source() {
			lighting->sources.remove(this);
		}

	public:
		static GLuint getDefaultFragment();

		Source(Lighting* lighting, Texture t, Data d) : UnsortedSprite(t, d), lighting(lighting) {
			setFragmentShader(getDefaultFragment());

			if (lighting) {
				lighting->sources.push_back(this);
				change = getChange();
			}
		}

		bool getChanged() {
			bool result = change->get();
			change->reset();
			return result;
		}

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

		DrawObject* object;

		DrawObject* bottom;
		DrawObject* middle;

		Vector bounds;

		Anchor anchor;
		Color color;

		Change* change;

	protected:
		~Shadow() {
			object->unload();

			bottom->unload();
			middle->unload();

			lighting->shadows.remove(this);
		}

	public:
		Shadow(Lighting* lighting, Vector bounds, Data d)
				: Point(d.pparent, d.pposition, d.pscale, d.protation), lighting(lighting), bounds(bounds), anchor(d.panchor), color(d.pcolor) {
			object = new DrawObject();
			object->bindTextureData(bounds);

			bottom = new DrawObject();
			bottom->bindTextureData({});

			middle = new DrawObject();
			middle->bindTextureData({});

			if (lighting) {
				lighting->shadows.push_back(this);
				change = getChange();
			}
		}

		bool getChanged() {
			bool result = change->get();
			change->reset();
			return result;
		}

		Anchor getAnchor() const { return anchor; }
		void setAnchor(Anchor a) {
			if (anchor == a)	return;

			anchor = a;
			setChanges();
		}

		Vector getBounds() const { return bounds; }
		void setBounds(Vector v) {
			if (bounds == v)	return;

			bounds = v;

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
