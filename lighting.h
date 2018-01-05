/* Lighting is a framebuffer(represents darkness) where you draw with Sources(represents light)
 * When Lighting object is drawn, the colors underneath it are blended by multiplication
 *
 * Something something shadows something stencils
*/

#ifndef SIMPLE_LIGHTING_H
#define SIMPLE_LIGHTING_H

#include "glfw.h"
#include "sortedSprite.h"
#include "framebuffer.h"

namespace simpleGL {

class Lighting : public SortedSprite {
public:
	class Source : public Sprite {
	private:
		static GLuint lightingFragmentShader;

		Lighting* lighting;

		GLint centreLoc;
		GLint boundsLoc;

	protected:
		~Source() {
			lighting->sources.remove(this);
		}

	public:
		static GLuint getDefaultFragment();

		struct Loader {
			Point* pparent {nullptr};
			Vector pposition;
			Vector pscale {1};
			Angle protation;
			Texture ptexture;
			Vector ppivot;
			Color pcolor {1};
			Lighting* plighting {nullptr};

			Loader() {}

			Loader& parent(Point* p) { pparent = p; return *this; }
			Loader& position(Vector v) { pposition = v; return *this; }
			Loader& scale(Vector v) { pscale = v; return *this; }
			Loader& rotation(Angle a) { protation = a; return *this; }
			Loader& texture(Texture t) { ptexture = t; return *this; }
			Loader& pivot(Vector v) { ppivot = v; return *this; }
			Loader& color(Color c) { pcolor = c; return *this; }
			Loader& lighting(Lighting* l) { plighting = l; return *this; }

			Source* load() { return new Source(pparent, pposition, pscale, protation, ptexture, ppivot, pcolor, plighting); }
		};

		Source(Point* parent, Vector position, Vector scale, Angle rotation, Texture texture, Vector pivot, Color color, Lighting* lighting)
			: Sprite(parent, position, scale, rotation, texture, pivot, color), lighting(lighting) {
			setFragmentShader(getDefaultFragment());

			if (lighting)	lighting->sources.push_back(this);
		}

		bool getChanged() {
			bool result = changed.get();
			changed.reset();
			return result;
		}

		void setFragmentShader(GLuint sh) override {
			Sprite::setFragmentShader(sh);
			centreLoc = glGetUniformLocation(sh, "centre");
			boundsLoc = glGetUniformLocation(sh, "bounds");
		}

		void draw() override;

	};

	class Shadow : public Rectangle {
	private:
		Lighting* lighting;

		DrawObject object;

		DrawObject bottom;
		DrawObject middle;

	protected:
		~Shadow();

	public:
		struct Loader {
			Point* pparent {nullptr};
			Vector pposition;
			Vector pscale {1};
			Angle protation;
			Vector pbounds;
			Vector ppivot;
			Lighting* plighting {nullptr};

			Loader() {}

			Loader& parent(Point* p) { pparent = p; return *this; }
			Loader& position(Vector v) { pposition = v; return *this; }
			Loader& scale(Vector v) { pscale = v; return *this; }
			Loader& rotation(Angle a) { protation = a; return *this; }
			Loader& bounds(Vector v) { pbounds = v; return *this; }
			Loader& pivot(Vector v) { ppivot = v; return *this; }
			Loader& lighting(Lighting* l) { plighting = l; return *this; }

			Shadow* load() { return new Shadow(pparent, pposition, pscale, protation, pbounds, ppivot, plighting); }
		};

		Shadow(Point* parent, Vector position, Vector scale, Angle rotation, Vector bounds, Vector pivot, Lighting* lighting);

		bool getChanged() {
			bool result = changed.get();
			changed.reset();
			return result;
		}

		void draw(Source* source);
	};

private:
	Framebuffer framebuffer;

	std::list<Source*> sources;
	std::list<Shadow*> shadows;

	bool first {true};
	void init();

protected:
	~Lighting();

public:
	struct Loader {
		Point* pparent {nullptr};
		Vector pposition;
		Vector pscale {1};
		Angle protation;
		unsigned pwidth {1};
		unsigned pheight {1};
		Vector ppivot;
		Color pcolor {1};
		SortedFrame* pframe {getMainFrame()};
		int pz {0};
		Color pbase {0};

		Loader() {}

		Loader& parent(Point* p) { pparent = p; return *this; }
		Loader& position(Vector v) { pposition = v; return *this; }
		Loader& scale(Vector v) { pscale = v; return *this; }
		Loader& rotation(Angle a) { protation = a; return *this; }
		Loader& bounds(unsigned u0, unsigned u1) { pwidth = u0; pheight = u1; return *this; }
		Loader& pivot(Vector v) { ppivot = v; return *this; }
		Loader& color(Color c) { pcolor = c; return *this; }
		Loader& frame(SortedFrame* f) { pframe = f; return *this; }
		Loader& z(int i) { pz = i; return *this; }
		Loader& base(Color c) { pbase = c; return *this; }

		Lighting* load() { return new Lighting(pparent, pposition, pscale, protation, pwidth, pheight, ppivot, pcolor, pframe, pz, pbase); }
	};

	Lighting(Point* parent, Vector position, Vector scale, Angle rotation,
		unsigned width, unsigned height, Vector pivot, Color color, SortedFrame* frame, int z, Color base)
			: SortedSprite(parent, position, scale, rotation, {}, pivot, color, frame, z),
				framebuffer(width, height, GL_RGB, true, GL_LINEAR, base) {}

	const Image* getImage() const {
		return framebuffer.getImage();
	}

	void draw() override;

};

}

#endif
