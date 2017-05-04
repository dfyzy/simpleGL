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
constexpr int SPRITE_VERTS = 4;

GLuint vbos[simpleGL::vboType::COUNT];

std::list<simpleGL::UnsortedSprite*> unsortedSprites;

std::queue<unsigned> deletedQueue;

unsigned spriteCount = 0;
unsigned spriteCapacity = 4;

GLuint defaultVertexShader;

GLuint defaultFragmentShader;
GLuint emptyFragmentShader;

}

namespace simpleGL {

Vector quadVertex(int i) {

	return Vector(SPRITE_QUAD[i*2], SPRITE_QUAD[i*2 + 1]);
}

void util::bindData(unsigned id, vboType::E type, Matrix model) {
	glBindBuffer(GL_ARRAY_BUFFER, vbos[type]);

	float data[SPRITE_VERTS*2];
	int offset = 0;

	for (int i = 0; i < SPRITE_VERTS; i++)
		(model*quadVertex(i)).round().load(data, &offset);//TODO: find any downsides to this method

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

void util::bindSprites() {
	for (UnsortedSprite* us : unsortedSprites)
		us->bindData();
}

UnsortedSprite::UnsortedSprite(Point* parent, Texture texture, Vector position, Vector scale, Angle rotation, Color color)
										: Point(parent, position, scale, rotation), texture(texture), color(color), bounds(texture.getBounds()) {
	util::print("Adding sprite");

	vertexShader = defaultVertexShader;
	if (texture.getImage() == nullptr)	fragmentShader = emptyFragmentShader;
	else											fragmentShader = defaultFragmentShader;

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

		//TOTRY: check if one copy/attribpointer is faster than copy/copy
		//GLuint t = vbos;
		//vbos = tempVbo;
		//tempVbo = t;

		glDeleteBuffers(1, &tempVbo);

		spriteCapacity *= 2;
	}

	unsortedSprites.push_back(this);
}

UnsortedSprite::~UnsortedSprite() {
	util::print("Unloading sprite");

	if (id < spriteCount - 1)
		deletedQueue.push(id);
	else
		spriteCount--;
	//TOTRY: check for redundant members of the queue.

	unsortedSprites.remove(this);
}

void UnsortedSprite::bindVertices() {
	Matrix m = getModelMatrix() * Matrix::scale(texture.getBounds());

	util::bindData(id, vboType::VERTEX, m);
}

void UnsortedSprite::bindTexture() {

	util::bindData(id, vboType::TEXTURE,
			Matrix::translate(texture.getPosition() + texture.getBounds()*0.5f) * Matrix::scale(texture.getBounds()));
}

//TODO: sleep on this
bool UnsortedSprite::inBounds(UnsortedSprite* other) {
	if (!isEnabled() || !other->isEnabled())	return false;

	Matrix mat = other->getModelMatrix();
	for (int i = 0; i < SPRITE_VERTS; i++)
		if (inBounds(mat * quadVertex(i)))	return true;

	mat = getModelMatrix();
	for (int i = 0; i < SPRITE_VERTS; i++)
		if (other->inBounds(mat * quadVertex(i)))	return true;

	return false;
}

void UnsortedSprite::draw() {
	if (!isEnabled())	return;

	texture.bind();

	glStencilFunc(stencilFunc, stencilRef, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, stencilOp);

	util::useShaders(vertexShader, fragmentShader, color);

	glDrawArrays(GL_TRIANGLE_STRIP, id*4, 4);
}

}
