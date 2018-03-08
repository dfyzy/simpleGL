#ifndef SIMPLE_OPENAL_CONTEXT_H
#define SIMPLE_OPENAL_CONTEXT_H

#include <AL/alext.h>

namespace simpleGL {

class OpenALContext {
private:
	ALCcontext* context = nullptr;
	ALCdevice* device = nullptr;

public:
	OpenALContext();
	~OpenALContext();

};

}

#endif
