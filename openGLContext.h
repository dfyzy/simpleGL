#ifndef SIMPLE_OPENGL_CONTEXT_H
#define SIMPLE_OPENGL_CONTEXT_H

#include <queue>

#include "openGLContextTypes.h"

namespace simpleGL {

class OpenGLContext {
private:
	static constexpr int RESIZE_FACTOR = 2;

	GLuint vao;
	GLuint vbos[(int)EBufferDataType::Count];

	std::queue<GLint> deletedQIDs;

	GLsizei quadCount = 0;
	GLsizei quadCapacity = 64;

public:
	OpenGLContext();
	~OpenGLContext();

	GLuint getVAO() const { return vao; }

	GLuint getVBO(EBufferDataType type) const { return vbos[(int)type] ; }

	GLint loadQID();
	void unloadQID(GLint qid);

};

}

#endif
