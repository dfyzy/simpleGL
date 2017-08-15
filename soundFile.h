/* File managment for sound data
*/

#ifndef SIMPLE_SOUND_FILE_H
#define SIMPLE_SOUND_FILE_H

#include <sndfile.h>

#include "sound.h"

namespace simpleGL {

class SoundFile {
private:
	SF_INFO info;
	SNDFILE* file;

public:
	SoundFile(std::string path);
	~SoundFile();

	bool read(Sound* sound, unsigned frames);
	void readAll(Sound* sound) {
		read(sound, info.frames);
	}

	void seek(unsigned frames) {
		sf_seek(file, frames, SEEK_SET);
	}

};

};

#endif
