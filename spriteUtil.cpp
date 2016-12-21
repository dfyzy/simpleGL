#include <queue>
#include <set>

#include "simpleGL.hpp"
#include "simpleUtil.hpp"
#include "simpleSprite.hpp"

struct Attrib {
	enum E { POSITION, BOUNDS, ROTATION, COLOR, TEX_DATA, COUNT } type;
	static const unsigned sizes[5];
};

const unsigned Attrib::sizes[5] = {2, 2, 1, 4, 4};

namespace simpleUtil {
	const int SPRITE_SIZE = 2 + 2 + 1 + 4 + 4;

	GLuint vbos[Attrib::COUNT];

	GLuint currentTexture = 0;

	//check if sorted vector is faster
	std::set<SimpleSprite*, SimpleSprite::Comparer> sprites;

	std::queue<unsigned> deletedQueue;

	unsigned spriteCount = 0;
	unsigned spriteCapacity = 4;

	inline void loadVector(SimpleVector sv, float* array, int* offset) {
		array[(*offset)++] = sv.x;
		array[(*offset)++] = sv.y;
	}

	inline void loadPosition(SimpleVector position, float* array, int* offset) {

		loadVector(simpleGL::actualToScreen(position), array, offset);
	}

	inline void loadBounds(SimpleVector bounds, SimpleTexture* tex, float* array, int* offset) {

		loadVector(simpleGL::actualToScreen(bounds*SimpleVector(tex->getWidth(), tex->getHeight())), array, offset);
	}

	inline void loadRotation(float rotation, float* array, int* offset) {
		array[(*offset)++] = rotation;
	}

	inline void loadColor(SimpleColor c, float* array, int* offset) {
		array[(*offset)++] = c.r;
		array[(*offset)++] = c.g;
		array[(*offset)++] = c.b;
		array[(*offset)++] = c.a;
	}

	inline void loadTexData(float x, float y, float width, float height, float* array, int* offset) {
		array[(*offset)++] = x + width*0.5f;
		array[(*offset)++] = y + height*0.5f;
		array[(*offset)++] = width;
		array[(*offset)++] = height;
	}

	void initBuffers() {
		glGenBuffers(Attrib::COUNT, vbos);

		for (int i = 0; i < Attrib::COUNT; i++) {
			glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);

			//alocating data for spriteCapacity number of sprites for a start.
			glBufferData(GL_ARRAY_BUFFER, spriteCapacity * Attrib::sizes[i] * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

			//binding buffers to layout locations in vertex shader.
			glVertexAttribPointer(i, Attrib::sizes[i], GL_FLOAT, GL_FALSE, 0, nullptr);
			glEnableVertexAttribArray(i);
		}
		print("Buffers initialized");
	}

	inline void bindSpriteAttrib(Attrib::E type, unsigned offset, float* data) {
		glBindBuffer(GL_ARRAY_BUFFER, vbos[type]);

		int size = Attrib::sizes[type] * sizeof(float);
		glBufferSubData(GL_ARRAY_BUFFER, offset * size, size, data);
	}

	void drawSprites() {
		for (SimpleSprite* cs : sprites)
			cs->draw();
	}

}

using namespace simpleUtil;

SimpleSprite* simpleGL::loadSprite(SimpleTexture* tex, SimpleVector position, int z, SimpleVector bounds, float rotation, SimpleColor c,
															float texX, float texY, float texW, float texH) {
	print("Adding sprite");

	float data[SPRITE_SIZE];
	int offset = 0;
	loadPosition(position, data, &offset);
	loadBounds(bounds, tex, data, &offset);
	loadRotation(rotation, data, &offset);
	loadColor(c, data, &offset);
	loadTexData(texX, texY, texW, texH, data, &offset);

	unsigned id;
	if (!deletedQueue.empty()) {
		id = deletedQueue.front();
		deletedQueue.pop();
	}else	id = spriteCount++;

	if (spriteCapacity < spriteCount) {
		print("Resizing data buffers");

		GLuint tempVbo;
		glGenBuffers(1, &tempVbo);

		for (int i = 0; i < Attrib::COUNT; i++) {
			int size = Attrib::sizes[i] * sizeof(float);
			int oldSize = spriteCapacity * size;

			//loading current data into temp buffer, resizing this buffer and loading data back from temp buffer.
			glBindBuffer(GL_COPY_WRITE_BUFFER, tempVbo);
			glBufferData(GL_COPY_WRITE_BUFFER, oldSize, nullptr, GL_DYNAMIC_DRAW);

			glBindBuffer(GL_COPY_READ_BUFFER, vbos[i]);
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, oldSize);

			glBindBuffer(GL_COPY_WRITE_BUFFER, vbos[i]);
			glBufferData(GL_COPY_WRITE_BUFFER, 2 * spriteCount * size, nullptr, GL_DYNAMIC_DRAW);

			glBindBuffer(GL_COPY_READ_BUFFER, tempVbo);
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, oldSize);

			// TODO: check if one copy/attribpointer is faster than copy/copy
			// GLuint t = vbos[i];
			// vbos[i] = tempVbo;
			// tempVbo = t;
		}
		glDeleteBuffers(1, &tempVbo);

		spriteCapacity = 2*spriteCount;
	}

	int dataOffset = 0;
	for (int i = 0; i < Attrib::COUNT; i++) {
		bindSpriteAttrib(static_cast<Attrib::E>(i), id, data + dataOffset);

		dataOffset += Attrib::sizes[i];
	}

	SimpleSprite* sprite = new SimpleSprite(id, z, tex);
	setDefaultShaders(sprite, tex->getTexture() == 0);

	sprites.insert(sprite);

	return sprite;
}

SimpleSprite::~SimpleSprite() {
	print("Unloading sprite");

	if (id < spriteCount - 1)
		deletedQueue.push(id);
	else
		spriteCount--;

	sprites.erase(this);
}

void SimpleSprite::setEnabled(bool b) {
	if (!b && enabled)		sprites.erase(this);
	else if (b && !enabled)	sprites.insert(this);

	enabled = b;
}

void SimpleSprite::setZ(int pz) {
	sprites.erase(this);
	z = pz;
	sprites.insert(this);
}

void SimpleSprite::setTexture(SimpleTexture* tex) {
	sprites.erase(this);
	texture = tex;
	sprites.insert(this);
}

void SimpleSprite::setPosition(SimpleVector position) {
	float data[Attrib::sizes[Attrib::POSITION]];
	int offset = 0;
	loadPosition(position, data, &offset);

	bindSpriteAttrib(Attrib::POSITION, id, data);
}

void SimpleSprite::setBounds(SimpleVector bounds) {
	float data[Attrib::sizes[Attrib::BOUNDS]];
	int offset = 0;
	loadBounds(bounds, texture, data, &offset);

	bindSpriteAttrib(Attrib::BOUNDS, id, data);
}

void SimpleSprite::setRotation(float rotation) {
	float data[Attrib::sizes[Attrib::ROTATION]];
	int offset = 0;
	loadRotation(rotation, data, &offset);

	bindSpriteAttrib(Attrib::ROTATION, id, data);
}

void SimpleSprite::setColor(SimpleColor c) {
	float data[Attrib::sizes[Attrib::COLOR]];
	int offset = 0;
	loadColor(c, data, &offset);

	bindSpriteAttrib(Attrib::COLOR, id, data);
}

void SimpleSprite::setTexData(float x, float y, float width, float height) {
	float data[Attrib::sizes[Attrib::TEX_DATA]];
	int offset = 0;
	loadTexData(x, y, width, height, data, &offset);

	bindSpriteAttrib(Attrib::TEX_DATA, id, data);
}

void SimpleSprite::draw() const {
	GLuint tex = texture->getTexture();

	if (tex != currentTexture) {
		glBindTexture(GL_TEXTURE_RECTANGLE, tex);
		currentTexture = tex;
	}

	useShaders(vertexShader, geometryShader, fragmentShader);

	glStencilFunc(stencilFunc, stencilRef, stencilMask);
	glStencilOp(stencilFail, depthFail, depthPass);

	glDrawArrays(GL_POINTS, id, 1);
}
