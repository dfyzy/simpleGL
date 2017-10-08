/* File managment for sound data
*/

#ifndef SIMPLE_SOUND_FILE_H
#define SIMPLE_SOUND_FILE_H

#include <sndfile.h>

#include <string>

namespace simpleGL {

class SoundFile {
private:
	SF_INFO info;
	SNDFILE* file;

public:
	SoundFile(std::string path);
	~SoundFile();

	sf_count_t getLength() const { return info.frames; }
	int getSampleRate() { return info.samplerate; }
	int getChannels() { return info.channels; }
	int getFormat() { return info.format; }

	bool read(class Sound* sound, sf_count_t frames);
	void readAll(Sound* sound) {
		read(sound, info.frames);
	}

	void seek(sf_count_t frames) {
		sf_seek(file, frames, SEEK_SET);
	}

};

};

#endif
