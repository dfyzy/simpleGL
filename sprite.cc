#include <queue>

#include "sprite.h"
#include "drawObject.h"
#include "shader.h"

namespace {

GLint stencilCount {0};
std::queue<GLint> deletedStencils;

}

namespace simpleGL {

Sprite::Sprite(Point* parent, Vector position, Vector scale, Angle rotation, Texture texture, Vector pivot, Color color)
	: Rectangle(parent, position, scale, rotation, texture.getBounds(), pivot),
		texture(texture), color(color), vertexShader(getDefaultVertexShader()) {
	setDefaultFragmentShader();
}

Sprite::~Sprite() {
	stopUsingStencil();
	for (Sprite* st : stenciled)
		st->setStencil(nullptr);
}

unsigned Sprite::getId() const {
	return drawObject.getId();
}

void Sprite::setDefaultFragmentShader() {
	fragmentShader = getDefaultFragmentShader(texture.getImage() == nullptr);

	defaultFrag = true;
}

void Sprite::stopUsingStencil() {
	if (!stencil)	return;

	stencil->stenciled.remove(this);

	if (stencil->stenciled.empty()) {
		if (stencil->getStencilRef() < stencilCount - 1)
			deletedStencils.push(stencil->getStencilRef());
		else
			stencilCount--;

		stencil->setDefaultStencil();
	}
}

void Sprite::setStencil(Sprite* spr) {
	stopUsingStencil();

	stencil = spr;

	if (stencil) {
		setMask(stencil);

		if (stencil->stenciled.empty()) {
			GLint ref;

			if (!deletedStencils.empty()) {
				ref = deletedStencils.front();
				deletedStencils.pop();
			} else	ref = ++stencilCount;

			stencil->setCustomStencil(GL_ALWAYS, GL_REPLACE, ref);
		}
		setCustomStencil(GL_EQUAL, GL_KEEP, stencil->getStencilRef());

		stencil->stenciled.push_back(this);
	} else {
		setMask(nullptr);

		setDefaultStencil();
	}
}

void Sprite::draw() {
	if (!isEnabled())	return;

	if (needUpdtTexture) {
		needUpdtTexture = false;
		drawObject.bindTextureData(texture);
	}

	if (needUpdtVertices) {
		needUpdtVertices = false;
		drawObject.bindVertexData(getCenter()->getModelMatrix() * Matrix::scale(texture.getBounds()));
	}

	if (needUpdtColor) {
		needUpdtColor = false;
		drawObject.bindColorData(color);
	}

	texture.bind();

	useShaders(vertexShader, fragmentShader);

	glStencilFunc(stencilFunc, stencilRef, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, stencilOp);

	drawObject.draw();
}

}
