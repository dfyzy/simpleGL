#include <queue>

#include "unsortedSprite.h"

namespace {

GLint stencilCount {1};
std::queue<GLint> deletedStencils;

}

namespace simpleGL {

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
		drawObject->bindTextureData(texture);
	}

	if (needUpdtVertices) {
		needUpdtVertices = false;
		drawObject->bindVertexData(getOffsetedModelMatrix() * Matrix::scale(texture.getBounds()));
	}

	if (needUpdtColor) {
		needUpdtColor = false;
		drawObject->bindColorData(color);
	}

	texture.bind();

	glStencilFunc(stencilFunc, stencilRef, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, stencilOp);

	useShaders(vertexShader, fragmentShader);

	drawObject->draw();
}

}
