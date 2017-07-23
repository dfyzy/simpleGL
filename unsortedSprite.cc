#include <queue>

#include "shaderData.h"
#include "util.h"

namespace {

GLint stencilCount {1};
std::queue<GLint> deletedStencils;

}

namespace simpleGL {

UnsortedSprite::UnsortedSprite(Point* parent, Texture texture, Anchor anchor, Vector position, Vector scale, Angle rotation, Color color)
							: BaseBoxShape(parent, anchor, position, scale, rotation), texture(texture), color(color) {
	drawObject = new DrawObject();

	vertexShader = getDefaultVertexShader();
	setDefaultFragmentShader();
}

UnsortedSprite::~UnsortedSprite() {
	stopUsingStencil();
	for (UnsortedSprite* st : stenciled)
		st->setStencil(nullptr);

	drawObject->unload();
}

void UnsortedSprite::bindVertices() {

	drawObject->bindVertexData(getModelMatrix() * Matrix::scale(texture.getBounds()));
}

void UnsortedSprite::bindColor() {

	drawObject->bindColorData(color);
}

void UnsortedSprite::bindTexture() {

	drawObject->bindTextureData(texture);
}

void UnsortedSprite::stopUsingStencil() {
	if (!stencil)	return;

	stencil->stenciled.remove(this);

	if (stencil->stenciled.empty()) {
		if (stencil->stencilRef < stencilCount - 1)
			deletedStencils.push(stencil->stencilRef);
		else
			stencilCount--;
	} else {
		stencilOp = GL_KEEP;
	}
}

void UnsortedSprite::setStencil(UnsortedSprite* spr) {
	setMask(spr);

	stopUsingStencil();

	stencil = spr;

	if (stencil) {
		if (stencil->stenciled.empty()) {
			if (!deletedStencils.empty()) {
				stencil->stencilRef = deletedStencils.front();
				deletedStencils.pop();
			} else	stencil->stencilRef = stencilCount++;

			stencil->stencilOp = GL_REPLACE;
		}
		stencilFunc = GL_EQUAL;
		stencilRef = stencil->stencilRef;

		stencil->stenciled.push_back(this);
	} else {
		stencilFunc = GL_ALWAYS;
	}
}

void UnsortedSprite::draw() {
	if (!isEnabled())	return;

	if (needUpdtTexture) {
		needUpdtTexture = false;
		bindTexture();
	}

	if (needUpdtVertices) {
		needUpdtVertices = false;
		bindVertices();
	}

	if (needUpdtColor) {
		needUpdtColor = false;
		bindColor();
	}

	texture.bind();

	glStencilFunc(stencilFunc, stencilRef, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, stencilOp);

	useShaders(vertexShader, fragmentShader);

	drawObject->draw();
}

}
