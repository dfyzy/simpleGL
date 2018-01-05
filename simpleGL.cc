#include <windows.h>

#include <chrono>
#include <set>

#include "simpleGL.h"
#include "window.h"
#include "camera.h"
#include "updatable.h"
#include "util.h"

//temp
#include <AL/alext.h>

#ifdef FPS_COUNTER
	#include <iostream>
#endif

namespace {

typedef std::chrono::high_resolution_clock Clock;

GLuint vao;

Clock::time_point previous;
double deltaTime = 0;

}

namespace simpleGL {

double getDeltaTime() {
	return deltaTime;
}

SortedFrame* getMainFrame() {
	return Camera::getInstance()->getFrame();
}

void loadGLContext() {
	if (!Window::getCurrent()) {
		util::println("error:simpleGL:initGLContext is for internal uses only");
		return;
	}

	util::println("GLEW:load");
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK) {
		util::println("error:GLEW:failed to init");
		Window::terminate();
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


	util::println("OpenAL:load");
	ALCdevice* device = alcOpenDevice(NULL);

	if (!device) {
		util::println("error:OpenAL:failed to create device");
		return;
	}

	ALCcontext* context = alcCreateContext(device, NULL);
	if (!context) {
		util::println("error:OpenAL:failed to create context");
		return;
	}

	alcMakeContextCurrent(context);
}

void draw() {
	util::println("simpleGL:start of draw cycle");

	Window* current = Window::getCurrent();
	if (!current) {
		util::println("error:simpleGL:no current OpenGL context");
		return;
	}

	previous = Clock::now();

	#ifdef FPS_COUNTER
		double fpsTime = 0;
		int frames = 0;
	#endif
	while (!glfwWindowShouldClose(current->getWindow())) {
		Clock::time_point now = Clock::now();
		deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - previous).count() * 0.000001;
		previous = now;

		#ifdef FPS_COUNTER
			fpsTime += deltaTime;
			frames++;
			if (fpsTime >= 1) {
				std::cout << "fps: " << frames << "; spf: " << fpsTime/frames << ";" << std::endl;
				fpsTime = 0;
				frames = 0;
			}
		#endif

		glfwPollEvents();

		glBindVertexArray(vao);

		Updatable<EUpdateType::PreTick>::updateAll();

		Updatable<EUpdateType::Tick>::updateAll();

		Updatable<EUpdateType::PostTick>::updateAll();

		Camera::getInstance()->draw();

		glBindVertexArray(0);

		glfwSwapBuffers(current->getWindow());
	}

	util::println("GLFW:unload");

	Window::terminate();
}

}
