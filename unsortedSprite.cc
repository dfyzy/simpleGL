#include <queue>

#include "shader.h"
#include "util.h"
#include "shaderData.h"

namespace {

constexpr int RESIZE_FACTOR = 2;

constexpr float SPRITE_QUAD[] = {-0.5f, 0.5f,
											-0.5f, -0.5f,
											0.5f, 0.5f,
											0.5f, -0.5f};

constexpr int SPRITE_VERTS = 4;

GLuint vbos[simpleGL::vboType::COUNT];
int vboSize[simpleGL::vboType::COUNT] { 2, 2, 4 };

std::queue<unsigned> deletedQueue;

unsigned spriteCount = 0;
unsigned spriteCapacity = 4;

std::list<simpleGL::UnsortedSprite*> unsortedSprites;

GLuint defaultVertexShader;

GLuint defaultFragmentShader;
GLuint emptyFragmentShader;

}

namespace simpleGL {

Vector quadVertex(int i) {

	return Vector(SPRITE_QUAD[i*2], SPRITE_QUAD[i*2 + 1]);
}

void bindVboData(unsigned id, vboType::E type, float data[]) {
	glBindBuffer(GL_ARRAY_BUFFER, vbos[type]);

	int size = vboSize[type]*SPRITE_VERTS*sizeof(float);
	glBufferSubData(GL_ARRAY_BUFFER, id * size, size, data);
}

void util::bindQuadData(unsigned id, vboType::E type, Matrix model) {
	float data[SPRITE_VERTS*2];
	int offset = 0;

	for (int i = 0; i < SPRITE_VERTS; i++)
		(model*quadVertex(i)).load(data, &offset);//TODO: change it

	bindVboData(id, type, data);
}

void util::initSprites() {
	util::print("Sprite data buffers:load");

	glGenBuffers(vboType::COUNT, vbos);

	for (int i = 0; i < vboType::COUNT; i++) {
		glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);

		//alocating data for spriteCapacity number of sprites for a start.
		glBufferData(GL_ARRAY_BUFFER, spriteCapacity * vboSize[i]*SPRITE_VERTS*sizeof(float), nullptr, GL_DYNAMIC_DRAW);

		//binding buffers to layout locations in vertex shader.
		glVertexAttribPointer(i, vboSize[i], GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(i);
	}

	defaultVertexShader = loadShaderSource(simpleShaderData::getVertex(), GL_VERTEX_SHADER);

	defaultFragmentShader = loadShaderSource(simpleShaderData::getDefaultFragment(), GL_FRAGMENT_SHADER);
	emptyFragmentShader = loadShaderSource(simpleShaderData::getEmptyFragment(), GL_FRAGMENT_SHADER);
}

void util::bindSprites() {
	for (UnsortedSprite* us : unsortedSprites)
		us->bindData();
}

UnsortedSprite::UnsortedSprite(Point* parent, Texture texture, Anchor anchor, Vector position, Vector scale, Angle rotation, Color color)
							: BaseBoxShape(parent, anchor, position, scale, rotation), texture(texture), color(color) {
	util::print("UnsortedSprite:load");

	vertexShader = defaultVertexShader;
	setDefaultFragmentShader();

	if (!deletedQueue.empty()) {
		id = deletedQueue.front();
		deletedQueue.pop();
	}else	id = spriteCount++;

	if (spriteCapacity < spriteCount) {
		util::print("Sprite data buffers:resize");

		for (int i = 0; i < vboType::COUNT; i++) {
			GLuint tempVbo;
			glGenBuffers(1, &tempVbo);

			int oldSize = spriteCapacity * vboSize[i]*SPRITE_VERTS*sizeof(float);
			glBindBuffer(GL_COPY_WRITE_BUFFER, tempVbo);
			glBufferData(GL_COPY_WRITE_BUFFER, oldSize, nullptr, GL_DYNAMIC_DRAW);

			//loading current data into temp buffer, resizing this buffer and loading data back from temp buffer.
			glBindBuffer(GL_COPY_READ_BUFFER, vbos[i]);

			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, oldSize);

			glBufferData(GL_COPY_READ_BUFFER, RESIZE_FACTOR * oldSize, nullptr, GL_DYNAMIC_DRAW);

			glCopyBufferSubData(GL_COPY_WRITE_BUFFER, GL_COPY_READ_BUFFER, 0, 0, oldSize);

			glDeleteBuffers(1, &tempVbo);
		}

		//TOTRY: check if one copy/attribpointer is faster than copy/copy
		//GLuint t = vbos;
		//vbos = tempVbo;
		//tempVbo = t;

		spriteCapacity *= RESIZE_FACTOR;
	}

	unsortedSprites.push_back(this);
}

UnsortedSprite::~UnsortedSprite() {
	util::print("UnsortedSprite:unload");

	if (id < spriteCount - 1)
		deletedQueue.push(id);
	else
		spriteCount--;
	//TOTRY: check for redundant members of the queue.

	unsortedSprites.remove(this);
}

void UnsortedSprite::setDefaultFragmentShader() {
	if (texture.getImage() == nullptr)	fragmentShader = emptyFragmentShader;
	else											fragmentShader = defaultFragmentShader;

	defaultFrag = true;
}

void UnsortedSprite::bindVertices() {
	Matrix m = getModelMatrix() * Matrix::scale(texture.getBounds());

	util::bindQuadData(id, vboType::VERTEX, m);
}

void UnsortedSprite::bindColor() {
	int size = vboSize[vboType::COLOR]*SPRITE_VERTS;
	float data[size];

	for (int i = 0; i < size; i += vboSize[vboType::COLOR]) {
		data[i] = color.r; data[i + 1] = color.g; data[i + 2] = color.b; data[i + 3] = color.a;
	}

	bindVboData(id, vboType::COLOR, data);
}

void UnsortedSprite::bindTexture() {

	util::bindQuadData(id, vboType::TEXTURE,
			Matrix::translate(texture.getPosition() + texture.getBounds()*0.5f) * Matrix::scale(texture.getBounds()));
}

void UnsortedSprite::draw() {
	if (!isEnabled())	return;

	texture.bind();

	glStencilFunc(stencilFunc, stencilRef, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, stencilOp);

	util::useShaders(vertexShader, fragmentShader);

	glDrawArrays(GL_TRIANGLE_STRIP, id*4, 4);
}

}
