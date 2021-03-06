#include "lighting.h"
#include "framebuffer.h"
#include "drawObject.h"
#include "shader.h"
#include "shaderData.h"
#include "util.h"

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

	Vector pos = getCenter()->getRealPosition();
	glProgramUniform2f(getFragmentShader(), centreLoc, pos.x, pos.y);
	Vector b = getBounds()*getScale();
	glProgramUniform2f(getFragmentShader(), boundsLoc, b.x, b.y);

	//TODO: stuff if overlapping
	glColorMask(false, false, false, false);

	setCustomStencil(GL_ALWAYS, GL_REPLACE, 1);

	Sprite::draw();

	useShaders(getDefaultVertexShader(), getDefaultFragmentShader(true));

	for (Shadow* sh : lighting->shadows)
		sh->draw(this);

	glColorMask(true, true, true, true);

	setCustomStencil(GL_EQUAL, GL_ZERO, 1);

	Sprite::draw();
}

Lighting::Shadow::Shadow(Point* parent, Vector position, Vector scale, Angle rotation, Vector bounds, Vector pivot, Lighting* lighting)
	: Rectangle(parent, position, scale, rotation, bounds, pivot),
		lighting(lighting) {
	object.bindTextureData(bounds);
	bottom.bindTextureData({});
	middle.bindTextureData({});

	if (lighting)	lighting->shadows.push_back(this);
}

Lighting::Shadow::~Shadow() {
	lighting->shadows.remove(this);
}

void Lighting::Shadow::draw(Source* source) {
	if (!isEnabled())	return;

	Matrix objModel = getCenter()->getModelMatrix() * Matrix::scale(getBounds());
	object.bindVertexData(objModel);

	Matrix bottomModel = objModel;
	bottom.bindVertexData(bottomModel);

	float data[QUAD_VERTS*2];

	Vector pov = source->getCenter()->getRealPosition();

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
	float length = (source->getBounds() * source->getScale() * 0.5f).length()/cosine;

	(left + (left - pov).normalize()*length).load(data, &i);
	(right + (right - pov).normalize()*length).load(data, &i);

	middle.bindData(EBufferDataType::Vertex, data);

	glStencilFunc(GL_EQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);

	bottom.draw();
	middle.draw();

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	object.draw();
}

Lighting::~Lighting() {
	util::unloadContainer(sources);
	util::unloadContainer(shadows);
}

void Lighting::draw() {
	if (first) {
		first = false;

		setTexture(framebuffer.getImage());
	}

	bool needToDraw = changed.get();
	changed.reset();

	for (Shadow* sh : shadows)
		needToDraw |= sh->getChanged();

	for (Source* src : sources)
		needToDraw |= src->getChanged();

	if (needToDraw) {
		glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);

		framebuffer.bind(getCenter()->getModelMatrix());

		for (Source* s : sources)
			s->draw();

		framebuffer.unbind();
	}

	glBlendFunc(GL_ZERO, GL_SRC_COLOR);

	SortedSprite::draw();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

}
