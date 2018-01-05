#include <memory>

#include "soundFile.h"
#include "sound.h"
#include "util.h"

namespace simpleGL {

SoundFile::SoundFile(const std::string& path) {
	util::println("SoundFile:load:" + path);

	file = sf_open(path.c_str(), SFM_READ, &info);

	if (!file) {
		int result = sf_error(NULL);

		std::string errorString;
		if (result == SF_ERR_UNRECOGNISED_FORMAT)			errorString = "unrecognised format";
		else if (result == SF_ERR_SYSTEM)					errorString = "system error";
		else if (result == SF_ERR_MALFORMED_FILE)			errorString = "malformed file";
		else if (result == SF_ERR_UNSUPPORTED_ENCODING)	errorString = "unsupported encoding";
		else	errorString = sf_error_number(result);

		util::println("error:libsndfile:failed to open file:" + errorString);
	}
}

SoundFile::~SoundFile() {
	util::println("SoundFile:unload");

	sf_close(file);
}

bool SoundFile::read(Sound& sound, sf_count_t frames) {
	std::unique_ptr<short[]> data {new short[frames*info.channels]};

	sf_count_t result = sf_readf_short(file, data.get(), frames);
	if (result == 0)	return false;

	sound.loadData(info.channels, info.samplerate, result, data.get());

	return true;
}

}
