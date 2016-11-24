#include <queue>

#include "simpleUtil.hpp"
#include "complexTexture.hpp"

const unsigned ComplexSprite::Attrib::sizes[5] = {3, 2, 1, 4, 4};

void ComplexSprite::loadPosition(SimplePosition sp, float* array, int offset) {
	array[0 + offset] = simpleGL::toScreenCoord(sp.x);
	array[1 + offset] = simpleGL::toScreenCoord(sp.y);
	array[2 + offset] = sp.z*ZPOINT;
}

void ComplexSprite::loadBounds(float width, float height, float* array, int offset) {
	array[0 + offset] = simpleGL::toScreenCoord(width*texture->getWidth());
	array[1 + offset] = simpleGL::toScreenCoord(height*texture->getHeight());
}

void ComplexSprite::loadRotation(float rotation, float* array, int offset) {
	array[0 + offset] = rotation;
}

void ComplexSprite::loadColor(SimpleColor c, float* array, int offset) {
	array[0 + offset] = c.r;
	array[1 + offset] = c.g;
	array[2 + offset] = c.b;
	array[3 + offset] = c.a;
}

void ComplexSprite::loadTexData(float x, float y, float width, float height, float* array, int offset) {
	array[0 + offset] = x + width*0.5f;
	array[1 + offset] = y + height*0.5f;
	array[2 + offset] = width;
	array[3 + offset] = height;
}

struct SpriteData {
	unsigned spriteId;
	std::array<float, 3 + 2 + 1 + 4 + 4> data;
};

namespace simpleUtil {

	GLuint vbos[ComplexSprite::Attrib::COUNT];

	boost::mutex spriteMutex;
	std::queue<SpriteData> spriteQueue;
	std::queue<unsigned> deletedQueue;

	boost::mutex changeMutex;
	std::queue<ComplexSprite::Attrib> changeQueue;

	unsigned spriteCount = 0;
	unsigned spriteCapacity = 4;

	void initBuffers() {
		glGenBuffers(ComplexSprite::Attrib::COUNT, vbos);

		for (int i = 0; i < ComplexSprite::Attrib::COUNT; i++) {
			glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);

			//alocating data for spriteCapacity number of sprites for a start.
			glBufferData(GL_ARRAY_BUFFER, spriteCapacity * ComplexSprite::Attrib::sizes[i] * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

			//binding buffers to layout locations in vertex shader.
			glVertexAttribPointer(i, ComplexSprite::Attrib::sizes[i], GL_FLOAT, GL_FALSE, 0, nullptr);
			glEnableVertexAttribArray(i);
		}

	}

	inline void bindSpriteAttrib(ComplexSprite::Attrib::E type, unsigned offset, float* data) {
		glBindBuffer(GL_ARRAY_BUFFER, vbos[type]);

		int size = ComplexSprite::Attrib::sizes[type] * sizeof(float);
		glBufferSubData(GL_ARRAY_BUFFER, offset * size, size, data);
	}

	void loadSprites() {
		boost::lock_guard<boost::mutex> lock(spriteMutex);

		if (!spriteQueue.empty()) {
			if (spriteCapacity < spriteCount) {
				print("Resizing data buffers");

				GLuint tempVbo;
				glGenBuffers(1, &tempVbo);

				for (int i = 0; i < ComplexSprite::Attrib::COUNT; i++) {
					int size = ComplexSprite::Attrib::sizes[i] * sizeof(float);
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
	}

	void changeSprites() {
		boost::lock_guard<boost::mutex> lock(changeMutex);

		while (!changeQueue.empty()) {
			ComplexSprite::Attrib change = std::move(changeQueue.front());
			changeQueue.pop();

			bindSpriteAttrib(change.type, change.spriteId, change.data.get());
		}
	}

	void checkSprites() {
		loadSprites();

		changeSprites();
	}

}

using namespace simpleUtil;

SimpleSprite* ComplexTexture::loadSprite(SimplePosition sp, float width, float height, float rotation, SimpleColor c,
															float texX, float texY, float texW, float texH) {
	SpriteData data;

	ComplexSprite sprite(this);
	//not actualy loading anything into sprite object. just wanted for these functions to be in ComplexSprite class.
	sprite.loadPosition(sp, data.data.data(), 0);
	sprite.loadBounds(width, height, data.data.data(), 3);
	sprite.loadRotation(rotation, data.data.data(), 5);
	sprite.loadColor(c, data.data.data(), 6);
	sprite.loadTexData(texX, texY, texW, texH, data.data.data(), 10);

	boost::lock_guard<boost::mutex> lock(spriteMutex);
	unsigned id;
	if (!deletedQueue.empty()) {
		id = deletedQueue.front();
		deletedQueue.pop();
	}else	id = spriteCount++;

	data.spriteId = id;
	spriteQueue.push(data);

	sprites.emplace_back(id, this);

	SimpleSprite* value = &(*--sprites.end());
	setDefaultShaders(value, texture == 0);

	return value;
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

	for (ComplexSprite& cs : sprites)
		cs.draw();
}

void ComplexSprite::draw() {
	boost::lock_guard<boost::mutex> lock(mutex);

	if (enabled && id < spriteCapacity) {
		useShaders(vertexShader, geometryShader, fragmentShader);

		glDrawArrays(GL_POINTS, id, 1);
	}
}

void ComplexSprite::changeAttrib(Attrib att) {
	att.spriteId = id;

	boost::lock_guard<boost::mutex> lock(changeMutex);
	changeQueue.push(std::move(att));
}
