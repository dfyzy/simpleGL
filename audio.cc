#include <memory>
#include <sndfile.h>

#include "audio.h"
#include "util.h"

namespace simpleGL {

Audio::Audio() {
	util::println("Audio:load");

	alGenBuffers(1, &id);
}

Audio* Audio::loadData(unsigned channels, unsigned length, unsigned sampleRate, const short* data) {
	util::println("Audio:loadData");

	ALenum format {0};
	if (channels == 1)		format = AL_FORMAT_MONO16;
	else if (channels == 2)	format = AL_FORMAT_STEREO16;
	else if (channels == 4)	format = alGetEnumValue("AL_FORMAT_QUAD16");
	else if (channels == 6)	format = alGetEnumValue("AL_FORMAT_51CHN16");
	else if (channels == 7)	format = alGetEnumValue("AL_FORMAT_61CHN16");
	else if (channels == 8)	format = alGetEnumValue("AL_FORMAT_71CHN16");

	if (format == 0) {
		util::println("error:Audio:unsupported number of channels");
		return this;
	}

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

Audio* Audio::loadData(std::string path) {
	util::println("Audio:load file:" + path);

	SF_INFO info;
	SNDFILE* file = sf_open(path.c_str(), SFM_READ, &info);

	if (!file) {
		int result = sf_error(NULL);

		std::string errorString;
		if (result == SF_ERR_UNRECOGNISED_FORMAT)			errorString = "unrecognised format";
		else if (result == SF_ERR_SYSTEM)					errorString = "system error";
		else if (result == SF_ERR_MALFORMED_FILE)			errorString = "malformed file";
		else if (result == SF_ERR_UNSUPPORTED_ENCODING)	errorString = "unsupported encoding";
		else	errorString = sf_error_number(result);

		util::println("error:libsndfile:failed to open file:" + errorString);

		return this;
	}

	int samples = info.frames*info.channels;
	std::unique_ptr<short[]> data {new short[samples]};
	sf_read_short(file, data.get(), samples);

	sf_close(file);

	return loadData(info.channels, info.frames, info.samplerate, data.get());
}

Audio::~Audio() {
	util::println("Audio:unload");

	alDeleteBuffers(1, &id);
}

}
