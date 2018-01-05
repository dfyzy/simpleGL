/* Base sprite class
 * Doesn't draw automatically
*/

#ifndef SIMPLE_SPRITE_H
#define SIMPLE_SPRITE_H

#include "glfw.h"
#include "rectangle.h"
#include "texture.h"
#include "color.h"
#include "drawObject.h"

namespace simpleGL {

class Sprite : public Rectangle {
private:
	DrawObject drawObject;

	Texture texture;

	Color color;

	bool needUpdtVertices {true};
	bool needUpdtTexture {true};
	bool needUpdtColor {true};

	GLuint vertexShader;
	GLuint fragmentShader;

	bool defaultFrag {true};

	GLenum stencilFunc = GL_ALWAYS;
	GLenum stencilOp = GL_KEEP;
	GLint stencilRef {0};

	Sprite* stencil {nullptr};
	std::list<Sprite*> stenciled;

	void stopUsingStencil();

protected:
	const DrawObject* getDrawObject() const { return &drawObject; }

	void updateModel() override {
		needUpdtVertices = true;

		Rectangle::updateModel();
	}

	~Sprite();

public:
	struct Loader {
		Point* pparent {nullptr};
		Vector pposition;
		Vector pscale {1};
		Angle protation;
		Texture ptexture;
		Vector ppivot;
		Color pcolor {1};

		Loader() {}

		Loader& parent(Point* p) { pparent = p; return *this; }
		Loader& position(Vector v) { pposition = v; return *this; }
		Loader& scale(Vector v) { pscale = v; return *this; }
		Loader& rotation(Angle a) { protation = a; return *this; }
		Loader& texture(Texture t) { ptexture = t; return *this; }
		Loader& pivot(Vector v) { ppivot = v; return *this; }
		Loader& color(Color c) { pcolor = c; return *this; }

		Sprite* load() { return new Sprite(pparent, pposition, pscale, protation, ptexture, ppivot, pcolor); }
	};

	Sprite(Point* parent, Vector position, Vector scale, Angle rotation, Texture texture, Vector pivot, Color color);

	unsigned getId() const;

	Texture getTexture() const { return texture; }
	void setTexture(Texture t) {
		if (texture == t)	return;

		texture = t;

		needUpdtTexture = true;
		needUpdtVertices = true;

		setBounds(t.getBounds());

		if (defaultFrag) setDefaultFragmentShader();
	}

	Color getColor() const { return color; }
	void setColor(Color c) {
		if (color == c)	return;

		color = c;

		needUpdtColor = true;
		onChange();
	}

	GLuint getVertexShader() const { return vertexShader; }
	void setVertexShader(GLuint sh) { vertexShader = sh; }

	GLuint getFragmentShader() const { return fragmentShader; }
	virtual void setFragmentShader(GLuint sh) { fragmentShader = sh; defaultFrag = false; }

	void setDefaultFragmentShader();

	GLenum getStencilFunc() const { return stencilFunc; }
	GLenum getStencilOp() const { return stencilOp; }
	GLint getStencilRef() const { return stencilRef; }

	void setStencil(Sprite* spr);
	void setCustomStencil(GLenum func, GLenum op, GLint ref) {
		stencilFunc = func;
		stencilOp = op;
		stencilRef = ref;
	}
	void setDefaultStencil() { setCustomStencil(GL_ALWAYS, GL_KEEP, 0); }

	virtual void draw();

};

}

#endif
