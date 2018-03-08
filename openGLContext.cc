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
}

OpenGLContext::~OpenGLContext() {
	println("OpenGL:unload");

	//TODO
}

}
