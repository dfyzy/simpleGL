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

	Audio* loadData(unsigned chanels, unsigned length, unsigned sampleRate, const short* data);
	Audio* loadData(std::string path);

	ALuint getId() const { return id; }

	void unload() { delete this; }

};

}

#endif
