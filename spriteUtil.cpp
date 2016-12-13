#include <queue>
#include <set>

#include "simpleGL.hpp"
#include "simpleUtil.hpp"
#include "SimpleSprite.hpp"

struct Attrib {
	enum E { POSITION, BOUNDS, ROTATION, COLOR, TEX_DATA, COUNT } type;
	static const unsigned sizes[5];

	std::unique_ptr<float> data;
	unsigned spriteId;

	Attrib(E t) : type(t) {
		data = std::unique_ptr<float>(new float[sizes[t]]);
	}
};

const unsigned Attrib::sizes[5] = {2, 2, 1, 4, 4};

struct SpriteData {
	unsigned spriteId;
	std::array<float, 2 + 2 + 1 + 4 + 4> data;
};

namespace simpleUtil {

	GLuint vbos[Attrib::COUNT];

	GLuint currentTexture = 0;

	boost::mutex spriteMutex;
	std::set<SimpleSprite*, SimpleSprite::Comparer> sprites;

	boost::mutex allocationMutex;
	std::queue<SpriteData> spriteQueue;
	std::queue<unsigned> deletedQueue;

	boost::mutex attribMutex;
	std::queue<Attrib> attribQueue;

	unsigned spriteCount = 0;
	unsigned spriteCapacity = 4;

	inline void loadVector(SimpleVector sv, float* array, int* offset) {
		array[(*offset)++] = sv.x;
		array[(*offset)++] = sv.y;
	}

	inline void loadPosition(SimpleVector position, float* array, int* offset) {

		loadVector(simpleGL::toScreenCoords(position), array, offset);
	}

	inline void loadBounds(SimpleVector bounds, SimpleTexture* tex, float* array, int* offset) {

		loadVector(simpleGL::toScreenCoords(bounds*SimpleVector(tex->getWidth(), tex->getHeight())), array, offset);
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

	void setAttrib(Attrib attrib, int id) {
		attrib.spriteId = id;

		boost::lock_guard<boost::mutex> lock(attribMutex);
		attribQueue.push(std::move(attrib));
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

	void loadSprites() {
		boost::lock_guard<boost::mutex> lock(allocationMutex);

		if (!spriteQueue.empty()) {
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

			while (!spriteQueue.empty()) {
				print("Adding sprite");

				SpriteData data = spriteQueue.front();
				spriteQueue.pop();

				int dataOffset = 0;
				for (int i = 0; i < Attrib::COUNT; i++) {
					bindSpriteAttrib(static_cast<Attrib::E>(i), data.spriteId, data.data.data() + dataOffset);

					dataOffset += Attrib::sizes[i];
				}
			}
		}
	}

	void changeSprites() {
		boost::lock_guard<boost::mutex> lock(attribMutex);

		while (!attribQueue.empty()) {
			Attrib change = std::move(attribQueue.front());
			attribQueue.pop();

			bindSpriteAttrib(change.type, change.spriteId, change.data.get());
		}
	}

	void checkSprites() {

		loadSprites();
		changeSprites();

	}

	void drawSprites() {
		boost::lock_guard<boost::mutex> lock(spriteMutex);

		for (SimpleSprite* cs : sprites)
			cs->draw();

	}

}

using namespace simpleUtil;

SimpleSprite* simpleGL::loadSprite(SimpleTexture* tex, SimpleVector position, int z, SimpleVector bounds, float rotation, SimpleColor c,
															float texX, float texY, float texW, float texH) {
	SpriteData data;

	//not actualy loading anything into sprite object. just wanted for these functions to be in SimpleSprite class.
	int offset = 0;
	loadPosition(position, data.data.data(), &offset);
	loadBounds(bounds, tex, data.data.data(), &offset);
	loadRotation(rotation, data.data.data(), &offset);
	loadColor(c, data.data.data(), &offset);
	loadTexData(texX, texY, texW, texH, data.data.data(), &offset);

	unsigned id;
	{
		boost::lock_guard<boost::mutex> lock(allocationMutex);
		if (!deletedQueue.empty()) {
			id = deletedQueue.front();
			deletedQueue.pop();
		}else	id = spriteCount++;

		data.spriteId = id;
		spriteQueue.push(data);
	}

	SimpleSprite* sprite = new SimpleSprite(id, z, tex);
	setDefaultShaders(sprite, tex->getTexture() == 0);

	boost::lock_guard<boost::mutex> lock(spriteMutex);
	sprites.insert(sprite);

	return sprite;
}

//TODO: before allocation
SimpleSprite::~SimpleSprite() {
	print("Unloading sprite");

	{
		boost::lock_guard<boost::mutex> lock(allocationMutex);
		if (id < spriteCount - 1)
			deletedQueue.push(id);
		else
			spriteCount--;
	}

	boost::lock_guard<boost::mutex> lock(spriteMutex);
	sprites.erase(this);
}

void SimpleSprite::setZ(int pz) {
	boost::lock_guard<boost::mutex> lock(spriteMutex);

	sprites.erase(this);

	mutex.lock();
	z = pz;
	mutex.unlock();

	sprites.insert(this);
}

void SimpleSprite::setTexture(SimpleTexture* tex) {
	boost::lock_guard<boost::mutex> lock(spriteMutex);

	sprites.erase(this);

	mutex.lock();
	texture = tex;
	mutex.unlock();

	sprites.insert(this);
}

void SimpleSprite::draw() const {
	boost::lock_guard<boost::mutex> lock(mutex);

	GLuint tex = texture->getTexture();
	if (tex != currentTexture) {
		glBindTexture(GL_TEXTURE_RECTANGLE, tex);
		currentTexture = tex;
	}

	if (enabled && id < spriteCapacity) {
		useShaders(vertexShader, geometryShader, fragmentShader);

		glStencilFunc(stencilFunc, stencilRef, stencilMask);
		glStencilOp(stencilFail, depthFail, depthPass);

		glDrawArrays(GL_POINTS, id, 1);
	}
}

void SimpleSprite::setPosition(SimpleVector position) {
	Attrib attrib(Attrib::POSITION);
	int offset = 0;
	loadPosition(position, attrib.data.get(), &offset);

	setAttrib(std::move(attrib), id);
}

void SimpleSprite::setBounds(SimpleVector bounds) {
	Attrib attrib(Attrib::BOUNDS);
	int offset = 0;
	loadBounds(bounds, texture, attrib.data.get(), &offset);

	setAttrib(std::move(attrib), id);
}

void SimpleSprite::setRotation(float rotation) {
	Attrib attrib(Attrib::ROTATION);
	int offset = 0;
	loadRotation(rotation, attrib.data.get(), &offset);

	setAttrib(std::move(attrib), id);
}

void SimpleSprite::setColor(SimpleColor c) {
	Attrib attrib(Attrib::COLOR);
	int offset = 0;
	loadColor(c, attrib.data.get(), &offset);

	setAttrib(std::move(attrib), id);
}

void SimpleSprite::setTexData(float x, float y, float width, float height) {
	Attrib attrib(Attrib::TEX_DATA);
	int offset = 0;
	loadTexData(x, y, width, height, attrib.data.get(), &offset);

	setAttrib(std::move(attrib), id);
}
