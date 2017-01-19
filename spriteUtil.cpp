#include <queue>
#include <set>

#include "simpleGL.hpp"
#include "simpleUtil.hpp"
#include <iostream>
struct Attrib {
	enum E { POSITION, BOUNDS, ROTATION, COLOR, TEX_DATA, COUNT } type;
	static const unsigned sizes[5];
};

const unsigned Attrib::sizes[5] = {2, 2, 1, 4, 4};

namespace simpleUtil {
	const int SPRITE_SIZE = 2 + 2 + 1 + 4 + 4;

	GLuint msaaFbo;
	GLuint rectFbo;

	GLuint vbos[Attrib::COUNT];

	GLuint currentTexture = 0;

	std::set<SimpleSprite*, SimpleSprite::Comparer> sprites;

	std::queue<unsigned> deletedQueue;

	unsigned spriteCount = 0;
	unsigned spriteCapacity = 4;

	inline void loadVector(SimpleVector sv, float* array, int* offset) {
		array[(*offset)++] = sv.x;
		array[(*offset)++] = sv.y;
	}

	inline void loadPosition(SimpleVector position, float* array, int* offset) {

		loadVector(position, array, offset);
	}

	inline void loadBounds(SimpleVector bounds, float* array, int* offset) {

		loadVector(bounds, array, offset);
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

	inline void loadTexData(SimpleVector texPosition, SimpleVector texBounds, float* array, int* offset) {
		loadVector(texPosition + texBounds*0.5f, array, offset);
		loadVector(texBounds, array, offset);
	}

	GLuint getMsaaFbo() {
		return msaaFbo;
	}

	void initBuffers() {
		glGenBuffers(Attrib::COUNT, vbos);

		for (int i = 0; i < Attrib::COUNT; i++) {
			glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);

			//alocating data for spriteCapacity number of sprites for a start.
			glBufferData(GL_ARRAY_BUFFER, spriteCapacity * Attrib::sizes[i] * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

			//binding buffers to layout locations in vertex shader.
			glVertexAttribPointer(i + 1, Attrib::sizes[i], GL_FLOAT, GL_FALSE, 0, nullptr);
			glEnableVertexAttribArray(i + 1);
		}

		print("Buffers initialized");
	}

	void initFbos() {
		glGenFramebuffers(1, &msaaFbo);
		glBindFramebuffer(GL_FRAMEBUFFER, msaaFbo);

		GLuint renderbuffer;
		glGenRenderbuffers(1, &renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);

		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, simpleGL::getWindowWidth(), simpleGL::getWindowHeight());

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);

		glGenFramebuffers(1, &rectFbo);
		glBindFramebuffer(GL_FRAMEBUFFER, rectFbo);

		glActiveTexture(GL_TEXTURE1);

		SimpleTexture texture(simpleGL::getWindowWidth(), simpleGL::getWindowHeight(), GL_RGB);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture.getTexture(), 0);

		glActiveTexture(GL_TEXTURE0);

		GLuint fboVbo;
		glGenBuffers(1, &fboVbo);
		glBindBuffer(GL_ARRAY_BUFFER, fboVbo);

		float width = simpleGL::getWindowWidth(), height = simpleGL::getWindowHeight();
		float data[] {-width, height,
							-width, -height,
							width, height,
							width, -height};
		glBufferData(GL_ARRAY_BUFFER, 8*sizeof(float), data, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(0);

		print("Fbos initialized");
	}

	inline void bindSpriteAttrib(Attrib::E type, unsigned offset, float* data) {
		glBindBuffer(GL_ARRAY_BUFFER, vbos[type]);

		int size = Attrib::sizes[type] * sizeof(float);
		glBufferSubData(GL_ARRAY_BUFFER, offset * size, size, data);
	}

	void drawSprites() {
		unsigned width = simpleGL::getWindowWidth(), height = simpleGL::getWindowHeight();

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, msaaFbo);

		glClear(GL_COLOR_BUFFER_BIT);

		for (SimpleSprite* cs : sprites)
			cs->draw();

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rectFbo);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaFbo);

		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		useOverlayShaders();

		glDisable(GL_BLEND);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glEnable(GL_BLEND);
	}

}

using namespace simpleUtil;

SimplerSprite::SimplerSprite(GLuint textureId, SimpleVector position, SimpleVector bounds, float rotation,
										SimpleColor color, SimpleVector texPosition, SimpleVector texBounds) : textureId(textureId) {
	print("Adding sprite");

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

	float data[SPRITE_SIZE];
	int offset = 0;
	loadPosition(position, data, &offset);
	loadBounds(bounds, data, &offset);
	loadRotation(rotation, data, &offset);
	loadColor(color, data, &offset);
	loadTexData(texPosition, texBounds, data, &offset);

	int dataOffset = 0;
	for (int i = 0; i < Attrib::COUNT; i++) {
		bindSpriteAttrib(static_cast<Attrib::E>(i), id, data + dataOffset);

		dataOffset += Attrib::sizes[i];
	}

	setDefaultShaders(this, textureId == 0);
}

SimpleSprite::SimpleSprite(GLuint texture, SimpleVector position, int z, SimpleVector bounds, float rotation,
										SimpleColor color, SimpleVector texPosition, SimpleVector texBounds)
										: SimplerSprite(texture, position, bounds, rotation, color, texPosition, texBounds), z(z) {
	sprites.insert(this);
}

SimplerSprite::~SimplerSprite() {
	print("Unloading sprite");

	if (id < spriteCount - 1)
		deletedQueue.push(id);
	else
		spriteCount--;
}

SimpleSprite::~SimpleSprite() {
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

void SimpleSprite::setTextureId(GLuint tex) {
	sprites.erase(this);
	textureId = tex;
	sprites.insert(this);
}

void SimplerSprite::setPosition(SimpleVector position) const {
	float data[Attrib::sizes[Attrib::POSITION]];
	int offset = 0;
	loadPosition(position, data, &offset);

	bindSpriteAttrib(Attrib::POSITION, id, data);
}

void SimplerSprite::setBounds(SimpleVector bounds) const {
	float data[Attrib::sizes[Attrib::BOUNDS]];
	int offset = 0;
	loadBounds(bounds, data, &offset);

	bindSpriteAttrib(Attrib::BOUNDS, id, data);
}

void SimplerSprite::setRotation(float rotation) const {
	float data[Attrib::sizes[Attrib::ROTATION]];
	int offset = 0;
	loadRotation(rotation, data, &offset);

	bindSpriteAttrib(Attrib::ROTATION, id, data);
}

void SimplerSprite::setColor(SimpleColor c) const {
	float data[Attrib::sizes[Attrib::COLOR]];
	int offset = 0;
	loadColor(c, data, &offset);

	bindSpriteAttrib(Attrib::COLOR, id, data);
}

void SimplerSprite::setTexData(SimpleVector texPosition, SimpleVector texBounds) const {
	float data[Attrib::sizes[Attrib::TEX_DATA]];
	int offset = 0;
	loadTexData(texPosition, texBounds, data, &offset);

	bindSpriteAttrib(Attrib::TEX_DATA, id, data);
}

void SimplerSprite::draw() {
	if (textureId != currentTexture) {
		glBindTexture(GL_TEXTURE_RECTANGLE, textureId);
		currentTexture = textureId;
	}

	useShaders(vertexShader, geometryShader, fragmentShader);

	glDrawArrays(GL_POINTS, id, 1);
}
