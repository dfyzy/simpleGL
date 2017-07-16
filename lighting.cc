#include "lighting.h"
#include "simpleGL.h"
#include "util.h"
#include "shaderData.h"

namespace {

std::pair<int, int> SIDE_VERTS [9] {{0, 3}, {1, 3}, {1, 2},
											{0, 1}, {0, 0}, {3, 2},
											{2, 1}, {2, 0}, {3, 0}};

}

namespace simpleGL {

GLuint Lighting::Source::lightingFragmentShader = 0;

GLuint Lighting::Source::getDefaultFragment() {
	if (lightingFragmentShader == 0)
		lightingFragmentShader = loadShaderSource(shaderData::getLightingDefaultFragment(), GL_FRAGMENT_SHADER);

	return lightingFragmentShader;
}

void Lighting::Source::draw() {
	if (!isEnabled())	return;

	Vector pos = getRealCenter();
	glProgramUniform2f(getFragmentShader(), centreLoc, pos.x, pos.y);
	Vector b = getTexture().getBounds()*getScale();
	glProgramUniform2f(getFragmentShader(), boundsLoc, b.x, b.y);

	//TODO: stuff if overlapping
	glColorMask(false, false, false, false);

	setCustomStencil(GL_ALWAYS, 1, GL_REPLACE);

	UnsortedSprite::draw();

	util::useShaders(getDefaultVertexShader(), getDefaultFragmentShader(true));

	for (Shadow* sh : lighting->shadows)
		sh->draw(this);

	glColorMask(true, true, true, true);

	setCustomStencil(GL_EQUAL, 1, GL_ZERO);

	UnsortedSprite::draw();
}

void Lighting::Shadow::draw(Source* source) {
	if (!isEnabled())	return;

	Matrix objModel = getModelMatrix() * Matrix::translate(toFactor(anchor) * bounds * 0.5f) * Matrix::scale(bounds);
	object->bindVertexData(objModel);

	Matrix bottomModel = objModel;
	bottom->bindVertexData(bottomModel);

	float data[QUAD_VERTS*2];

	Vector pov = source->getRealCenter();

	Vector prj = objModel.inv() * pov;
	std::pair<int, int> verts = SIDE_VERTS[(prj.x > 0.5f) - (prj.x < -0.5f) + 1
													+ 3*((prj.y > 0.5f) - (prj.y < -0.5f) + 1)];

	Vector left = bottomModel * QUAD[verts.first];
	Vector right = bottomModel * QUAD[verts.second];

	int i = 0;
	left.load(data, &i);
	right.load(data, &i);

	Vector a = left - pov; Vector b = right - pov;
	float cosine = std::sqrt(a.dot(b)/(a.length()*b.length()) + 1)/2;
	if (cosine == 0) return;

	//an approximation of a formula i'm too lazy to implement accurately
	float length = (source->getTexture().getBounds() * source->getScale() * 0.5f).length()/cosine;

	(left + (left - pov).normalize()*length).load(data, &i);
	(right + (right - pov).normalize()*length).load(data, &i);

	middle->bindData(DataType::VERTEX, data);

	glStencilFunc(GL_EQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);

	bottom->draw();
	middle->draw();

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	object->draw();
}

Lighting::Lighting(Data d, int z, unsigned width, unsigned height, Color base)
				: Sprite({}, d, z) {
	util::println("Lighting:load");

	framebuffer = new Framebuffer(width, height, GL_RGB, true, GL_LINEAR, base);
}

Lighting::~Lighting() {
	util::println("Lighting:unload");

	for (Source* s : sources)
		s->unload();

	for (Shadow* s : shadows)
		s->unload();

	framebuffer->unload();
}

void Lighting::draw() {
	bool needToDraw = false;
	for (Shadow* sh : shadows)
		needToDraw |= sh->getChanged();

	for (Source* src : sources)
		needToDraw |= src->getChanged();

	if (first) {
		first = false;

		setTexture(framebuffer->getImage());
		needToDraw = true;
	}

	if (needToDraw) {
		glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);

		framebuffer->bind(getModelMatrix());

		for (Source* s : sources)
			s->draw();

		framebuffer->unbind();
	}

	glBlendFunc(GL_ZERO, GL_SRC_COLOR);

	Sprite::draw();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

}
