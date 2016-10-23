#include <queue>
#include <boost/thread.hpp>

#include "complexTexture.hpp"
#include "simpleUtil.hpp"

const unsigned ComplexSprite::Attrib::sizes[4] = {3, 2, 4, 4};

struct SpriteData {
	unsigned spriteId;
	std::array<float, 3 + 2 + 4 + 4> data;
};

unsigned resWidth, resHeight;
const float ZPOINT = 0.0001f;

GLuint vbos[ComplexSprite::Attrib::COUNT];

boost::mutex spriteMutex;
std::queue<SpriteData> spriteQueue;
std::queue<unsigned> deletedQueue;

boost::mutex changeMutex;
std::queue<ComplexSprite::Attrib> changeQueue;

boost::mutex enableMutex;

unsigned spriteCount = 0;
unsigned spriteCapacity = 0;

void SimpleSprite::setEnabled(bool b) {
	boost::lock_guard<boost::mutex> lock(enableMutex);

	enabled = b;
}

bool SimpleSprite::isEnabled() {
	boost::lock_guard<boost::mutex> lock(enableMutex);

	return enabled;
}

void simpleUtil::initBuffers() {
	GLuint instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

	float instanceData [] {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f};//the quad I'm going to draw instances of

	glBufferData(GL_ARRAY_BUFFER, sizeof(instanceData), instanceData, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	glGenBuffers(ComplexSprite::Attrib::COUNT, vbos);

	for (int i = 0; i < ComplexSprite::Attrib::COUNT; i++) {
		glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);

		glVertexAttribPointer(i + 1, ComplexSprite::Attrib::sizes[i], GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(i + 1);
		glVertexAttribDivisor(i + 1, 1);
	}

	for (int i = 0; i < ComplexSprite::Attrib::COUNT; i++) {
		glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
		glBufferData(GL_ARRAY_BUFFER, 10 * ComplexSprite::Attrib::sizes[i] * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
	}
	spriteCapacity = 10;
}

void bindSpriteAttrib(ComplexSprite::Attrib::E type, unsigned offset, float* data) {
	glBindBuffer(GL_ARRAY_BUFFER, vbos[type]);

	int size = ComplexSprite::Attrib::sizes[type] * sizeof(float);
	glBufferSubData(GL_ARRAY_BUFFER, offset * size, size, data);
}

void loadSprites() {
	/*if (spriteCapacity < spriteCount) {
		for (int i = 0; i < ComplexSprite::Attrib::COUNT; i++) {
			glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
			glBufferData(GL_ARRAY_BUFFER, spriteCount * ComplexSprite::Attrib::sizes[i] * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
		}
		spriteCapacity = spriteCount;
	}*/

	while (!spriteQueue.empty()) {
		SpriteData data = spriteQueue.front();
		spriteQueue.pop();

		int dataOffset = 0;
		for (int i = 0; i < ComplexSprite::Attrib::COUNT; i++) {
			bindSpriteAttrib(static_cast<ComplexSprite::Attrib::E>(i), data.spriteId, data.data.data() + dataOffset);

			dataOffset += ComplexSprite::Attrib::sizes[i];
		}
	}
}

void checkSpritQueue() {
	boost::lock_guard<boost::mutex> lock(spriteMutex);

	if (!spriteQueue.empty())	loadSprites();
}

void changeSprites() {
	while (!changeQueue.empty()) {
		ComplexSprite::Attrib change = std::move(changeQueue.front());
		changeQueue.pop();

		bindSpriteAttrib(change.type, change.spriteId, change.data.get());
	}
}

void checkChangeQueue() {
	boost::lock_guard<boost::mutex> lock(changeMutex);

	if (!changeQueue.empty())	changeSprites();
}

void simpleUtil::checkSprites() {
	checkSpritQueue();

	checkChangeQueue();
}

//hmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
void simpleUtil::setResolution(unsigned w, unsigned h) {
	resWidth = w;
	resHeight = h;
}

SimpleSprite* ComplexTexture::loadSprite(float x, float y, float z, float w, float h, Color c,
															float texX, float texY, float texW, float texH) {
	SpriteData data;
	data.data[0] = 2*x/resWidth;			data.data[1] = 2*y/resHeight;				data.data[2] = z*ZPOINT;
	data.data[3] = 2*w*width/resWidth;	data.data[4] = 2*h*height/resHeight;
	data.data[5] = c.r;						data.data[6] = c.g;							data.data[7] = c.b;			data.data[8] = c.a;
	data.data[9] = texX+texW/2;			data.data[10] = texY+texH/2;				data.data[11] = texW;		data.data[12] = texH;

	boost::lock_guard<boost::mutex> lock(spriteMutex);
	unsigned id;
	if (!deletedQueue.empty()) {
		id = deletedQueue.front();
		deletedQueue.pop();
	}else	id = spriteCount++;

	data.spriteId = id;
	spriteQueue.push(data);

	sprites.push_back(ComplexSprite(id, this));

	return &(*--sprites.end());
}

void ComplexSprite::unload() {
	boost::lock_guard<boost::mutex> lock(spriteMutex);

	if (id < spriteCount - 1)
		deletedQueue.push(id);
	else
		spriteCount--;

	texture->removeSprite(this);
}

void ComplexTexture::draw() {
	glBindTexture(GL_TEXTURE_RECTANGLE, texture);

	for (ComplexSprite cs : sprites) {
		unsigned id = cs.getId();

		if (cs.isEnabled() && id < spriteCapacity)
			glDrawArraysInstancedBaseInstance(GL_TRIANGLE_STRIP, 0, 4, 1, id);

	}
}

void ComplexSprite::changeAttrib(Attrib att) {
	boost::lock_guard<boost::mutex> lock(changeMutex);

	changeQueue.push(std::move(att));
}

void ComplexSprite::changePosition(float x, float y, float z) {
	Attrib att(Attrib::POSITION, id);
	att.data.get()[0] = 2*x/resWidth;	att.data.get()[1] = 2*y/resHeight;	att.data.get()[2] = z*ZPOINT;
	changeAttrib(std::move(att));
}

void ComplexSprite::changeBounds(float width, float height) {
	Attrib att(Attrib::BOUNDS, id);
	att.data.get()[0] = 2*width*texture->getWidth()/resWidth;	att.data.get()[1] = 2*height*texture->getHeight()/resHeight;
	changeAttrib(std::move(att));
}

void ComplexSprite::changeColor(Color c) {
	Attrib att(Attrib::COLOR, id);
	att.data.get()[0] = c.r;	att.data.get()[1] = c.g;	att.data.get()[2] = c.b;	att.data.get()[3] = c.a;
	changeAttrib(std::move(att));
}

void ComplexSprite::changeTexData(float texX, float texY, float texW, float texH) {
	Attrib att(Attrib::TEX_DATA, id);
	att.data.get()[0] = texX+texW/2;	att.data.get()[1] = texY+texH/2;	att.data.get()[2] = texW;	att.data.get()[3] = texH;
	changeAttrib(std::move(att));
}
