#include <queue>

#include "drawObject.h"
#include "glfw.h"
#include "util.h"

namespace {

constexpr int RESIZE_FACTOR = 2;

GLuint vbos[(int)simpleGL::EDataType::Count];
bool buffersNotInited {true};

int dataSize[(int)simpleGL::EDataType::Count] { 2, 2, 4 };

std::queue<unsigned> deletedDos;

unsigned objectCount = 0;
unsigned objectCapacity = 4;

}

namespace simpleGL {

DrawObject::DrawObject() {
	if (buffersNotInited) {
		buffersNotInited = false;

		util::println("Data buffers:load");

		glGenBuffers((int)EDataType::Count, vbos);

		for (int i = 0; i < (int)EDataType::Count; i++) {
			glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);

			//alocating data for objectCapacity number of objects.
			glBufferData(GL_ARRAY_BUFFER, objectCapacity * dataSize[i]*QUAD_VERTS*sizeof(float), nullptr, GL_DYNAMIC_DRAW);

			//binding buffers to layout locations in vertex shader.
			glVertexAttribPointer(i, dataSize[i], GL_FLOAT, GL_FALSE, 0, nullptr);
			glEnableVertexAttribArray(i);
		}
	}

	if (!deletedDos.empty()) {
		id = deletedDos.front();
		deletedDos.pop();
	}else	id = objectCount++;

	if (objectCapacity < objectCount) {
		util::println(std::string("Data buffers:resize:") + std::to_string(objectCapacity*RESIZE_FACTOR));

		for (int i = 0; i < (int)EDataType::Count; i++) {
			GLuint tempVbo;
			glGenBuffers(1, &tempVbo);

			int oldSize = objectCapacity * dataSize[i]*QUAD_VERTS*sizeof(float);
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

		objectCapacity *= RESIZE_FACTOR;
	}
}

DrawObject::~DrawObject() {
	if (id < objectCount - 1)
		deletedDos.push(id);
	else
		objectCount--;
}

void DrawObject::bindData(EDataType type, float data[]) const {
	if (type == EDataType::Count) {
		util::println("error:DrawObject:do not use DataType::Count");
		return;
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbos[(int)type]);

	int size = dataSize[(int)type]*QUAD_VERTS*sizeof(float);
	glBufferSubData(GL_ARRAY_BUFFER, id * size, size, data);
}

void DrawObject::bindColorData(Color color) const {
	int vertSize = dataSize[(int)EDataType::Color];
	int size = vertSize*QUAD_VERTS;
	float data[size];

	for (int i = 0; i < size; i += vertSize) {
		data[i] = color.r; data[i + 1] = color.g; data[i + 2] = color.b; data[i + 3] = color.a;
	}

	bindData(EDataType::Color, data);
}

void DrawObject::draw() const {
	glDrawArrays(GL_TRIANGLE_STRIP, id*4, 4);
}

}
