#include "lighting.h"
#include "simpleGL.h"
#include "util.h"
#include "shaderData.h"

namespace {

std::pair<int, int> SIDE_VERTS [9] {{0, 3}, {1, 3}, {1, 2},
											{0, 1}, {0, 0}, {3, 2},
											{2, 1}, {2, 0}, {3, 0}};

bool shadowChanged;

}

namespace simpleGL {

GLuint Lighting::Source::lightingFragmentShader = 0;

GLuint Lighting::Source::getDefaultFragment() {
	if (lightingFragmentShader == 0)
		lightingFragmentShader = loadShaderSource(shaderData::getLightingDefaultFragment(), GL_FRAGMENT_SHADER);

	return lightingFragmentShader;
}

Lighting::Source::Source(Lighting* lighting, Texture t, Data d) : UnsortedSprite(t, d), lighting(lighting) {
	setFragmentShader(getDefaultFragment());

	Vector bounds = t.getBounds().ceil();
	framebuffer = new Framebuffer(bounds.x, bounds.y, GL_RGB, GL_NEAREST, {0});

	if (lighting) {
		lighting->sources.push_back(this);
		change = getChange();
	}
}

Lighting::Source::~Source() {
	framebuffer->unload();

	lighting->sources.remove(this);
}

void Lighting::Source::draw() {
	if (!isEnabled())	return;

	Vector pos = getRealCenter();
	glProgramUniform2f(getFragmentShader(), centreLoc, pos.x, pos.y);
	Vector b = getTexture().getBounds()*getScale();
	glProgramUniform2f(getFragmentShader(), boundsLoc, b.x, b.y);

	if (shadowChanged || change->get()) {
		change->reset();

		framebuffer->bind(getModelMatrix());

		glDisable(GL_BLEND);

		//TODO: stuff if overlapping

		UnsortedSprite::draw();

		for (Shadow* sh : lighting->shadows)
			sh->draw(this);

		glEnable(GL_BLEND);

		framebuffer->unbind();
	}

	util::useShaders(getDefaultVertexShader(), getDefaultFragmentShader(false));
	Texture(framebuffer->getImage()).bind();

	getDrawObject()->draw();
}

Lighting::Shadow::Shadow(Lighting* lighting, Vector bounds, Data d)
		: Point(d.pparent, d.pposition, d.pscale, d.protation), lighting(lighting), bounds(bounds), anchor(d.panchor), color(d.pcolor) {
	bottom = new DrawObject();
	bottom->bindTextureData({});
	bottom->bindColorData({0});

	middle = new DrawObject();
	middle->bindTextureData({});
	middle->bindColorData({0});

	if (lighting) {
		lighting->shadows.push_back(this);
		lighting->shadowChanges.push_back(getChange());
	}
}

Lighting::Shadow::~Shadow() {
	bottom->unload();
	middle->unload();

	lighting->shadows.remove(this);
}

void Lighting::Shadow::draw(Source* source) {
	if (!isEnabled())	return;

	util::useShaders(getDefaultVertexShader(), getDefaultFragmentShader(true));

	Matrix bottomModel = getModelMatrix() * Matrix::translate(toFactor(anchor) * bounds * 0.5f) * Matrix::scale(bounds);
	bottom->bindVertexData(bottomModel);

	float data[QUAD_VERTS*2];

	Vector pov = source->getRealCenter();

	Vector prj = bottomModel.inv() * pov;
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

	bottom->draw();
	middle->draw();
}

Lighting::Lighting(Data d, int z, unsigned width, unsigned height, Color base)
				: Sprite({}, d, z) {
	util::println("Lighting:load");

	framebuffer = new Framebuffer(width, height, GL_RGB, GL_LINEAR, base);
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
	for (Change* ch : shadowChanges)
		if (ch->get()) {
			ch->reset();
			needToDraw = true;
			shadowChanged = true;
		}

	for (Source* src : sources)
		if (src->getChanged()) {
			needToDraw = true;
			break;
		}

	if (first) {
		first = false;

		setTexture(framebuffer->getImage());
		needToDraw = true;
		shadowChanged = true;
	}

	if (needToDraw) {
		glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);

		framebuffer->bind(getModelMatrix());

		for (Source* s : sources)
			s->draw();

		framebuffer->unbind();
	}
	shadowChanged = false;

	glBlendFunc(GL_ZERO, GL_SRC_COLOR);

	Sprite::draw();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

}
