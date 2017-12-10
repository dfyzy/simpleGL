/* Lighting is a framebuffer(represents darkness) where you draw with Sources(represents light)
 * When Lighting object is drawn, the colors underneath it are blended by multiplication
 *
 * Something something shadows something stencils
*/

#ifndef SIMPLE_LIGHTING_H
#define SIMPLE_LIGHTING_H

#include "glfw.h"
#include "sprite.h"

namespace simpleGL {

class Framebuffer;

class Lighting : public Sprite {
public:
	class Source : public UnsortedSprite {
	private:
		static GLuint lightingFragmentShader;

		Lighting* lighting;

		GLint centreLoc;
		GLint boundsLoc;

		Change* change;

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
			EAnchor panchor {EAnchor::Center};
			Color pcolor {1};
			Lighting* plighting {nullptr};

			Loader() {}

			Loader& parent(Point* p) { pparent = p; return *this; }
			Loader& position(Vector v) { pposition = v; return *this; }
			Loader& scale(Vector v) { pscale = v; return *this; }
			Loader& rotation(Angle a) { protation = a; return *this; }
			Loader& texture(Texture t) { ptexture = t; return *this; }
			Loader& anchor(EAnchor a) { panchor = a; return *this; }
			Loader& color(Color c) { pcolor = c; return *this; }
			Loader& lighting(Lighting* l) { plighting = l; return *this; }

			Source* load() { return new Source(pparent, pposition, pscale, protation, ptexture, panchor, pcolor, plighting); }
		};

		Source(Point* parent, Vector position, Vector scale, Angle rotation, Texture texture, EAnchor anchor, Color color, Lighting* lighting)
			: UnsortedSprite(parent, position, scale, rotation, texture, anchor, color), lighting(lighting), change(getChange()) {
			setFragmentShader(getDefaultFragment());

			if (lighting)	lighting->sources.push_back(this);
		}

		bool getChanged() {
			bool result = change->get();
			change->reset();
			return result;
		}

		void setFragmentShader(GLuint sh) override {
			UnsortedSprite::setFragmentShader(sh);
			centreLoc = glGetUniformLocation(sh, "centre");
			boundsLoc = glGetUniformLocation(sh, "bounds");
		}

		void draw() override;

	};

	class Shadow : public AnchoredBox {
	private:
		Lighting* lighting;

		DrawObject* object;

		DrawObject* bottom;
		DrawObject* middle;

		Change* change;

	protected:
		~Shadow();

	public:
		struct Loader {
			Point* pparent {nullptr};
			Vector pposition;
			Vector pscale {1};
			Angle protation;
			Vector pbounds;
			EAnchor panchor {EAnchor::Center};
			Lighting* plighting {nullptr};

			Loader() {}

			Loader& parent(Point* p) { pparent = p; return *this; }
			Loader& position(Vector v) { pposition = v; return *this; }
			Loader& scale(Vector v) { pscale = v; return *this; }
			Loader& rotation(Angle a) { protation = a; return *this; }
			Loader& bounds(Vector v) { pbounds = v; return *this; }
			Loader& anchor(EAnchor a) { panchor = a; return *this; }
			Loader& lighting(Lighting* l) { plighting = l; return *this; }

			Shadow* load() { return new Shadow(pparent, pposition, pscale, protation, pbounds, panchor, plighting); }
		};

		Shadow(Point* parent, Vector position, Vector scale, Angle rotation, Vector bounds, EAnchor anchor, Lighting* lighting);

		bool getChanged() {
			bool result = change->get();
			change->reset();
			return result;
		}

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
	struct Loader {
		Point* pparent {nullptr};
		Vector pposition;
		Vector pscale {1};
		Angle protation;
		unsigned pwidth {1};
		unsigned pheight {1};
		EAnchor panchor {EAnchor::Center};
		Color pcolor {1};
		int pz {0};
		Color pbase {0};

		Loader() {}

		Loader& parent(Point* p) { pparent = p; return *this; }
		Loader& position(Vector v) { pposition = v; return *this; }
		Loader& scale(Vector v) { pscale = v; return *this; }
		Loader& rotation(Angle a) { protation = a; return *this; }
		Loader& bounds(unsigned u0, unsigned u1) { pwidth = u0; pheight = u1; return *this; }
		Loader& anchor(EAnchor a) { panchor = a; return *this; }
		Loader& color(Color c) { pcolor = c; return *this; }
		Loader& z(int i) { pz = i; return *this; }
		Loader& base(Color c) { pbase = c; return *this; }

		Lighting* load() { return new Lighting(pparent, pposition, pscale, protation, pwidth, pheight, panchor, pcolor, pz, pbase); }
	};

	Lighting(Point* parent, Vector position, Vector scale, Angle rotation,
		unsigned width, unsigned height, EAnchor anchor, Color color, int z, Color base);

	Image* getImage() const;

	void draw() override;

};

}

#endif
