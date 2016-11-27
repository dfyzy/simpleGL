#include <queue>
#include <set>

#include "simpleGL.hpp"
#include "simpleUtil.hpp"
#include "complexSprite.hpp"

const unsigned ComplexSprite::Attrib::sizes[5] = {2, 2, 1, 4, 4};

struct SpriteData {
	unsigned spriteId;
	std::array<float, 2 + 2 + 1 + 4 + 4> data;
};

//less
struct SpriteComparer {
	bool operator()(const ComplexSprite* lhs, const ComplexSprite* rhs) {
		if (lhs->getZ() == rhs->getZ()) {
			if (lhs->getTexture()->getTexture() == rhs->getTexture()->getTexture())
				return lhs != rhs;
			else
				return lhs->getTexture()->getTexture() < rhs->getTexture()->getTexture();
		} else
			return lhs->getZ() > rhs->getZ();
	}
};

namespace simpleUtil {

	GLuint vbos[ComplexSprite::Attrib::COUNT];

	boost::mutex spriteMutex;
	std::set<ComplexSprite*, SpriteComparer> sprites;

	boost::mutex allocationMutex;
	std::queue<SpriteData> spriteQueue;
	std::queue<unsigned> deletedQueue;

	boost::mutex changeMutex;
	std::queue<ComplexSprite::Attrib> changeQueue;

	unsigned spriteCount = 0;
	unsigned spriteCapacity = 4;

	void loadVector(SimpleVector sv, float* array, int* offset) {
		array[(*offset)++] = sv.x;
		array[(*offset)++] = sv.y;
	}

	void loadPosition(SimpleVector position, float* array, int* offset) {

		loadVector(simpleGL::toScreenCoord(position), array, offset);
	}

	void loadBounds(SimpleVector bounds, SimpleTexture* tex, float* array, int* offset) {

		loadVector(simpleGL::toScreenCoord(bounds*SimpleVector(tex->getWidth(), tex->getHeight())), array, offset);
	}

	void loadRotation(float rotation, float* array, int* offset) {
		array[(*offset)++] = rotation;
	}

	void loadColor(SimpleColor c, float* array, int* offset) {
		array[(*offset)++] = c.r;
		array[(*offset)++] = c.g;
		array[(*offset)++] = c.b;
		array[(*offset)++] = c.a;
	}

	void loadTexData(float x, float y, float width, float height, float* array, int* offset) {
		array[(*offset)++] = x + width*0.5f;
		array[(*offset)++] = y + height*0.5f;
		array[(*offset)++] = width;
		array[(*offset)++] = height;
	}

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
		print("Buffers initialized");
	}

	inline void bindSpriteAttrib(ComplexSprite::Attrib::E type, unsigned offset, float* data) {
		glBindBuffer(GL_ARRAY_BUFFER, vbos[type]);

		int size = ComplexSprite::Attrib::sizes[type] * sizeof(float);
		glBufferSubData(GL_ARRAY_BUFFER, offset * size, size, data);
	}

	void loadSprites() {
		boost::lock_guard<boost::mutex> lock(allocationMutex);

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
				print("Adding sprite");

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

	void drawSprites() {
		boost::lock_guard<boost::mutex> lock(spriteMutex);

		for (ComplexSprite* cs : sprites)
			cs->draw();

	}

}

using namespace simpleUtil;

SimpleSprite* simpleGL::loadSprite(SimpleTexture* tex, SimpleVector position, int z, SimpleVector bounds, float rotation, SimpleColor c,
															float texX, float texY, float texW, float texH) {
	SpriteData data;

	//not actualy loading anything into sprite object. just wanted for these functions to be in ComplexSprite class.
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

	ComplexSprite* sprite = new ComplexSprite(id, z, tex);
	setDefaultShaders(sprite, tex->getTexture() == 0);

	boost::lock_guard<boost::mutex> lock(spriteMutex);
	sprites.insert(sprite);

	return sprite;
}

ComplexSprite::~ComplexSprite() {
	simpleUtil::print("Sprite destructor");
	unload();
}

//TODO: before allocation
void ComplexSprite::unload() {
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

void ComplexSprite::resort() {
	boost::lock_guard<boost::mutex> lock(spriteMutex);

	sprites.erase(this);
	sprites.insert(this);
}

void ComplexSprite::draw() const {
	boost::lock_guard<boost::mutex> lock(mutex);

	GLuint tex = texture->getTexture();
	GLint current = 0;
	glGetIntegerv(GL_TEXTURE_BINDING_RECTANGLE, &current);

	if (tex != (GLuint) current)
		glBindTexture(GL_TEXTURE_RECTANGLE, texture->getTexture());

	if (enabled && id < spriteCapacity) {
		useShaders(vertexShader, geometryShader, fragmentShader);

		glStencilFunc(stencilFunc, stencilRef, stencilMask);
		glStencilOp(stencilFail, depthFail, depthPass);

		glDrawArrays(GL_POINTS, id, 1);
	}
}

void ComplexSprite::setAttrib(Attrib att) const {
	att.spriteId = id;

	boost::lock_guard<boost::mutex> lock(changeMutex);
	changeQueue.push(std::move(att));
}

void ComplexSprite::setPosition(SimpleVector position) {
	Attrib att(Attrib::POSITION);
	int offset = 0;
	loadPosition(position, att.data.get(), &offset);

	setAttrib(std::move(att));
}

void ComplexSprite::setBounds(SimpleVector bounds) {
	Attrib att(Attrib::BOUNDS);
	int offset = 0;
	loadBounds(bounds, texture, att.data.get(), &offset);

	setAttrib(std::move(att));
}

void ComplexSprite::setRotation(float rotation) {
	Attrib att(Attrib::ROTATION);
	int offset = 0;
	loadRotation(rotation, att.data.get(), &offset);

	setAttrib(std::move(att));
}

void ComplexSprite::setColor(SimpleColor c) {
	Attrib att(Attrib::COLOR);
	int offset = 0;
	loadColor(c, att.data.get(), &offset);

	setAttrib(std::move(att));
}

void ComplexSprite::setTexData(float x, float y, float width, float height) {
	Attrib att(Attrib::TEX_DATA);
	int offset = 0;
	loadTexData(x, y, width, height, att.data.get(), &offset);

	setAttrib(std::move(att));
}
