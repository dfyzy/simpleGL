#ifndef SIMPLE_AUDIO_H
#define SIMPLE_AUDIO_H

#include <AL/alext.h>
#include <string>

namespace simpleGL {

class Audio {
private:
	ALuint id {0};

protected:
	~Audio();

public:
	Audio();

	Audio& loadData(int chanels, int length, int sampleRate, const short* data);
	Audio& loadData(std::string path);

};

}

#endif
