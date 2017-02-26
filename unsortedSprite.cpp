#include <queue>

#include "simpleGL.hpp"
#include "simpleUtil.hpp"

using namespace simpleGL;

namespace simpleUtil {
	const int SPRITE_VERTS = 8;
	const int SPRITE_SIZE = SPRITE_VERTS * sizeof(float);

	const float quad[SPRITE_VERTS] = {-0.5, 0.5,
												-0.5, -0.5,
												0.5, 0.5,
												0.5, -0.5};

	struct vboType { enum E { VERTEX, TEXTURE, COUNT }; };
	GLuint vbos[vboType::COUNT];

	std::queue<unsigned> deletedQueue;

	unsigned spriteCount = 0;
	unsigned spriteCapacity = 4;

	inline void loadVector(Vector sv, float* array, int* offset) {
		array[(*offset)++] = sv.x;
		array[(*offset)++] = sv.y;
	}

	void initBuffers() {
		glGenBuffers(vboType::COUNT, vbos);

		for (int i = 0; i < vboType::COUNT; i++) {
			glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);

			//alocating data for spriteCapacity number of sprites for a start.
			glBufferData(GL_ARRAY_BUFFER, spriteCapacity * SPRITE_SIZE, nullptr, GL_DYNAMIC_DRAW);

			//binding buffers to layout locations in vertex shader.
			glVertexAttribPointer(i + 1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
			glEnableVertexAttribArray(i + 1);
		}

		print("Buffers initialized");
	}

}

using namespace simpleUtil;

UnsortedSprite::UnsortedSprite(Texture texture, Vector position, Vector scale, float rotation, Color color)
											: texture(texture), position(position), scale(scale), color(color) {
	print("Adding sprite");

	if (!deletedQueue.empty()) {
		id = deletedQueue.front();
		deletedQueue.pop();
	}else	id = spriteCount++;

	if (spriteCapacity < spriteCount) {
		print("Resizing data buffers");

		GLuint tempVbo;
		glGenBuffers(1, &tempVbo);

		int oldSize = spriteCapacity * SPRITE_SIZE;
		glBindBuffer(GL_COPY_WRITE_BUFFER, tempVbo);
		glBufferData(GL_COPY_WRITE_BUFFER, oldSize, nullptr, GL_DYNAMIC_DRAW);

		for (int i = 0; i < vboType::COUNT; i++) {
			//loading current data into temp buffer, resizing this buffer and loading data back from temp buffer.
			glBindBuffer(GL_COPY_READ_BUFFER, vbos[i]);

			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, oldSize);

			glBufferData(GL_COPY_READ_BUFFER, 2 * oldSize, nullptr, GL_DYNAMIC_DRAW);

			glCopyBufferSubData(GL_COPY_WRITE_BUFFER, GL_COPY_READ_BUFFER, 0, 0, oldSize);
		}

		// TODO: check if one copy/attribpointer is faster than copy/copy
		// GLuint t = vbos;
		// vbos = tempVbo;
		// tempVbo = t;

		glDeleteBuffers(1, &tempVbo);

		spriteCapacity *= 2;
	}

	setRotation(rotation);
	bindTextureData();

	setDefaultShaders(this, texture.getId() == 0);
}

UnsortedSprite::~UnsortedSprite() {
	print("Unloading sprite");

	if (id < spriteCount - 1)
		deletedQueue.push(id);
	else
		spriteCount--;
}

void UnsortedSprite::bindVertexData() {
	glBindBuffer(GL_ARRAY_BUFFER, vbos[vboType::VERTEX]);

	float data[SPRITE_VERTS];
	int offset = 0;

	for (int i = 0; i < SPRITE_VERTS; i += 2) {
		Vector vec = position + (Vector(quad[i], quad[i + 1]) * texture.getBounds() * scale).rotate(sinRot, cosRot);
		if (parent)	vec = parent->position + vec.rotate(parent->sinRot, parent->cosRot);

		loadVector(vec, data, &offset);
	}

	glBufferSubData(GL_ARRAY_BUFFER, id * SPRITE_SIZE, SPRITE_SIZE, data);

	for (UnsortedSprite* child : children)
		child->bindVertexData();
}

void UnsortedSprite::bindTextureData() {
	glBindBuffer(GL_ARRAY_BUFFER, vbos[vboType::TEXTURE]);

	float data[SPRITE_VERTS];
	int offset = 0;

	for (int i = 0; i < SPRITE_VERTS; i += 2)
		loadVector(texture.getPosition() + (Vector(quad[i], quad[i + 1]) + 0.5f) * texture.getBounds(), data, &offset);

	glBufferSubData(GL_ARRAY_BUFFER, id * SPRITE_SIZE, SPRITE_SIZE, data);
}

void UnsortedSprite::draw() {
	texture.bind();

	useShaders(vertexShader, fragmentShader, color);

	glDrawArrays(GL_TRIANGLE_STRIP, id*4, 4);
}
