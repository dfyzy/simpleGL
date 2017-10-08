#include "sound.h"
#include "soundFile.h"
#include "util.h"

namespace simpleGL {

Sound::Sound() {
	util::println("Sound:load");

	alGenBuffers(1, &id);
}

Sound* Sound::loadData(int pchannels, unsigned psampleRate, unsigned plength, const short* data) {
	ALenum format {0};
	if (pchannels == 1)			format = AL_FORMAT_MONO16;
	else if (pchannels == 2)	format = AL_FORMAT_STEREO16;
	else if (pchannels == 4)	format = alGetEnumValue("AL_FORMAT_QUAD16");
	else if (pchannels == 6)	format = alGetEnumValue("AL_FORMAT_51CHN16");
	else if (pchannels == 7)	format = alGetEnumValue("AL_FORMAT_61CHN16");
	else if (pchannels == 8)	format = alGetEnumValue("AL_FORMAT_71CHN16");

	if (format == 0) {
		util::println("error:Sound:unsupported number of channels");
		return this;
	}

	channels = pchannels;
	length = plength;
	sampleRate = psampleRate;

	alBufferData(id, format, data, length*channels*sizeof(short), sampleRate);

	ALenum error = alGetError();
	if (error != AL_NO_ERROR) {
		std::string errorString;
		if (error == AL_INVALID_OPERATION)	errorString = "invalid operation";
		else if (error == AL_INVALID_ENUM)	errorString = "invalid format";
		else if (error == AL_INVALID_VALUE)	errorString = "invalid value";
		else if (error == AL_INVALID_NAME)	errorString = "invalid name";
		else if (error == AL_OUT_OF_MEMORY)	errorString = "out of memory";

		util::println("error:OpenAL:" + errorString);
	}

	return this;
}

Sound* Sound::loadData(std::string path) {
	SoundFile soundFile(path);
	soundFile.readAll(this);

	return this;
}

Sound::~Sound() {
	util::println("Sound:unload");

	alDeleteBuffers(1, &id);
}

}
