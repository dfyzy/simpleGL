#include "shaderData.h"
#include "util.h"

namespace {

std::list<simpleGL::UnsortedSprite*> unsortedSprites;

GLint stencilCount {0};

}

namespace simpleGL {

void util::bindSprites() {
	for (UnsortedSprite* us : unsortedSprites)
		us->bindData();
}

UnsortedSprite::UnsortedSprite(Point* parent, Texture texture, Anchor anchor, Vector position, Vector scale, Angle rotation, Color color)
							: BaseBoxShape(parent, anchor, position, scale, rotation), texture(texture), color(color) {
	drawObject = new DrawObject();

	vertexShader = getDefaultVertexShader();
	setDefaultFragmentShader();

	unsortedSprites.push_back(this);
}

UnsortedSprite::~UnsortedSprite() {
	unsortedSprites.remove(this);

	drawObject->unload();
}

GLint UnsortedSprite::getStencil() {
	if (stencilOp == GL_KEEP) {
		stencilFunc = GL_ALWAYS;
		stencilOp = GL_REPLACE;
		stencilRef = ++stencilCount;
	}

	return stencilRef;
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

void UnsortedSprite::draw() {
	if (!isEnabled())	return;

	texture.bind();

	glStencilFunc(stencilFunc, stencilRef, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, stencilOp);

	util::useShaders(vertexShader, fragmentShader);

	drawObject->draw();
}

}
