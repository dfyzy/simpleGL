#include <queue>
#include <boost/thread.hpp>

#include "simpleUtil.hpp"
#include "complexTexture.hpp"

const unsigned ComplexSprite::Attrib::sizes[4] = {3, 2, 4, 4};

struct SpriteData {
	unsigned spriteId;
	std::array<float, 3 + 2 + 4 + 4> data;
};

namespace simpleUtil {

	GLuint vbos[ComplexSprite::Attrib::COUNT];

	boost::mutex spriteMutex;
	std::queue<SpriteData> spriteQueue;
	std::queue<unsigned> deletedQueue;

	boost::mutex changeMutex;
	std::queue<ComplexSprite::Attrib> changeQueue;

	boost::mutex enableMutex;

	unsigned spriteCount = 0;
	unsigned spriteCapacity = 0;

}

using namespace simpleUtil;

void SimpleSprite::setEnabled(bool b) {
	boost::lock_guard<boost::mutex> lock(enableMutex);

	enabled = b;
}

bool SimpleSprite::isEnabled() {
	boost::lock_guard<boost::mutex> lock(enableMutex);

	return enabled;
}

namespace simpleUtil {

	void initBuffers() {
		GLuint instanceVbo;
		glGenBuffers(1, &instanceVbo);
		glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);

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

		spriteCapacity = 5;
		for (int i = 0; i < ComplexSprite::Attrib::COUNT; i++) {
			glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
			glBufferData(GL_ARRAY_BUFFER, spriteCapacity * ComplexSprite::Attrib::sizes[i] * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
		}
	}

	inline void bindSpriteAttrib(ComplexSprite::Attrib::E type, unsigned offset, float* data) {
		glBindBuffer(GL_ARRAY_BUFFER, vbos[type]);

		int size = ComplexSprite::Attrib::sizes[type] * sizeof(float);
		glBufferSubData(GL_ARRAY_BUFFER, offset * size, size, data);
	}

	void loadSprites() {
		if (spriteCapacity < spriteCount) {
			print("Resizing data buffers");

			GLuint tempVbo;
			glGenBuffers(1, &tempVbo);

			for (int i = 0; i < ComplexSprite::Attrib::COUNT; i++) {
				int size = ComplexSprite::Attrib::sizes[i] * sizeof(float);

				glBindBuffer(GL_COPY_WRITE_BUFFER, tempVbo);
				glBufferData(GL_COPY_WRITE_BUFFER, spriteCapacity * size, nullptr, GL_DYNAMIC_DRAW);

				glBindBuffer(GL_COPY_READ_BUFFER, vbos[i]);
				glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, spriteCapacity * size);

				glBindBuffer(GL_COPY_WRITE_BUFFER, vbos[i]);
				glBufferData(GL_COPY_WRITE_BUFFER, spriteCount * size, nullptr, GL_DYNAMIC_DRAW);

				glBindBuffer(GL_COPY_READ_BUFFER, tempVbo);
				glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, spriteCapacity * size);

				// TODO: check if one copy/attribpointer is faster than copy/copy
				// GLuint t = vbos[i];
				// vbos[i] = tempVbo;
				// tempVbo = t;
			}
			glDeleteBuffers(1, &tempVbo);

			spriteCapacity = spriteCount;
		}

		while (!spriteQueue.empty()) {
			print ("Adding sprite");

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

	void checkSprites() {
		checkSpritQueue();

		checkChangeQueue();
	}

}


SimpleSprite* ComplexTexture::loadSprite(SimplePosition sp, float w, float h, SimpleColor c,
															float texX, float texY, float texW, float texH) {
	SpriteData data;

	loadPosition(sp, data.data.data());//I know, right?
	loadBounds(w, h, data.data.data(), 3);
	loadColor(c, data.data.data(), 5);
	loadTexData(texX, texY, texW, texH, data.data.data(), 9);

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

void ComplexSprite::deleteData() {
	if (id < spriteCount - 1)
		deletedQueue.push(id);
	else
		spriteCount--;
}

void ComplexSprite::unload() {
	boost::lock_guard<boost::mutex> lock(spriteMutex);

	deleteData();

	texture->removeSprite(this);
}

void ComplexTexture::draw() {
	glBindTexture(GL_TEXTURE_RECTANGLE, texture);

	boost::lock_guard<boost::mutex> lock(spriteMutex);

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

void ComplexSprite::changePosition(SimplePosition sp) {
	Attrib att(Attrib::POSITION, id);
	texture->loadPosition(sp, att.data.get());

	changeAttrib(std::move(att));
}

void ComplexSprite::changeBounds(float width, float height) {
	Attrib att(Attrib::BOUNDS, id);
	texture->loadBounds(width, height, att.data.get(), 0);

	changeAttrib(std::move(att));
}

void ComplexSprite::changeColor(SimpleColor c) {
	Attrib att(Attrib::COLOR, id);
	texture->loadColor(c, att.data.get(), 0);

	changeAttrib(std::move(att));
}

void ComplexSprite::changeTexData(float x, float y, float w, float h) {
	Attrib att(Attrib::TEX_DATA, id);
	texture->loadTexData(x, y, w, h, att.data.get(), 0);

	changeAttrib(std::move(att));
}
