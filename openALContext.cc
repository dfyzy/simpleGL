#include "openALContext.h"
#include "log.h"

namespace simpleGL {

OpenALContext::OpenALContext() {
	println("OpenAL:load");
	device = alcOpenDevice(NULL);

	if (!device) {
		println("error:OpenAL:failed to create device");
		return;
	}

	context = alcCreateContext(device, NULL);
	if (!context) {
		println("error:OpenAL:failed to create context");
		return;
	}

	alcMakeContextCurrent(context);
}

OpenALContext::~OpenALContext() {
	println("OpenAL:unload");

	alcMakeContextCurrent(nullptr);

	alcDestroyContext(context);
	alcCloseDevice(device);
}

}
