#include "drawObject.h"
#include "globalInstance.h"
#include "openGLContext.h"
#include "log.h"

namespace simpleGL {

DrawObject::DrawObject() {
	qid = GlobalInstance<OpenGLContext>::get()->loadQID();
}

DrawObject::~DrawObject() {
	GlobalInstance<OpenGLContext>::get()->unloadQID(qid);
}

void DrawObject::bindData(EBufferDataType type, float data[]) const {
	if (type == EBufferDataType::Count) {
		println("error:DrawObject:do not use DataType::Count");
		return;
	}

	glBindBuffer(GL_ARRAY_BUFFER, GlobalInstance<OpenGLContext>::get()->getVBO(type));

	int size = bufferDataSize[(int)type]*QUAD_VERTS*sizeof(float);
	glBufferSubData(GL_ARRAY_BUFFER, qid * size, size, data);
}

void DrawObject::bindColorData(Color color) const {
	int vertSize = bufferDataSize[(int)EBufferDataType::Color];
	int size = vertSize*QUAD_VERTS;
	float data[size];

	for (int i = 0; i < size; i += vertSize) {
		data[i] = color.r; data[i + 1] = color.g; data[i + 2] = color.b; data[i + 3] = color.a;
	}

	bindData(EBufferDataType::Color, data);
}

void DrawObject::draw() const {
	glDrawArrays(GL_TRIANGLE_STRIP, qid*QUAD_VERTS, QUAD_VERTS);
}

}
