#include <windows.h>

#include "openGLContext.h"
#include "log.h"

namespace simpleGL {

OpenGLContext::OpenGLContext() {
	println("GLEW:load");
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK) {
		println("error:GLEW:failed to init");
		return;
	}

	#ifdef _WIN32
		// Turn on vertical screen sync if on Windows.
		typedef BOOL (WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int interval);
		PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
		if(wglSwapIntervalEXT)
			wglSwapIntervalEXT(1);
	#else
		glfwSwapInterval(1);
	#endif

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_STENCIL_TEST);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	println("Data buffers:load");

	glGenBuffers((int)EBufferDataType::Count, vbos);

	for (int i = 0; i < (int)EBufferDataType::Count; i++) {
		glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);

		//alocating data for quadCapacity number of quads.
		glBufferData(GL_ARRAY_BUFFER, quadCapacity * bufferDataSize[i]*QUAD_VERTS*sizeof(float), nullptr, GL_DYNAMIC_DRAW);

		//binding buffers to layout locations in vertex shader.
		glVertexAttribPointer(i, bufferDataSize[i], GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(i);
	}
}

OpenGLContext::~OpenGLContext() {
	println("OpenGL:unload");

	glDeleteBuffers((int)EBufferDataType::Count, vbos);
	glDeleteVertexArrays(1, &vao);
}

GLint OpenGLContext::loadQID() {
	GLint qid;

	if (!deletedQIDs.empty()) {
		qid = deletedQIDs.front();

		deletedQIDs.pop();
	} else {
		qid = quadCount++;

		if (quadCapacity < quadCount) {
			println(std::string("Data buffers:resize:") + std::to_string(quadCapacity*RESIZE_FACTOR));

			for (int i = 0; i < (int)EBufferDataType::Count; i++) {
				GLuint tempVbo;
				glGenBuffers(1, &tempVbo);

				int oldSize = quadCapacity * bufferDataSize[i]*QUAD_VERTS*sizeof(float);
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

			quadCapacity *= RESIZE_FACTOR;
		}
	}

	return qid;
}

void OpenGLContext::unloadQID(GLint qid) {
	if (qid < quadCount - 1)
		deletedQIDs.push(qid);
	else
		quadCount--;
}

}
