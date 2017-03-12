#include <queue>

#include "shader.h"
#include "util.h"
#include "shaderData.h"

namespace {

constexpr float SPRITE_QUAD[] = {-0.5, 0.5,
											-0.5, -0.5,
											0.5, 0.5,
											0.5, -0.5};

constexpr int SPRITE_SIZE = sizeof(SPRITE_QUAD);
constexpr int SPRITE_VERTS = SPRITE_SIZE/sizeof(float);

GLuint vbos[simpleGL::vboType::COUNT];

std::queue<unsigned> deletedQueue;

unsigned spriteCount = 0;
unsigned spriteCapacity = 4;

GLuint defaultVertexShader;

GLuint defaultFragmentShader;
GLuint emptyFragmentShader;

}

namespace simpleGL {

void util::bindData(unsigned id, vboType::E type, Vector centre, Vector bounds, Angle rotation) {
	glBindBuffer(GL_ARRAY_BUFFER, vbos[type]);

	float data[SPRITE_VERTS];
	int offset = 0;

	for (int i = 0; i < SPRITE_VERTS; i += 2)
		(centre + (bounds*Vector(SPRITE_QUAD[i], SPRITE_QUAD[i + 1])).rotate(rotation)).load(data, &offset);

	glBufferSubData(GL_ARRAY_BUFFER, id * SPRITE_SIZE, SPRITE_SIZE, data);
}

void util::initSprites() {
	glGenBuffers(vboType::COUNT, vbos);

	for (int i = 0; i < vboType::COUNT; i++) {
		glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);

		//alocating data for spriteCapacity number of sprites for a start.
		glBufferData(GL_ARRAY_BUFFER, spriteCapacity * SPRITE_SIZE, nullptr, GL_DYNAMIC_DRAW);

		//binding buffers to layout locations in vertex shader.
		glVertexAttribPointer(i, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(i);
	}

	defaultVertexShader = loadShaderSource(simpleShaderData::getVertex(), GL_VERTEX_SHADER);

	defaultFragmentShader = loadShaderSource(simpleShaderData::getDefaultFragment(), GL_FRAGMENT_SHADER);
	emptyFragmentShader = loadShaderSource(simpleShaderData::getEmptyFragment(), GL_FRAGMENT_SHADER);

	util::print("Sprites initialized");
}

UnsortedSprite::UnsortedSprite(UnsortedSprite* parent, Texture texture, Vector position, Vector scale, Angle rotation, Color color)
											: parent(parent), texture(texture), position(position), scale(scale), rotation(rotation), color(color) {
	util::print("Adding sprite");

	if (parent) parent->children.push_back(this);

	if (!deletedQueue.empty()) {
		id = deletedQueue.front();
		deletedQueue.pop();
	}else	id = spriteCount++;

	if (spriteCapacity < spriteCount) {
		util::print("Resizing data buffers");

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

	bindVertexData();
	bindTextureData();

	vertexShader = defaultVertexShader;
	if (texture.getId() == 0)	fragmentShader = emptyFragmentShader;
	else								fragmentShader = defaultFragmentShader;
}

UnsortedSprite::~UnsortedSprite() {
	util::print("Unloading sprite");

	if (id < spriteCount - 1)
		deletedQueue.push(id);
	else
		spriteCount--;
}

void UnsortedSprite::bindVertexData() {
	Vector pos = position;

	UnsortedSprite* nextParent = parent;
	Angle angle = rotation;
	while (nextParent) {
		pos = nextParent->position + pos.rotate(nextParent->rotation);

		angle += nextParent->rotation;

		nextParent = nextParent->parent;
	}

	util::bindData(id, vboType::VERTEX, pos, texture.getBounds() * scale, angle);
}

void UnsortedSprite::bindTextureData() {

	util::bindData(id, vboType::TEXTURE, texture.getPosition() + texture.getBounds()*0.5f, texture.getBounds(), {});
}

void UnsortedSprite::draw() {
	texture.bind();

	util::useShaders(vertexShader, fragmentShader, color);

	glDrawArrays(GL_TRIANGLE_STRIP, id*4, 4);
}

}
