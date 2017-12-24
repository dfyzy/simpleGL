#include <queue>

#include "unsortedSprite.h"
#include "drawObject.h"
#include "shader.h"

namespace {

GLint stencilCount {0};
std::queue<GLint> deletedStencils;

}

namespace simpleGL {

UnsortedSprite::UnsortedSprite(Point* parent, Vector position, Vector scale, Angle rotation, Texture texture, Vector pivot, Color color)
	: Rectangle(parent, position, scale, rotation, texture.getBounds(), pivot),
		drawObject(new DrawObject()), texture(texture), color(color), vertexShader(getDefaultVertexShader()) {
	setDefaultFragmentShader();
}

UnsortedSprite::~UnsortedSprite() {
	stopUsingStencil();
	for (UnsortedSprite* st : stenciled)
		st->setStencil(nullptr);

	drawObject->unload();
}

unsigned UnsortedSprite::getId() const {
	return drawObject->getId();
}

void UnsortedSprite::setDefaultFragmentShader() {
	fragmentShader = getDefaultFragmentShader(texture.getImage() == nullptr);

	defaultFrag = true;
}

void UnsortedSprite::stopUsingStencil() {
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

void UnsortedSprite::setStencil(UnsortedSprite* spr) {
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

void UnsortedSprite::draw() {
	if (!isEnabled())	return;

	if (needUpdtTexture) {
		needUpdtTexture = false;
		drawObject->bindTextureData(texture);
	}

	if (needUpdtVertices) {
		needUpdtVertices = false;
		drawObject->bindVertexData(getCenter()->getModelMatrix() * Matrix::scale(texture.getBounds()));
	}

	if (needUpdtColor) {
		needUpdtColor = false;
		drawObject->bindColorData(color);
	}

	texture.bind();

	useShaders(vertexShader, fragmentShader);

	glStencilFunc(stencilFunc, stencilRef, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, stencilOp);

	drawObject->draw();
}

}
