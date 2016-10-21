#include <queue>
#include <boost/thread.hpp>

#include "complexTexture.hpp"
#include "simpleUtil.hpp"

struct Attribs { enum E { POSITION, BOUNDS, COLOR, TEX_DATA, COUNT }; };
const int attribsSizes[] = {3, 2, 4, 4};

typedef std::array<float, 3 + 2 + 4 + 4> SpriteData;

unsigned resWidth, resHeight;
const float ZPOINT = 0.0001f;

GLuint vbos[Attribs::COUNT];

boost::mutex spriteMutex;

std::queue<SpriteData> spriteQueue;

unsigned spriteCount = 0;
unsigned spriteCapacity = 0;

void simpleUtil::initBuffers() {
	GLuint instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

	float instanceData [] {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f};//the quad I'm going to draw instances of

	glBufferData(GL_ARRAY_BUFFER, sizeof(instanceData), instanceData, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	glGenBuffers(Attribs::COUNT, vbos);

	for (int i = 0; i < Attribs::COUNT; i++) {
		glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);

		glVertexAttribPointer(i + 1, attribsSizes[i], GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(i + 1);
		glVertexAttribDivisor(i + 1, 1);
	}
}

void loadSprites() {
	for (int i = 0; i < Attribs::COUNT; i++) {
		glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
		glBufferData(GL_ARRAY_BUFFER, spriteCount * attribsSizes[i] * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
	}
	spriteCapacity = spriteCount;

	while (!spriteQueue.empty()) {
		int position = spriteCount - spriteQueue.size();

		SpriteData data = spriteQueue.front();
		spriteQueue.pop();

		int dataOffset = 0;
		for (int i = 0; i < Attribs::COUNT; i++) {
			glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
			glBufferSubData(GL_ARRAY_BUFFER, position * attribsSizes[i] * sizeof(float),
																		attribsSizes[i] * sizeof(float), data.data() + dataOffset);

			dataOffset += attribsSizes[i];
		}
	}
}

void simpleUtil::checkSprites() {
	spriteMutex.lock();
	bool empty = spriteQueue.size() == 0;
	spriteMutex.unlock();

	if (!empty)	loadSprites();
}

//hmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
void simpleUtil::setResolution(unsigned w, unsigned h) {
	resWidth = w;
	resHeight = h;
}

SimpleSprite* ComplexTexture::createSprite(float x, float y, float z, float w, float h, Color c,
															float texX, float texY, float texW, float texH) {
	SpriteData data;
	data[0] = 2*x/resWidth;	data[1] = 2*y/resHeight;	data[2] = z * ZPOINT;
	data[3] = 2*w/resWidth;	data[4] = 2*h/resHeight;
	data[5] = c.r;				data[6] = c.g;					data[7] = c.b;				data[8] = c.a;
	data[9] = texX+texW/2;	data[10] = texY+texH/2;		data[11] = texW;			data[12] = texH;

	boost::lock_guard<boost::mutex> lock(spriteMutex);
	spriteQueue.push(data);

	sprites.push_back(SimpleSprite(spriteCount++));

	return &(*--sprites.end());
}

void ComplexTexture::draw() {
	glBindTexture(GL_TEXTURE_RECTANGLE, texture);

	for (SimpleSprite ss : sprites) {
		unsigned id = ss.getId();

		if (ss.isEnabled() && id < spriteCapacity)
			glDrawArraysInstancedBaseInstance(GL_TRIANGLE_STRIP, 0, 4, 1, id);

	}
}
